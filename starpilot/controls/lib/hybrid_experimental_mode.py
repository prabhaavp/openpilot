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

    # Last-frame diagnostics surfaced to live logs
    self.last_w_vision = 0.0
    self.last_regime = "throttle"
    self.last_standstill = False
    self.last_exp_dominant = False

    # User tuning
    self.HYBRID_EXP_BIAS = 0.2            # [-1.0, 1.0]
    self.VISION_BRAKE_SENSITIVITY = 1.2   # [0.0, 2.0]

    # Active profile parameters
    self.t_follow = self.BASE_T_FOLLOW
    self.jerk_factor = 1.0
    self._update_profile_limits(self.t_follow, self.jerk_factor)

  def reset(self, a_ego: float = 0.0):
    """Seed target with actual vehicle acceleration on engagement to prevent torque bumps."""
    self.prev_a_target = float(a_ego) if np.isfinite(a_ego) else 0.0
    self.exp_authority = 0.5
    self.last_w_vision = 0.0
    self.last_regime = "throttle"
    self.last_standstill = False
    self.last_exp_dominant = False

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
    traj_v = np.asarray(traj_v, dtype=float)
    if not np.all(np.isfinite(traj_v)):
      return np.array([v_ego], dtype=float)
    return traj_v

  @staticmethod
  def _get_model_trajectory_x(model_v2) -> np.ndarray:
    position = getattr(model_v2, "position", None)
    traj_x = getattr(position, "x", None) if position is not None else None
    if traj_x is None or len(traj_x) == 0:
      return np.array([], dtype=float)
    traj_x = np.asarray(traj_x, dtype=float)
    if not np.all(np.isfinite(traj_x)):
      return np.array([], dtype=float)
    return traj_x

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
    v_horizon = float(traj_v[-1]) if len(traj_v) > 0 else v_ego
    v_ref = max(v_ego, 2.0)

    # Detect deceleration profile or low-speed stop line target
    speed_drop_ratio = max(0.0, (v_ego - v_min) / v_ref)
    stop_target_active = sigmoid(1.2 - v_horizon, k=4.0, x0=0.0)
    model_decel_strength = max(0.0, -a_exp / 2.0)

    raw_vision_metric = max(speed_drop_ratio, stop_target_active, model_decel_strength)
    w_vision = float(np.clip(raw_vision_metric * self.VISION_BRAKE_SENSITIVITY, 0.0, 1.0))

    # Kinematic stopping calculation when approaching a stop line
    if v_min < 1.2 and v_horizon < 2.0 and v_ego > 0.1 and len(traj_x) > min_idx and traj_x[min_idx] > 0.2:
      d_min = float(traj_x[min_idx])
      d_stop_effective = max(d_min - 1.5, 2.0)
      a_kinematic_stop = float(np.clip(- (v_ego ** 2) / (2.0 * d_stop_effective), -3.5, 0.0))
      if d_stop_effective < 6.0 and v_ego < 3.0:
        a_kinematic_stop = min(a_kinematic_stop, -0.6)
      a_exp_effective = min(a_exp, a_kinematic_stop)
    elif v_horizon < 1.0 and v_ego > 0.05:
      # Final roll-in: enforce negative acceleration to complete stop
      a_exp_effective = min(a_exp, -0.5)
    else:
      a_exp_effective = a_exp

    base_auth = float(np.clip(0.5 + (0.35 * self.HYBRID_EXP_BIAS), 0.0, 1.0))
    alpha_exp = lerp(base_auth, 1.0, w_vision)
    self.exp_authority = alpha_exp

    # 2. ACCELERATION FUSION (Throttle vs Braking Regimes)
    # Throttle Regime: Snappy pickup on open roads with clean cruise setpoint clamp
    a_throttle_raw = smooth_max(a_chill, a_exp, k=4.0)
    if v_ego >= v_cruise:
      a_throttle_optimal = min(a_throttle_raw, a_chill)
    else:
      overshoot_risk = sigmoid(v_ego - v_cruise, k=4.0, x0=-1.0)
      a_throttle_capped = min(a_throttle_raw, max(0.0, a_chill))
      a_throttle_optimal = lerp(a_throttle_raw, a_throttle_capped, overshoot_risk)

    a_throttle_conservative = smooth_min(a_chill, a_exp, k=4.0)
    a_throttle_fused = lerp(a_throttle_optimal, a_throttle_conservative, w_vision)

    # Braking Regime: Never dilute Exp stop braking with Chill's 0.0 m/s^2
    if a_exp_effective < 0.0:
      a_chill_brake = min(a_chill, 0.0)
      a_brake_fused = min(a_exp_effective, a_chill_brake) if a_chill_brake < a_exp_effective \
        else lerp(a_exp_effective, a_chill_brake, 1.0 - alpha_exp)
    else:
      a_brake_fused = min(a_chill, a_exp_effective)

    # Regime Selection: If vision sees a stop or braking is requested, lock out positive throttle
    is_braking_phase = (w_vision > 0.3) or (a_exp_effective < -0.2) or (a_chill < -0.2)
    if is_braking_phase:
      w_accel = 0.0
    else:
      phase_metric = smooth_min(a_chill, a_exp, k=4.0)
      w_accel = sigmoid(phase_metric, k=3.0, x0=-0.1) * (1.0 - w_vision)

    a_fused = lerp(a_brake_fused, a_throttle_fused, w_accel)

    # 3. STANDSTILL ANCHOR (Hold at 0 mph, release cleanly on green departure or gas tap)
    is_stopped = sigmoid(0.4 - v_ego, k=8.0, x0=0.0)
    is_staying_stopped = sigmoid(0.5 - v_horizon, k=6.0, x0=0.0)
    lead_departing = lead_status and (getattr(lead_one, "vLead", 0.0) > 0.5)
    vision_departing = (v_horizon > 0.5) and (a_exp > 0.1)
    driver_departing = (a_chill > 0.4) and (not lead_status or lead_d_rel > 10.0)
    # A real trajectory still predicting a stop line (red light / stop sign) keeps
    # the anchor engaged: cruise creep must not release the brake at a stop it can't see.
    model_stop_predicted = len(traj_v) > 1 and v_horizon < 0.5
    departing = (lead_departing or vision_departing or driver_departing) and not model_stop_predicted
    standstill_weight = (0.0 if departing else 1.0) * is_stopped * is_staying_stopped
    a_anchored = lerp(a_fused, smooth_min(a_fused, -0.5, k=6.0), standstill_weight)

    # 4. SAFETY BARRIER (Lead Vehicle Proximity Check)
    d_static_effective = self.D_STATIC_SAFE + max(0.0, 1.5 * (1.0 - (v_ego / 4.0)))
    d_safe = (v_ego * self.T_FOLLOW_SAFE) + d_static_effective

    distance_ratio = (lead_d_rel - d_static_effective) / max(d_safe - d_static_effective, 1.0)
    lead_safety_risk = sigmoid(1.0 - distance_ratio, k=5.0, x0=0.0) * float(lead_status)

    # Enforce hard ceiling when lead is within safety envelope or Chill is braking for lead
    lead_safety_active = lead_status and (lead_d_rel < d_safe or a_chill < 0.0)
    if lead_safety_active and a_chill < a_anchored:
      a_safe = min(a_anchored, a_chill)
    else:
      a_safe = a_anchored

    # 5. ASYMMETRIC DIRECTIONAL SLEW FILTER (Limit Jerk)
    da = a_safe - self.prev_a_target
    if da >= 0.0:
      jerk_limit = self.MAX_JERK_ACCEL
    else:
      # Bypass comfort brake rate for lead collision or committed vision-stop emergencies.
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
    # Border hint: True when the fused output tracks the E2E/vision input more
    # closely than chill ACC. Setting-independent, unlike raw exp_authority which
    # includes the E2E Authority Bias baseline.
    out = self.prev_a_target
    self.last_exp_dominant = abs(out - a_exp) < abs(out - a_chill) - 0.03
    return self.prev_a_target
