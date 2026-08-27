#!/usr/bin/env python3
import math
from openpilot.common.realtime import DT_MDL


def lerp(a: float, b: float, t: float) -> float:
  """Linear interpolation / blend between a and b by weight t (0.0 to 1.0)."""
  return (1.0 - t) * a + t * b


def clamp(val: float, low: float, high: float) -> float:
  return max(low, min(high, val))


class HybridExperimentalMode:
  """
  1. Open Road: Follows Chill MPC cruise & radar headway.
  2. Slower Lead Approach: Blends smooth vision decel with MPC follow distance.
  3. Stop Signs / Red Lights: Pure vision stopping authority (locks out positive cruise throttle).
  4. Standstill / Stop Completion: Latches should_stop for LongControl mechanical brake hold.
  5. Green Light / Lead Depart: Instant release back to Chill cruise acceleration.
  """

  def __init__(self):
    self.DT = DT_MDL
    self.w_vision = 0.0
    self.prev_a_target = 0.0
    self.last_exp_dominant = False
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

    # 2. Vision Departure / Driver Override Detection (Priority Check)
    at_standstill = v_ego < 0.8
    lead_departing = at_standstill and lead_status and (lead_v > 0.6) and ((lead_v - v_ego) > 0.4)
    vision_departing = at_standstill and (v_horizon > 1.5) and (a_exp > 0.15)
    driver_override = bool(gas_pressed)
    is_departing = lead_departing or vision_departing or driver_override

    # 3. Dual-Regime Fusion
    # Braking: Experimental mode gets 100% authority whenever it wants to decelerate
    # more than Chill MPC (a_exp < a_chill). Openpilot vision models rarely plan a
    # hard v=0 in the trajectory until actual standstill (they show a rolling dip to
    # ~2 m/s then resume), so gating braking behind trajectory thresholds released the
    # brakes at ~5 mph mid-stop. No trajectory gating, no filtering, no dilution here.
    a_brake_fused = min(a_chill, a_exp)
    # Throttle: Chill MPC controls cruise acceleration, with optional exp bias.
    a_throttle_fused = a_chill + max(0.0, a_exp - a_chill) * self.HYBRID_EXP_BIAS

    # 4. Output Arbitration
    # If the model is commanding real deceleration (a_exp < 0, stronger than Chill),
    # grant it full braking immediately and hold it all the way to standstill.
    is_braking = not is_departing and (a_exp < a_chill and a_exp < 0.0)
    self.last_exp_dominant = bool(is_braking or should_stop_exp)

    # w_vision is a diagnostic mirror of braking dominance (used by replay tooling).
    if is_departing:
      self.w_vision = 0.0
    elif self.last_exp_dominant:
      self.w_vision = 1.0
    else:
      self.w_vision = max(0.0, self.w_vision - 0.05)

    if self.last_exp_dominant:
      a_out = a_brake_fused
    else:
      a_out = a_throttle_fused

    # 5. Authoritative Standstill Handshake
    # Bring to full mechanical stop hold when stopping is requested, or when ego is
    # near zero speed while still intending to brake.
    standstill_intent = not is_departing and (
      should_stop_exp or
      (v_ego < 0.6 and (v_horizon < 0.8 or a_exp < -0.1))
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
        "is_braking": is_braking,
        "a_brake_fused": a_brake_fused,
        "a_throttle_fused": a_throttle_fused,
        "exp_dominant": self.last_exp_dominant,
        "standstill_intent": standstill_intent,
        "should_stop_fused": should_stop_fused,
        "a_out": a_out,
        "regime": "brake" if self.last_exp_dominant else "throttle",
        "standstill": standstill_intent,
      }

    self.prev_a_target = a_out
    return a_out, should_stop_fused