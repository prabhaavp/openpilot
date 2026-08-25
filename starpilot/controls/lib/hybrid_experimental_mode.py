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
  1. Detects vision stopping intent from trajectory v_min and distance horizon.
  2. Calculates kinematically required stopping deceleration (-v^2 / 2d).
  3. Clamps Chill positive throttle during stop events to prevent brake fighting.
  4. Holds 0 m/s at standstills to prevent creep.
  5. Falls back safely to Chill if lead vehicle distance is compromised.
  6. Slew-rate limits acceleration to respect vehicle jerk limits.
  """

  # Base physical actuator jerk limits (m/s^3)
  BASE_MAX_JERK_BRAKE = 3.5
  BASE_MAX_JERK_ACCEL = 5.5

  # Base safety floor parameters
  BASE_T_FOLLOW = 1.45
  BASE_STOP_DISTANCE = STOP_DISTANCE  # 6.0 m

  def __init__(self):
    self.DT = DT_MDL
    self.prev_a_target = 0.0
    self.exp_authority = 0.5

    # User tuning
    self.HYBRID_EXP_BIAS = 0.2            # [-1.0, 1.0]
    self.VISION_BRAKE_SENSITIVITY = 1.2   # [0.0, 2.0]

    # Active profile parameters
    self.t_follow = self.BASE_T_FOLLOW
    self.jerk_factor = 1.0
    self._update_profile_limits(self.t_follow, self.jerk_factor)

  def reset(self, a: float = 0.0):
    self.prev_a_target = float(a)
    self.exp_authority = 0.5

  def set_tuning(self, exp_bias: float, vision_brake_sensitivity: float, t_follow=None, jerk_factor=None):
    self.HYBRID_EXP_BIAS = float(np.clip(exp_bias, -1.0, 1.0))
    self.VISION_BRAKE_SENSITIVITY = float(np.clip(vision_brake_sensitivity, 0.0, 2.0))
    if t_follow is not None or jerk_factor is not None:
      self._update_profile_limits(t_follow, jerk_factor)

  def _update_profile_limits(self, t_follow, jerk_factor=1.0):
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
  def _get_model_trajectory_v(model_v2, v_ego: float) -> np.ndarray:
    velocity = getattr(model_v2, "velocity", None)
    traj_v = getattr(velocity, "x", None) if velocity is not None else None
    if traj_v is None or len(traj_v) == 0:
      return np.array([v_ego], dtype=float)
    return np.asarray(traj_v, dtype=float)

  @staticmethod
  def _get_model_trajectory_x(model_v2) -> np.ndarray:
    position = getattr(model_v2, "position", None)
    traj_x = getattr(position, "x", None) if position is not None else None
    if traj_x is None or len(traj_x) == 0:
      return np.array([], dtype=float)
    return np.asarray(traj_x, dtype=float)

  def update(self, v_ego, v_cruise, lead_one, model_v2, a_chill, a_exp,
             t_follow=None, jerk_factor=None):
    # 0. Sync profile parameters if passed per-frame
    if (t_follow is not None and abs(t_follow - self.t_follow) > 1e-4) or \
       (jerk_factor is not None and abs(jerk_factor - self.jerk_factor) > 1e-4):
      self._update_profile_limits(t_follow, jerk_factor)

    lead_status = bool(getattr(lead_one, "status", False))
    lead_d_rel = float(getattr(lead_one, "dRel", 150.0))

    # 1. VISION INTENT & STOP HORIZON DETECTION
    traj_v = self._get_model_trajectory_v(model_v2, v_ego)
    traj_x = self._get_model_trajectory_x(model_v2)

    min_idx = int(np.argmin(traj_v))
    v_min = float(traj_v[min_idx])
    d_min = float(traj_x[min_idx]) if len(traj_x) > min_idx else 100.0
    v_ref = max(v_ego, 2.0)

    # Detect drop anywhere along trajectory (stop sign / red light profile)
    speed_drop_ratio = max(0.0, (v_ego - v_min) / v_ref)
    is_stopping_profile = sigmoid(1.5 - v_min, k=4.0, x0=0.0) * sigmoid(v_ego, k=3.0, x0=1.0)
    model_decel_strength = max(0.0, -a_exp / 2.5)

    raw_vision_metric = max(speed_drop_ratio, is_stopping_profile, model_decel_strength)
    w_vision = float(np.clip(raw_vision_metric * self.VISION_BRAKE_SENSITIVITY, 0.0, 1.0))

    # Calculate actual kinematic braking required to stop at the detected stop line
    if v_min < 1.2 and v_ego > 1.0 and d_min > 0.5:
      # Kinematic decel: -v^2 / (2 * d) with 2.0m stop line cushion
      d_stop_effective = max(d_min - 2.0, 1.5)
      a_kinematic_stop = - (v_ego ** 2) / (2.0 * d_stop_effective)
      a_exp_effective = min(a_exp, a_kinematic_stop)
    else:
      a_exp_effective = a_exp

    # Dynamic Exp Authority: scales directly to 100% when vision intent is high
    base_auth = float(np.clip(0.5 + (0.35 * self.HYBRID_EXP_BIAS), 0.0, 1.0))
    alpha_exp = lerp(base_auth, 1.0, w_vision)
    self.exp_authority = alpha_exp

    # 2. ACCELERATION FUSION (Throttle vs Braking Regimes)
    # Throttle Regime: Snappy pickup on open roads
    a_throttle_optimal = smooth_max(a_chill, a_exp, k=4.0)
    a_throttle_conservative = smooth_min(a_chill, a_exp, k=4.0)
    a_throttle_fused = lerp(a_throttle_optimal, a_throttle_conservative, w_vision)

    # Braking Regime: CLAMP Chill to <= 0 so cruise throttle cannot fight the stop!
    a_chill_brake = min(a_chill, 0.0)
    a_brake_fused = lerp(a_chill_brake, a_exp_effective, alpha_exp)

    # Regime Selection: If vision sees a stop (w_vision -> 1), FORCE braking regime (w_accel -> 0)
    phase_metric = smooth_min(a_chill, a_exp, k=4.0)
    w_accel = sigmoid(phase_metric, k=3.0, x0=-0.1) * (1.0 - w_vision)
    a_fused = lerp(a_brake_fused, a_throttle_fused, w_accel)

    # 3. STANDSTILL ANCHOR (Prevent creeping at 0 mph)
    is_stopped = sigmoid(0.4 - v_ego, k=8.0, x0=0.0)
    is_min_stopped = sigmoid(0.8 - v_min, k=4.0, x0=0.0)
    standstill_weight = is_stopped * is_min_stopped
    a_anchored = lerp(a_fused, smooth_min(a_fused, 0.0, k=8.0), standstill_weight)

    # 4. SAFETY BARRIER (Lead Vehicle Proximity Check)
    d_static_effective = self.D_STATIC_SAFE + max(0.0, 1.5 * (1.0 - (v_ego / 4.0)))
    d_safe = (v_ego * self.T_FOLLOW_SAFE) + d_static_effective

    distance_ratio = (lead_d_rel - d_static_effective) / max(d_safe - d_static_effective, 1.0)
    lead_safety_risk = sigmoid(1.0 - distance_ratio, k=5.0, x0=0.0) * float(lead_status)

    a_emergency_brake = smooth_min(a_anchored, a_chill, k=6.0)
    a_safe = lerp(a_anchored, a_emergency_brake, lead_safety_risk)

    # 5. ASYMMETRIC SLEW FILTER (Limit Jerk)
    jerk_limit = self.MAX_JERK_ACCEL if a_safe >= self.prev_a_target else self.MAX_JERK_BRAKE
    max_delta = jerk_limit * self.DT

    self.prev_a_target = float(np.clip(a_safe, self.prev_a_target - max_delta, self.prev_a_target + max_delta))
    return self.prev_a_target