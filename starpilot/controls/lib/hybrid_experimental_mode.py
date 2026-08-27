#!/usr/bin/env python3
import numpy as np
from openpilot.common.realtime import DT_MDL


def lerp(a: float, b: float, t: float) -> float:
  """Linear interpolation / blend between a and b by weight t (0.0 to 1.0)."""
  return float((1.0 - t) * a + t * b)


def sigmoid(x: float, k: float = 4.0, x0: float = 0.0) -> float:
  """Smooth 0-to-1 activation curve."""
  z = np.clip(-k * (x - x0), -30.0, 30.0)
  return float(1.0 / (1.0 + np.exp(z)))


def smooth_min(a: float, b: float, k: float = 6.0) -> float:
  return lerp(b, a, sigmoid(b - a, k=k))


class HybridExperimentalMode:
  """
  Final Arbitrator between Chill (MPC Cruise/Radar) and Exp (Vision E2E):
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
    self.prev_a_target = float(a_ego) if np.isfinite(a_ego) else 0.0
    self.w_vision = 0.0
    self.last_exp_dominant = False
    self.diag = {}

  def set_tuning(self, exp_bias: float, vision_brake_sensitivity: float):
    self.HYBRID_EXP_BIAS = float(np.clip(exp_bias, -1.0, 1.0))
    self.VISION_BRAKE_SENSITIVITY = float(np.clip(vision_brake_sensitivity, 0.0, 2.0))

  def update(self, v_ego, v_cruise, lead_one, model_v2, a_chill, a_exp,
             should_stop_exp=False, should_stop_chill=False):

    # Robustness: never let non-finite or corrupt inputs propagate into the blend.
    if not np.isfinite(a_chill):
      a_chill = float(self.prev_a_target)
    if not np.isfinite(a_exp):
      a_exp = a_chill

    # 1. Trajectory Analysis & Robust Slicing
    traj_v = getattr(getattr(model_v2, "velocity", None), "x", [])
    traj_v = np.asarray(traj_v, dtype=float)
    has_full_trajectory = traj_v.size >= 24 and np.all(np.isfinite(traj_v))

    if has_full_trajectory:
      v_horizon = float(traj_v[-1])
      v_short = float(traj_v[23])  # ~4.0s lookahead
      v_min = float(np.min(traj_v))
    else:
      v_horizon = float(v_ego)
      v_short = float(v_ego)
      v_min = float(v_ego)

    lead_status = bool(getattr(lead_one, "status", False))
    lead_v = float(getattr(lead_one, "vLead", 0.0))
    lead_d = float(getattr(lead_one, "dRel", 150.0))

    # 2. Vision Departure / Driver Override Detection (Priority Check)
    lead_departing = lead_status and (lead_v > 0.5)
    vision_departing = (v_horizon > 1.2) and (a_exp > 0.1)
    driver_override = (a_chill > 0.8)
    is_departing = lead_departing or vision_departing or driver_override

    # 3. Vision Stop & Decel Detection
    # Do not latch horizon_stopping if the lead is actively pulling away or driver commands takeoff
    if is_departing:
      horizon_stopping = False
    else:
      horizon_stopping = (v_horizon < 0.8) or (has_full_trajectory and v_short < 1.5) or should_stop_exp

    speed_drop_ratio = max(0.0, (v_ego - v_min) / max(v_ego, 2.0))
    model_decel_strength = max(0.0, -a_exp / 2.0)

    if is_departing:
      raw_vision_metric = 0.0
    elif horizon_stopping:
      raw_vision_metric = 1.0
    elif lead_status and lead_d < 40.0:
      # Slower lead closing: blend vision deceleration with MPC follow
      raw_vision_metric = max(speed_drop_ratio, model_decel_strength)
    else:
      # Open road mild vision response
      raw_vision_metric = max(speed_drop_ratio * 0.5, model_decel_strength * 0.5)

    w_target = float(np.clip(raw_vision_metric * self.VISION_BRAKE_SENSITIVITY, 0.0, 1.0))

    # 4. Dynamic Filter (Fast Attack, Smooth Decay)
    if is_departing:
      self.w_vision = 0.0
    elif w_target > self.w_vision:
      self.w_vision = min(1.0, self.w_vision + 0.15)
    else:
      self.w_vision = max(0.0, self.w_vision - 0.04)

    # 5. Dual-Regime blend
    # Braking Regime: Pure vision braking when model demands it
    if a_exp < 0.0:
      a_brake_fused = min(a_chill, a_exp)
    else:
      a_brake_fused = min(a_chill, a_exp)

    # Throttle Regime: Follow Chill MPC cruise with optional Exp bias
    a_throttle_fused = a_chill + max(0.0, a_exp - a_chill) * max(0.0, self.HYBRID_EXP_BIAS)

    # Output Arbitration
    is_stopping_event = (self.w_vision > 0.3) or horizon_stopping
    if is_stopping_event and not is_departing:
      a_out = a_brake_fused
      if horizon_stopping and v_ego < 2.0:
        a_out = min(a_out, -0.6)  # Standstill anchor into full stop
      self.last_exp_dominant = True
    else:
      a_out = lerp(a_throttle_fused, a_brake_fused, self.w_vision)
      self.last_exp_dominant = False

    # 6. Authoritative Standstill Handshake
    standstill_intent = (v_ego < 0.5 and (v_horizon < 0.4 or should_stop_exp)) and not is_departing
    should_stop_fused = bool((should_stop_chill or should_stop_exp or standstill_intent) and not is_departing)

    if self.record_diag:
      self.diag = {
        # inputs
        "v_ego": v_ego, "v_cruise": v_cruise,
        "a_chill": a_chill, "a_exp": a_exp,
        "should_stop_chill": should_stop_chill,
        "should_stop_exp": should_stop_exp,
        "lead_status": lead_status, "lead_d_rel": lead_d, "lead_v_lead": lead_v,
        # vision intent
        "has_full_trajectory": bool(has_full_trajectory),
        "v_horizon": v_horizon, "v_short": v_short, "v_min": v_min,
        "speed_drop_ratio": speed_drop_ratio,
        "model_decel_strength": model_decel_strength,
        "raw_vision_metric": raw_vision_metric, "w_target": w_target,
        "w_vision": self.w_vision,
        # departure / override
        "lead_departing": lead_departing,
        "vision_departing": vision_departing,
        "driver_override": driver_override,
        "is_departing": is_departing,
        "horizon_stopping": horizon_stopping,
        # fusion
        "a_brake_fused": a_brake_fused,
        "a_throttle_fused": a_throttle_fused,
        "is_stopping_event": is_stopping_event,
        "exp_dominant": self.last_exp_dominant,
        # standstill handshake
        "standstill_intent": standstill_intent,
        "should_stop_fused": should_stop_fused,
        "a_out": a_out,
        # regime labels (kept for downstream forensic/analyzer tooling)
        "regime": "brake" if (is_stopping_event and not is_departing) else "throttle",
        "standstill": standstill_intent,
      }

    self.prev_a_target = a_out
    return a_out, should_stop_fused
