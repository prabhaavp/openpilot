#!/usr/bin/env python3
import numpy as np
from openpilot.common.realtime import DT_MDL
from openpilot.selfdrive.controls.lib.longitudinal_mpc_lib.long_mpc import STOP_DISTANCE


def lerp(a: float, b: float, t: float) -> float:
  """Linear interpolation / blend between a and b by weight t (0.0 to 1.0)."""
  return float((1.0 - t) * a + t * b)


def sigmoid(x: float, k: float = 4.0, x0: float = 0.0) -> float:
  """Smooth 0-to-1 activation curve."""
  z = np.clip(-k * (x - x0), -30.0, 30.0)
  return float(1.0 / (1.0 + np.exp(z)))


def smooth_min(a: float, b: float, k: float = 6.0) -> float:
  return lerp(b, a, sigmoid(b - a, k=k))


def smooth_max(a: float, b: float, k: float = 6.0) -> float:
  return lerp(b, a, sigmoid(a - b, k=k))


class HybridExperimentalMode:
  """
  Fuses Chill Mode (radar/lead tracking) and Experimental Mode (vision/stop signs/lights):
  1. Detects vision stopping intent from trajectory drop and horizon endpoints.
  2. Calculates kinematically required stopping deceleration (-v^2 / 2d).
  3. Clamps Chill positive throttle during stop events to prevent brake fighting/dilution.
  4. Holds brake at standstills to prevent creep, but releases immediately on green lights or gas tap.
  5. Falls back safely to Chill if lead vehicle distance is compromised.
  6. Slew-rate limits acceleration to respect vehicle jerk limits, with emergency bypass.
  """

  # Physical actuator jerk limits (m/s^3)
  BASE_MAX_JERK_BRAKE = 3.5
  BASE_MAX_JERK_ACCEL = 5.5
  EMERGENCY_JERK_BRAKE = 14.0

  # Base safety floor parameters
  BASE_T_FOLLOW = 1.45
  BASE_STOP_DISTANCE = STOP_DISTANCE  # 6.0 m

  def __init__(self):
    self.DT = DT_MDL
    self.prev_a_target = 0.0
    self.exp_authority = 0.5
    self.w_vision_filtered = 0.0
    self.tracked_stop_dist = None

    # Last-frame diagnostics surfaced to live logs
    self.last_w_vision = 0.0
    self.last_regime = "throttle"
    self.last_standstill = False
    self.last_exp_dominant = False

    self.diag = {}
    self.record_diag = False

    # User tuning
    self.HYBRID_EXP_BIAS = 0.2            # [-1.0, 1.0]
    self.VISION_BRAKE_SENSITIVITY = 1.2   # [0.0, 2.0]
    self.KINEMATIC_STOP_GAIN = 1.0        # [0.0, 2.0] scales the -v^2/2d stop-line brake floor

    # Active profile parameters
    self.t_follow = self.BASE_T_FOLLOW
    self.jerk_factor = 1.0
    self._update_profile_limits(self.t_follow, self.jerk_factor)

  def reset(self, a_ego: float = 0.0):
    """Seed target with actual vehicle acceleration on engagement to prevent torque bumps."""
    self.prev_a_target = float(a_ego) if np.isfinite(a_ego) else 0.0
    self.exp_authority = 0.5
    self.w_vision_filtered = 0.0
    self.tracked_stop_dist = None
    self.last_w_vision = 0.0
    self.last_regime = "throttle"
    self.last_standstill = False
    self.last_exp_dominant = False
    self.diag = {}

  def set_tuning(self, exp_bias: float, vision_brake_sensitivity: float, kinematic_stop_gain: float = 1.0,
                 t_follow=None, jerk_factor=None):
    self.HYBRID_EXP_BIAS = float(np.clip(exp_bias, -1.0, 1.0))
    self.VISION_BRAKE_SENSITIVITY = float(np.clip(vision_brake_sensitivity, 0.0, 2.0))
    self.KINEMATIC_STOP_GAIN = float(np.clip(kinematic_stop_gain, 0.0, 2.0))
    if t_follow is not None or jerk_factor is not None:
      self._update_profile_limits(
        t_follow if t_follow is not None else self.t_follow,
        jerk_factor if jerk_factor is not None else self.jerk_factor
      )

  def _update_profile_limits(self, t_follow=None, jerk_factor=None):
    """Updates safety headway floor and jerk limits based on active driving profile."""
    self.t_follow = float(t_follow) if t_follow is not None else self.BASE_T_FOLLOW
    self.jerk_factor = float(np.clip(jerk_factor if jerk_factor is not None else 1.0, 0.25, 2.0))

    # Safety buffer floors
    self.T_FOLLOW_SAFE = float(np.clip(self.t_follow * 0.80, 1.00, 1.60))
    self.D_STATIC_SAFE = float(np.clip(self.BASE_STOP_DISTANCE * 0.75, 3.5, 6.0))

    # Jerk rate limits
    self.MAX_JERK_ACCEL = float(np.clip(self.BASE_MAX_JERK_ACCEL * self.jerk_factor, 2.5, 8.0))
    self.MAX_JERK_BRAKE = float(np.clip(self.BASE_MAX_JERK_BRAKE * self.jerk_factor, 1.8, 6.0))

  @staticmethod
  def _get_traj_array(model_v2, attr: str) -> np.ndarray:
    obj = getattr(model_v2, attr, None)
    arr = getattr(obj, "x", None) if obj is not None else None
    if arr is None or len(arr) == 0:
      return np.array([], dtype=float)
    arr = np.asarray(arr, dtype=float)
    return arr if np.all(np.isfinite(arr)) else np.array([], dtype=float)

  def _get_model_trajectory_v(self, model_v2, v_ego: float) -> np.ndarray:
    traj_v = self._get_traj_array(model_v2, "velocity")
    return traj_v if len(traj_v) > 0 else np.array([v_ego], dtype=float)

  def _get_model_trajectory_x(self, model_v2) -> np.ndarray:
    return self._get_traj_array(model_v2, "position")

  def update(self, v_ego, v_cruise, lead_one, model_v2, a_chill, a_exp,
             t_follow=None, jerk_factor=None):
    # 0. Sync profile parameters if passed per-frame
    if (t_follow is not None and abs(t_follow - self.t_follow) > 1e-4) or \
       (jerk_factor is not None and abs(jerk_factor - self.jerk_factor) > 1e-4):
      self._update_profile_limits(t_follow, jerk_factor)

    if not np.isfinite(a_chill):
      a_chill = float(self.prev_a_target)
    if not np.isfinite(a_exp):
      a_exp = a_chill

    lead_status = bool(getattr(lead_one, "status", False))
    lead_d_rel = float(getattr(lead_one, "dRel", 150.0))

    # 1. VISION INTENT & STOP HORIZON DETECTION
    traj_v = self._get_model_trajectory_v(model_v2, v_ego)
    traj_x = self._get_model_trajectory_x(model_v2)

    v_min = float(np.min(traj_v))
    min_idx = int(np.argmin(traj_v))
    v_ref = max(v_ego, 2.0)

    # Use actual/full model horizon endpoint (index -1) for standstill verification
    v_horizon = float(traj_v[-1]) if len(traj_v) > 0 else v_ego

    # Detect deceleration profile or low-speed stop line target
    speed_drop_ratio = max(0.0, (v_ego - v_min) / v_ref)
    model_decel_strength = max(0.0, -a_exp / 2.0)

    # Fix 3 (Refined): Use a shorter planning horizon (~4 seconds out, index 23) for stop sign detection.
    # Sigmoids adjusted to engage earlier at higher approach speeds, preventing the vehicle from eating up stop distance.
    v_horizon_short = float(traj_v[min(len(traj_v) - 1, 23)]) if len(traj_v) > 0 else v_ego
    stop_target_active = sigmoid(1.8 - v_horizon_short, k=3.0)

    raw_vision_metric = max(speed_drop_ratio, stop_target_active, model_decel_strength)
    w_vision_raw = float(np.clip(raw_vision_metric * self.VISION_BRAKE_SENSITIVITY, 0.0, 1.0))

    # Identify departure signals
    lead_departing = lead_status and (getattr(lead_one, "vLead", 0.0) > 0.5)

    # Core Bug Fix: Suppress false driver_departing latch resets if we are actively stopping,
    # unless a true heavy driver override (>1.2) occurs.
    is_actively_stopping = self.w_vision_filtered > 0.25 and v_ego > 0.5
    driver_override = (a_chill > 1.2)

    driver_departing = (a_chill > 0.4) and (not lead_status or lead_d_rel > 10.0) and (not is_actively_stopping or driver_override)
    model_stop_predicted = len(traj_v) > 1 and v_horizon < 0.5
    vision_departing = (v_horizon > 0.5) and (a_exp > 0.1)
    departing = (lead_departing or vision_departing or driver_departing) and not model_stop_predicted

    # Latch holds during approach, decay only on verified departure or drivers override
    should_reset_latch = driver_departing or lead_departing or (vision_departing and v_ego < 0.15 and not model_stop_predicted)

    if should_reset_latch:
      self.w_vision_filtered = 0.0
    elif departing:
      # Gently decay the latch if the vision model indicates a departure while still rolling
      self.w_vision_filtered *= 0.90
    elif w_vision_raw > 0.15:
      self.w_vision_filtered = max(self.w_vision_filtered, w_vision_raw)   # hold, no decay
    else:
      self.w_vision_filtered *= 0.97

    w_vision = self.w_vision_filtered

    # Kinematic stopping calculation
    d_min = float(traj_x[min_idx]) if len(traj_x) > min_idx else float("inf")

    slow_horizon = sigmoid(4.0 - v_horizon_short, k=1.5)
    stop_confidence = max(stop_target_active, slow_horizon * speed_drop_ratio)

    # Bug A & B Fix: check if model plans a stop/slowdown anywhere in near-to-mid distance
    near_stop_planned = False
    if len(traj_x) == len(traj_v) and len(traj_v) > 0:
      near_stop_planned = np.any((traj_v < 4.0) & (traj_x < 35.0))
    elif len(traj_v) > 0:
      near_stop_planned = np.any(traj_v[:12] < 4.0)

    if near_stop_planned:
      stop_confidence = max(stop_confidence, 0.8)

    # Stable odometry-based stop line tracking to bypass distance latency:
    # Trigger tracking when filtered w_vision is high and model thinks there's a stop line.
    if w_vision > 0.25 and d_min < 100.0:
      if self.tracked_stop_dist is None:
        self.tracked_stop_dist = d_min  # Initialize once on stop latching
      else:
        self.tracked_stop_dist -= v_ego * self.DT  # Track strictly using physical odometry
      d_stop_calc = self.tracked_stop_dist
    else:
      self.tracked_stop_dist = None
      d_stop_calc = d_min

    d_stop_effective = max(d_stop_calc - 1.5, 2.0)
    a_kinematic_stop = 0.0

    # Kinematic deceleration is calculated from the stable odometer-tracked distance
    if v_ego > 0.1 and 0.2 < d_stop_calc < float("inf") and w_vision > 0.25:
      a_kinematic_stop = float(np.clip(- (v_ego ** 2) / (2.0 * d_stop_effective), -3.5, 0.0))
      a_kinematic_stop *= self.KINEMATIC_STOP_GAIN
      if d_stop_effective < 6.0 and v_ego < 3.0:
        a_kinematic_stop = min(a_kinematic_stop, -0.6)
      a_exp_effective = min(a_exp, a_kinematic_stop)
    elif v_horizon < 1.0 and v_ego > 0.05:
      a_exp_effective = min(a_exp, -0.5)
    else:
      a_exp_effective = a_exp

    base_auth = float(np.clip(0.5 + (0.35 * self.HYBRID_EXP_BIAS), 0.0, 1.0))
    alpha_exp = lerp(base_auth, 1.0, w_vision)
    self.exp_authority = alpha_exp

    # 2. ACCELERATION FUSION (Throttle vs Braking Regimes)
    a_throttle_raw = smooth_max(a_chill, a_exp, k=4.0)
    overshoot_risk = 0.0
    a_throttle_capped = a_throttle_raw
    if v_ego >= v_cruise:
      a_throttle_optimal = min(a_throttle_raw, a_chill)
    else:
      overshoot_risk = sigmoid(v_ego - v_cruise, k=4.0, x0=-1.0)
      a_throttle_capped = min(a_throttle_raw, max(0.0, a_chill))
      a_throttle_optimal = lerp(a_throttle_raw, a_throttle_capped, overshoot_risk)

    a_throttle_conservative = smooth_min(a_chill, a_exp, k=4.0)
    a_throttle_fused = lerp(a_throttle_optimal, a_throttle_conservative, w_vision)

    # Braking Regime: Never dilute Exp stop braking with Chill's 0.0 m/s^2
    a_chill_brake = 0.0
    if a_exp_effective < 0.0:
      a_chill_brake = min(a_chill, 0.0)
      a_brake_fused = min(a_exp_effective, a_chill_brake)
    else:
      a_brake_fused = min(a_chill, a_exp_effective)

    # Regime Selection: Lock out positive throttle during stopping/braking
    is_braking_phase = (w_vision > 0.3) or (a_exp_effective < -0.2) or (a_chill < -0.2)
    phase_metric = 0.0
    if is_braking_phase:
      w_accel = 0.0
    else:
      phase_metric = smooth_min(a_chill, a_exp, k=4.0)
      w_accel = sigmoid(phase_metric, k=3.0, x0=-0.1) * (1.0 - w_vision)

    a_fused = lerp(a_brake_fused, a_throttle_fused, w_accel)

    # 3. STANDSTILL ANCHOR
    is_stopped = sigmoid(0.4 - v_ego, k=8.0)
    is_staying_stopped = sigmoid(0.5 - v_horizon, k=6.0)

    # Bug C Fix: Low-speed acceleration lockout (active up to 5.0 m/s when vision stop intent is latched)
    if self.w_vision_filtered > 0.25 and v_ego < 5.0:
      a_fused = min(a_fused, 0.0)

    # Smooth soft lockout ramp: scale positive acceleration to zero based on filter intensity
    if v_ego < 4.0 and a_fused > 0.0:
      scale = max(0.0, 1.0 - (self.w_vision_filtered / 0.30))
      a_fused *= scale

    standstill_weight = (0.0 if departing else 1.0) * is_stopped * is_staying_stopped
    a_anchored = lerp(a_fused, smooth_min(a_fused, -0.5, k=6.0), standstill_weight)

    # 4. SAFETY BARRIER (Lead Vehicle Proximity Check)
    d_static_effective = self.D_STATIC_SAFE + max(0.0, 1.5 * (1.0 - (v_ego / 4.0)))
    d_safe = (v_ego * self.T_FOLLOW_SAFE) + d_static_effective

    distance_ratio = (lead_d_rel - d_static_effective) / max(d_safe - d_static_effective, 1.0)
    lead_safety_risk = sigmoid(1.0 - distance_ratio, k=5.0) * float(lead_status)

    lead_safety_active = lead_status and (lead_d_rel < d_safe or a_chill < 0.0)
    a_safe = min(a_anchored, a_chill) if lead_safety_active else a_anchored

    # 5. ASYMMETRIC DIRECTIONAL SLEW FILTER (Limit Jerk)
    da = a_safe - self.prev_a_target
    if da >= 0.0:
      jerk_limit = self.MAX_JERK_ACCEL
    else:
      is_lead_emergency = (lead_safety_risk > 0.5) and (a_chill < -1.5)
      is_vision_emergency = (w_vision > 0.7) and (a_exp < -2.0)
      jerk_limit = self.EMERGENCY_JERK_BRAKE if (is_lead_emergency or is_vision_emergency) else self.MAX_JERK_BRAKE

    max_delta = jerk_limit * self.DT
    a_out = float(np.clip(a_safe, self.prev_a_target - max_delta, self.prev_a_target + max_delta))
    if np.isfinite(a_out):
      self.prev_a_target = a_out
    self.last_w_vision = w_vision
    self.last_regime = "brake" if is_braking_phase else "throttle"
    self.last_standstill = standstill_weight > 0.0
    self.last_exp_dominant = abs(self.prev_a_target - a_exp) < abs(self.prev_a_target - a_chill) - 0.03

    if self.record_diag:
      self.diag = {
        # inputs
        "v_ego": v_ego, "v_cruise": v_cruise,
        "a_chill": a_chill, "a_exp": a_exp,
        "lead_status": lead_status, "lead_d_rel": lead_d_rel,
        "lead_v_lead": float(getattr(lead_one, "vLead", 0.0)),
        "t_follow": self.t_follow,
        # vision intent
        "v_min": v_min, "v_horizon": v_horizon, "v_horizon_short": v_horizon_short, "v_ref": v_ref,
        "min_idx": min_idx, "speed_drop_ratio": speed_drop_ratio,
        "stop_target_active": stop_target_active,
        "stop_confidence": stop_confidence,
        "model_decel_strength": model_decel_strength,
        "raw_vision_metric": raw_vision_metric, "w_vision": w_vision,
        # kinematic stop
        "d_min": d_min, "d_stop_effective": d_stop_effective,
        "a_kinematic_stop": a_kinematic_stop, "a_exp_effective": a_exp_effective,
        # authority
        "base_auth": base_auth, "alpha_exp": alpha_exp,
        # throttle path
        "a_throttle_raw": a_throttle_raw,
        "a_throttle_optimal": a_throttle_optimal,
        "a_throttle_capped": a_throttle_capped,
        "overshoot_risk": overshoot_risk,
        "a_throttle_conservative": a_throttle_conservative,
        "a_throttle_fused": a_throttle_fused,
        # brake path
        "a_chill_brake": a_chill_brake, "a_brake_fused": a_brake_fused,
        # regime selection
        "is_braking_phase": is_braking_phase,
        "phase_metric": phase_metric, "w_accel": w_accel,
        "a_fused": a_fused,
        # standstill anchor
        "is_stopped": is_stopped, "is_staying_stopped": is_staying_stopped,
        "lead_departing": lead_departing, "vision_departing": vision_departing,
        "driver_departing": driver_departing,
        "model_stop_predicted": model_stop_predicted,
        "departing": departing, "standstill_weight": standstill_weight,
        "a_anchored": a_anchored,
        # safety barrier
        "d_static_effective": d_static_effective, "d_safe": d_safe,
        "distance_ratio": distance_ratio, "lead_safety_risk": lead_safety_risk,
        "lead_safety_active": lead_safety_active, "a_safe": a_safe,
        # slew filter
        "da": da, "jerk_limit": jerk_limit, "max_delta": max_delta,
        "a_out": a_out, "prev_a_target": self.prev_a_target,
        # regime labels
        "regime": self.last_regime,
        "standstill": self.last_standstill,
      }
    return self.prev_a_target