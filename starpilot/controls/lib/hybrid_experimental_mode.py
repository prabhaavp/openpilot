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
  1. Open Road: Follows Chill MPC cruise & radar headway.
  2. Slower Lead Approach: Blends smooth vision decel with MPC follow distance.
  3. Red Lights / Stop Signs: Latches pure vision braking with active zero-speed ramp.
  4. Rolling-Dip Immune: Decelerates all the way to 0 m/s even if model trajectory rises at low speeds.
  5. Standstill Hold & Departure: Locks mechanical brake hold until green light/lead departure/gas press.
  """

  STANDSTILL_SPEED = 0.6       # m/s (~1.3 mph)
  STOP_ENGAGE_DECEL = -0.15    # m/s^2 (threshold to enter stopping state)
  MINOR_DIFF_THRESHOLD = 0.25  # m/s^2

  def __init__(self):
    self.DT = DT_MDL
    self.w_vision = 0.0
    self.prev_a_target = 0.0
    self.last_exp_dominant = False
    self.stopping_latched = False
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
    self.diag = {}

  def set_tuning(self, exp_bias: float, vision_brake_sensitivity: float):
    self.HYBRID_EXP_BIAS = clamp(exp_bias, -1.0, 1.0)
    self.VISION_BRAKE_SENSITIVITY = clamp(vision_brake_sensitivity, 0.0, 2.0)

  def update(self, v_ego, v_cruise, lead_one, model_v2, a_chill, a_exp,
             should_stop_exp=False, should_stop_chill=False, gas_pressed=False):

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
    # Verified green light: sustained positive model accel and open lookahead
    vision_departing = at_standstill and (v_horizon > 3.0) and (v_short > 1.5) and (a_exp > 0.25)
    is_departing = lead_departing or vision_departing or driver_override

    # 3. Stopping Latch Management
    # Trigger stopping latch on real vision decel, stop request, or low-speed stop line approach
    wants_stopping = (
      (a_exp < (a_chill - 0.05) and a_exp < self.STOP_ENGAGE_DECEL) or
      should_stop_exp or
      (v_ego < 3.5 and v_min < 1.0 and a_exp < 0.0)
    )

    if driver_override or is_departing:
      self.stopping_latched = False
    elif wants_stopping:
      self.stopping_latched = True
    elif self.stopping_latched and not at_standstill:
      # Only clear latch while rolling if the road clearly opens (e.g. light turns green mid-slowdown)
      if a_exp > 0.40 and v_min > (v_ego * 0.95) and not should_stop_exp:
        self.stopping_latched = False

    # 4. Output Arbitration & Regime Fusion
    if self.stopping_latched:
      self.last_exp_dominant = True
      self.w_vision = 1.0

      if a_exp < -0.20:
        # Vision model is actively commanding strong decel
        a_vision_brake = a_exp * self.VISION_BRAKE_SENSITIVITY
        a_out = min(a_chill, a_vision_brake)
      else:
        # Rolling dip / low-speed completion: enforce smooth decel ramp to 0 m/s
        # (Prevents coasting/surging when model trajectory bottoms out at ~3 mph)
        a_stop_ramp = -clamp(v_ego * 0.75 + 0.35, 0.45, 1.20)
        a_out = min(a_chill, a_stop_ramp)

    else:
      # CRUISE / THROTTLE REGIME
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

    # 5. Authoritative Standstill Handshake
    # Bring to full mechanical stop hold when stopping is latched near standstill
    standstill_intent = not is_departing and (
      should_stop_exp or
      (self.stopping_latched and v_ego < 1.2) or
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
        "exp_dominant": self.last_exp_dominant,
        "standstill_intent": standstill_intent,
        "should_stop_fused": should_stop_fused,
        "a_out": a_out,
        "regime": "pure_brake" if self.stopping_latched else "cruise",
        "standstill": standstill_intent,
      }

    self.prev_a_target = a_out
    return a_out, should_stop_fused