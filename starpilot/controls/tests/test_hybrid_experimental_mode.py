import numpy as np
import pytest

from openpilot.starpilot.controls.lib.hybrid_experimental_mode import (
  HybridExperimentalMode,
  sigmoid,
  soft_max,
  soft_min,
)


class FakeLead:
  def __init__(self, status=False, d_rel=150.0, v_lead=0.0):
    self.status = status
    self.dRel = d_rel
    self.vLead = v_lead


class FakeModel:
  def __init__(self, velocity=None):
    self.velocity = type("Velocity", (), {"x": velocity})()


def make_controller(prev=0.0):
  controller = HybridExperimentalMode()
  controller.reset(prev)
  return controller


def run(controller, *, v_ego=20.0, v_cruise=30.0, lead=None, model=None, a_chill=0.5, a_exp=0.8, frames=80):
  lead = lead if lead is not None else FakeLead()
  model = model if model is not None else FakeModel(velocity=[v_ego] * 20)
  result = 0.0
  for _ in range(frames):
    result = controller.update(v_ego, v_cruise, lead, model, a_chill, a_exp)
  return result


def test_soft_operators_are_continuous_and_bounded():
  for value in (-5.0, -0.1, 0.0, 0.1, 5.0):
    assert 0.0 < sigmoid(value) < 1.0
  assert soft_max(1.0, 2.0) == pytest.approx(2.0, abs=1e-2)
  assert soft_min(1.0, 2.0) == pytest.approx(1.0, abs=1e-2)


def test_throttle_fusion_leans_toward_snappier_target():
  controller = make_controller()
  # No lead, no vision stop: the continuous fusion routes toward the more
  # confident throttle (exp 0.8) without a mode switch.
  a = run(controller, a_chill=0.5, a_exp=0.8)
  assert a > 0.75
  assert a < 0.85


def test_vision_stop_horizon_grants_full_braking():
  controller = make_controller()
  model = FakeModel(velocity=np.linspace(20.0, 0.1, 20))
  # The model trajectory decays to a terminal stop, so w_vision -> 1 and the
  # early vision braking curve takes over.
  a = run(controller, lead=FakeLead(status=False), model=model, a_chill=-0.5, a_exp=-2.0)
  assert a <= -1.5


def test_phantom_brake_shield_caps_unconfirmed_vision_braking():
  controller = make_controller()
  # No lead and a flat model horizon: unconfirmed vision braking is clamped to
  # the gentle coast limit instead of passing the raw E2E target through.
  a = run(controller, lead=FakeLead(status=False), a_chill=0.0, a_exp=-2.0)
  assert -0.6 <= a <= -0.5


def test_cbf_safety_floor_prevents_under_braking_near_close_lead():
  controller = make_controller()
  # Vision wants throttle but a close lead means the Control Barrier Function
  # smoothly forces the target toward the Chill safety floor.
  lead = FakeLead(status=True, d_rel=5.0, v_lead=0.0)
  a = run(controller, lead=lead, a_chill=-1.0, a_exp=0.5)
  assert a == pytest.approx(-1.0, abs=1e-2)


def test_cbf_is_inactive_when_lead_is_far():
  controller = make_controller()
  lead = FakeLead(status=True, d_rel=80.0, v_lead=25.0)
  a = run(controller, lead=lead, a_chill=0.5, a_exp=0.8)
  assert a > 0.7


def test_jerk_slew_limits_single_frame_step():
  controller = make_controller(prev=0.0)
  lead = FakeLead(status=False)
  model = FakeModel(velocity=[20.0] * 20)
  a = controller.update(20.0, 30.0, lead, model, 2.0, 2.0)
  assert a == pytest.approx(controller.MAX_JERK * controller.DT, abs=1e-3)


def test_reset_clears_integrator_state():
  controller = make_controller(prev=0.0)
  lead = FakeLead(status=True, d_rel=20.0, v_lead=15.0)
  model = FakeModel(velocity=[20.0] * 20)
  controller.update(20.0, 30.0, lead, model, -1.0, -2.0)
  assert controller.prev_a_target != 0.0
  controller.reset(0.0)
  assert controller.prev_a_target == 0.0


def test_set_tuning_clamps_to_documented_ranges():
  controller = make_controller()
  controller.set_tuning(5.0, 9.0)
  assert controller.HYBRID_EXP_BIAS == 1.0
  assert controller.VISION_BRAKE_SENSITIVITY == 2.0
  controller.set_tuning(-5.0, -1.0)
  assert controller.HYBRID_EXP_BIAS == -1.0
  assert controller.VISION_BRAKE_SENSITIVITY == 0.0
  controller.set_tuning(0.5, 1.5)
  assert controller.HYBRID_EXP_BIAS == 0.5
  assert controller.VISION_BRAKE_SENSITIVITY == 1.5


def test_missing_model_velocity_falls_back_to_ego_speed():
  controller = make_controller()
  model = FakeModel(velocity=None)
  lead = FakeLead(status=False)
  # Falls back to a constant-velocity horizon, so no vision braking is injected.
  a = run(controller, lead=lead, model=model, a_chill=0.5, a_exp=0.2)
  assert a > 0.4
