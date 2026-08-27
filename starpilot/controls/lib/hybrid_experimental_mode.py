#!/usr/bin/env python3
import math
from openpilot.common.realtime import DT_MDL


def lerp(a: float, b: float, t: float) -> float:
  return (1.0 - t) * a + t * b


def clamp(val: float, low: float, high: float) -> float:
  return max(low, min(high, val))


class HybridExperimentalMode:
  """
  TEMPORARY PASS-THROUGH: Pure Experimental Mode only.
  Directly outputs experimental acceleration and experimental stop signals.
  """

  STANDSTILL_SPEED = 0.6
  STOP_ENGAGE_DECEL = -0.15
  MINOR_DIFF_THRESHOLD = 0.25

  def __init__(self):
    self.DT = DT_MDL
    self.w_vision = 1.0
    self.prev_a_target = 0.0
    self.last_exp_dominant = True
    self.stopping_latched = False
    self.diag = {}
    self.record_diag = False

    self.HYBRID_EXP_BIAS = 0.0
    self.VISION_BRAKE_SENSITIVITY = 1.0

  def reset(self, a_ego: float = 0.0):
    self.prev_a_target = float(a_ego) if math.isfinite(a_ego) else 0.0
    self.w_vision = 1.0
    self.last_exp_dominant = True
    self.stopping_latched = False
    self.diag = {}

  def set_tuning(self, exp_bias: float, vision_brake_sensitivity: float):
    self.HYBRID_EXP_BIAS = clamp(exp_bias, -1.0, 1.0)
    self.VISION_BRAKE_SENSITIVITY = clamp(vision_brake_sensitivity, 0.0, 2.0)

  def update(self, v_ego, v_cruise, lead_one, model_v2, a_chill, a_exp,
             should_stop_exp=False, should_stop_chill=False, gas_pressed=False):

    # Sanitize exp acceleration fallback to previous target if invalid
    a_out = float(a_exp) if math.isfinite(a_exp) else self.prev_a_target
    should_stop_fused = bool(should_stop_exp)

    if self.record_diag:
      self.diag = {
        "v_ego": v_ego,
        "v_cruise": v_cruise,
        "a_chill": a_chill,
        "a_exp": a_exp,
        "should_stop_chill": should_stop_chill,
        "should_stop_exp": should_stop_exp,
        "a_out": a_out,
        "should_stop_fused": should_stop_fused,
        "regime": "pure_exp_passthrough",
      }

    self.prev_a_target = a_out
    return a_out, should_stop_fused