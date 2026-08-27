#!/usr/bin/env python3
import time
import math
from openpilot.common.realtime import DT_MDL


def lerp(a: float, b: float, t: float) -> float:
  """Linear interpolation between a and b by weight t (0.0 to 1.0)."""
  return (1.0 - t) * a + t * b


def clamp(val: float, low: float, high: float) -> float:
  return max(low, min(high, val))


class HybridExperimentalMode:
  """
  Hybrid Experimental Mode (HEM)
  1. Open Road: Pure Chill MPC cruise & radar follow distance.
  2. Pure Braking Authority: Experimental decel dominates 100% with time-based latching.
  3. Rolling-Dip Immunity: Cruise throttle locked out during stops; low-speed dips cannot surge.
  4. Subtle Blending: Smooth transitions only when throttle commands are nearly identical.
  5. Standstill Lock: Hardware brake hold latched until verified green/lead departure.
  """

  # Time in seconds to hold braking dominance once triggered (immune to rolling dips)
  BRAKE_HOLD_TIME = 3.0
  STANDSTILL_SPEED = 0.8  # m/s (~1.8 mph)
  MINOR_DIFF_THRESHOLD = 0.25  # m/s^2

  def __init__(self):
    self.DT = DT_MDL
    self.w_vision = 0.0
    self.prev_a_target = 0.0
    self.last_exp_dominant = False
    self.stopping_latched = False
    self.brake_hold_until = 0.0
    self.diag = {}
    self.record_diag = False

    # Tunings
    self.HYBRID_EXP_BIAS = 0.0
    self.VISION_BRAKE_SENSITIVITY = 1.0

  def reset(self, a_ego: float = 0.0):
    """Seed target with actual vehicle acceleration on engagement to prevent torque bumps."""
    self.prev_a_target = float(a_ego) if math.isfinite(a_ego) else 0.0
    self.w_vision = 0.0
    self.last_exp_dominant = False
    self.stopping_latched = False
    self.brake_hold_until = 0.0
    self.diag = {}

  def set_tuning(self, exp_bias: float, vision_brake_sensitivity: float):
    self.HYBRID_EXP_BIAS = clamp(exp_bias, -1.0, 1.0)
    self.VISION_BRAKE_SENSITIVITY = clamp(vision_brake_sensitivity, 0.0, 2.0)

  def update(self, v_ego, v_cruise, lead_one, model_v2, a_chill, a_exp,
             should_stop_exp=False, should_stop_chill=False, gas_pressed=False):
    now = time.monotonic()

    # 0. Sanitize inputs
    if not math.isfinite(a_chill):
      a_chill = self.prev_a_target
    if not math.isfinite(a_exp):
      a_exp = a_chill

    # 1. Trajectory Analysis
    traj_v = getattr(getattr(model_v2, "velocity", None), "x", None)
    has_full_trajectory = bool(traj_v and len(traj_v) >= 24 and all(math.isfinite(v) for v in traj_v))

    if has_full_trajectory:
      v_horizon = float(traj_v[-1])
      v_short = float(traj_v[23])  # ~4.0s lookahead
      v_min = float(min(traj_v))
    else:
      v_horizon = v_short = v_min = float(v_ego)

    lead_status = bool(getattr(lead_one, "status", False))
    lead_v = float(getattr(lead_one, "vLead", 0.0))
    lead_d = float(getattr(lead_one, "dRel", 150.0))

    # 2. Standstill & Departure Detection
    at_standstill = v_ego < self.STANDSTILL_SPEED
    driver_override = bool(gas_pressed)
    lead_departing = at_standstill and lead_status and (lead_v > 0.6) and ((lead_v - v_ego) > 0.4)
    # Require sustained vision acceleration & open lookahead to verify departure
    vision_departing = at_standstill and (v_horizon > 2.5) and (v_short > 1.2) and (a_exp > 0.20)
    is_departing = lead_departing or vision_departing or driver_override

    # 3. Pure Braking & Stopping Trigger
    # Trigger on any real vision decel, stop request, or low-speed stop-line trajectory
    wants_pure_braking = (
      (a_exp < (a_chill - 0.05) and a_exp < -0.10) or
      should_stop_exp or
      (v_min < 0.5 and v_ego < 4.0 and a_exp < 0.1)
    )

    # 4. Stopping State & Time-Based Latch Management
    if driver_override or is_departing:
      self.stopping_latched = False
      self.brake_hold_until = 0.0
    elif wants_pure_braking:
      self.stopping_latched = True
      self.brake_hold_until = now + self.BRAKE_HOLD_TIME
    elif self.stopping_latched and not at_standstill:
      # Road opened up mid-slowdown (e.g. light turned green before complete stop)
      if a_exp > 0.35 and v_min > (v_ego * 0.9) and not should_stop_exp and now >= self.brake_hold_until:
        self.stopping_latched = False

    latch_active = (now < self.brake_hold_until) or self.stopping_latched

    # 5. Output Arbitration & Regime Fusion
    if latch_active:
      # PURE BRAKING DOMINANCE: Lock out positive cruise throttle completely
      self.last_exp_dominant = True
      self.w_vision = 1.0

      if a_exp < 0.0:
        a_vision_brake = a_exp * self.VISION_BRAKE_SENSITIVITY
        a_out = min(a_chill, a_vision_brake)
      else:
        # Near bottom of rolling dip: prevent positive acceleration surges
        a_out = min(a_chill, 0.0)

    else:
      # CRUISE / THROTTLE REGIME:
      self.last_exp_dominant = False
      a_diff = a_exp - a_chill

      if a_diff < 0.0 and a_exp < 0.0:
        # Transient mild deceleration
        a_out = min(a_chill, a_exp)
        self.w_vision = max(0.0, self.w_vision - 0.05)
      elif abs(a_diff) <= self.MINOR_DIFF_THRESHOLD:
        # Minor difference: smooth blend between Chill and Exp
        blend_weight = abs(a_diff) / self.MINOR_DIFF_THRESHOLD
        a_out = lerp(a_chill, a_exp, blend_weight * 0.5)
        self.w_vision = max(0.0, self.w_vision - 0.05)
      else:
        # Open road acceleration governed by Chill MPC + optional exp bias
        a_out = a_chill + max(0.0, a_diff) * self.HYBRID_EXP_BIAS
        self.w_vision = max(0.0, self.w_vision - 0.05)

    # 6. Authoritative Standstill Handshake
    standstill_intent = not is_departing and (
      should_stop_exp or
      (self.stopping_latched and v_ego < 0.6) or
      (v_ego < 0.5 and (v_horizon < 1.0 or a_exp < -0.05))
    )
    should_stop_fused = bool(should_stop_chill or standstill_intent)

    if self.record_diag:
      self.diag = {
        "v_ego": v_ego, "v_cruise": v_cruise,
        "a_chill": a_chill, "a_exp": a_exp,
        "should_stop_chill": should_stop_chill,
        "should_stop_exp": should_stop_exp,
        "lead_status": lead_status, "lead_d_rel": lead_d, "lead_v_lead": lead_v,
        "has_full_trajectory": has_full_trajectory,
        "v_horizon": v_horizon, "v_short": v_short, "v_min": v_min,
        "w_vision": self.w_vision,
        "lead_departing": lead_departing,
        "vision_departing": vision_departing,
        "driver_override": driver_override,
        "is_departing": is_departing,
        "stopping_latched": self.stopping_latched,
        "latch_active": latch_active,
        "exp_dominant": self.last_exp_dominant,
        "standstill_intent": standstill_intent,
        "should_stop_fused": should_stop_fused,
        "a_out": a_out,
        "regime": "pure_brake" if latch_active else "cruise",
        "standstill": standstill_intent,
      }

    self.prev_a_target = a_out
    return a_out, should_stop_fused