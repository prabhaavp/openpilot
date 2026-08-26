#!/usr/bin/env python3
import numpy as np
import pytest

from openpilot.starpilot.controls.lib.hybrid_experimental_mode import (
  HybridExperimentalMode,
  lerp,
  sigmoid,
  smooth_max,
  smooth_min,
)


class FakeLead:
  def __init__(self, status=False, d_rel=150.0, v_lead=0.0):
    self.status = status
    self.dRel = float(d_rel)
    self.vLead = float(v_lead)


class FakeModel:
  def __init__(self, velocity=None, position=None):
    self.velocity = type("Velocity", (), {"x": list(velocity)})() if velocity is not None else None
    self.position = type("Position", (), {"x": list(position)})() if position is not None else None


def make_controller(prev=0.0):
  controller = HybridExperimentalMode()
  controller.reset(prev)
  return controller


def run(controller, *, v_ego=20.0, v_cruise=30.0, lead=None, model=None, a_chill=0.5, a_exp=0.8, frames=80):
  lead = lead if lead is not None else FakeLead()
  if model is None:
    model = FakeModel(velocity=[v_ego] * 33, position=list(np.linspace(0.0, 100.0, 33)))
  result = 0.0
  for _ in range(frames):
    result = controller.update(v_ego, v_cruise, lead, model, a_chill, a_exp)
  return result

def test_soft_operators_are_continuous_and_bounded():
  for value in (-5.0, -0.1, 0.0, 0.1, 5.0):
    assert 0.0 < sigmoid(value) < 1.0
  assert smooth_max(1.0, 2.0) == pytest.approx(2.0, abs=1e-2)
  assert smooth_min(1.0, 2.0) == pytest.approx(1.0, abs=1e-2)
  assert lerp(10.0, 20.0, 0.5) == pytest.approx(15.0, abs=1e-3)

def test_throttle_fusion_leans_toward_snappier_target():
  controller = make_controller()
  a = run(controller, a_chill=0.5, a_exp=0.8)
  assert 0.70 < a < 0.85


def test_throttle_fusion_conservative_when_chill_is_more_eager():
  controller = make_controller()
  a = run(controller, a_chill=1.2, a_exp=0.6)
  assert a > 1.0


def test_throttle_fusion_smooth_cruise_speed_capping():
  controller = make_controller()
  a = run(controller, v_ego=30.0, v_cruise=30.0, a_chill=-0.2, a_exp=1.0)
  assert a <= 0.0, f"Must not accelerate past set cruise speed, got {a}"

def test_green_light_departure_from_standstill():
  controller = make_controller(prev=-0.5)
  traj_v = np.linspace(0.0, 12.0, 33)
  traj_x = np.linspace(0.0, 45.0, 33)
  model = FakeModel(velocity=traj_v, position=traj_x)

  a = run(controller, v_ego=0.0, v_cruise=20.0, model=model, a_chill=1.0, a_exp=1.5, frames=40)
  assert a > 1.0, f"Vehicle should depart promptly on green light, got {a}"


def test_standstill_hold_at_red_light_without_lead():
  controller = make_controller(prev=0.0)
  traj_v = np.zeros(33)
  traj_x = np.zeros(33)
  model = FakeModel(velocity=traj_v, position=traj_x)

  a = run(controller, v_ego=0.0, v_cruise=25.0, lead=FakeLead(status=False),
          model=model, a_chill=0.2, a_exp=-1.0, frames=40)
  assert a <= -0.4, f"Standstill brake must hold even when Chill cruise wants to go, got {a}"

def test_red_light_high_speed_approach_braking():
  controller = make_controller(prev=0.0)
  traj_v = np.linspace(25.0, 0.0, 33)
  traj_x = np.linspace(0.0, 60.0, 33)
  model = FakeModel(velocity=traj_v, position=traj_x)

  a = run(controller, v_ego=25.0, v_cruise=25.0, model=model, a_chill=0.5, a_exp=-2.0, frames=60)
  assert a <= -2.0, f"Stopping deceleration should be fully honored without dilution, got {a}"


def test_red_light_low_speed_roll_prevent_dilution():
  controller = make_controller(prev=-0.5)
  traj_v = np.linspace(0.8, 0.0, 33)
  traj_x = np.linspace(0.0, 3.0, 33)
  model = FakeModel(velocity=traj_v, position=traj_x)

  a = run(controller, v_ego=0.8, v_cruise=20.0, model=model, a_chill=0.8, a_exp=-0.6, frames=30)
  assert a <= -0.55, f"Kinematic stop clamp must enforce stopping bite at low speeds, got {a}"


def test_kinematic_stopping_does_not_blow_up_on_close_dmin():
  controller = make_controller(prev=0.0)
  traj_v = np.zeros(33)
  traj_x = np.ones(33) * 1.0
  model = FakeModel(velocity=traj_v, position=traj_x)

  a = controller.update(15.0, 20.0, FakeLead(), model, a_chill=0.0, a_exp=-1.0)
  assert a >= -(controller.MAX_JERK_BRAKE * controller.DT + 1e-4)


def test_kinematic_stopping_graceful_on_missing_position():
  controller = make_controller(prev=0.0)
  traj_v = np.linspace(10.0, 0.0, 33)
  model = FakeModel(velocity=traj_v, position=[])

  a = run(controller, v_ego=10.0, v_cruise=20.0, model=model, a_chill=0.5, a_exp=-2.0, frames=40)
  assert a <= -1.8

def test_radar_lead_safety_barrier_overrides_vision_throttle():
  controller = make_controller()
  lead = FakeLead(status=True, d_rel=5.0, v_lead=0.0)
  model = FakeModel(velocity=[15.0] * 33, position=list(np.linspace(0.0, 100.0, 33)))

  a = run(controller, v_ego=15.0, v_cruise=20.0, lead=lead, model=model, a_chill=-2.5, a_exp=1.0, frames=40)
  assert a == pytest.approx(-2.5, abs=0.1), f"Safety barrier must enforce radar braking floor, got {a}"


def test_radar_lead_safety_barrier_inactive_when_gap_is_safe():
  controller = make_controller()
  lead = FakeLead(status=True, d_rel=80.0, v_lead=25.0)
  model = FakeModel(velocity=[25.0] * 33, position=list(np.linspace(0.0, 150.0, 33)))

  a = run(controller, v_ego=25.0, v_cruise=30.0, lead=lead, model=model, a_chill=0.6, a_exp=0.9, frames=40)
  assert a > 0.75, f"Lead barrier should remain transparent when headway is safe, got {a}"


def test_cut_in_emergency_braking_ramp_rate():
  controller = make_controller(prev=0.0)
  cut_in_lead = FakeLead(status=True, d_rel=6.0, v_lead=10.0)
  model = FakeModel(velocity=[20.0] * 33)

  for _ in range(8):
    a = controller.update(20.0, 25.0, cut_in_lead, model, a_chill=-3.2, a_exp=0.0)
  assert a <= -2.5, f"Emergency cut-in should ramp braking power quickly, got {a}"

def test_asymmetric_jerk_slew_rates():
  controller_accel = make_controller(prev=0.0)
  a_up = controller_accel.update(20.0, 30.0, FakeLead(), FakeModel(velocity=[20.0] * 33), 4.0, 4.0)
  assert a_up == pytest.approx(controller_accel.MAX_JERK_ACCEL * controller_accel.DT, abs=1e-3)

  controller_brake = make_controller(prev=0.0)
  a_down = controller_brake.update(20.0, 30.0, FakeLead(), FakeModel(velocity=[20.0] * 33), -1.0, -1.0)
  assert a_down == pytest.approx(-controller_brake.MAX_JERK_BRAKE * controller_brake.DT, abs=1e-3)


def test_directional_brake_release_rate():
  controller = make_controller(prev=-3.0)
  a_rel = controller.update(20.0, 30.0, FakeLead(), FakeModel(velocity=[20.0] * 33), 0.0, 0.0)
  expected = -3.0 + controller.MAX_JERK_ACCEL * controller.DT
  assert a_rel == pytest.approx(expected, abs=1e-3)


def test_profile_limits_dynamic_update():
  controller = make_controller()
  controller.update(20.0, 30.0, FakeLead(), FakeModel(velocity=[20.0] * 33), 0.5, 0.5, t_follow=1.1, jerk_factor=1.3)
  assert controller.t_follow == pytest.approx(1.1, abs=1e-4)
  assert controller.jerk_factor == pytest.approx(1.3, abs=1e-4)
  assert controller.MAX_JERK_ACCEL == pytest.approx(5.5 * 1.3, abs=1e-3)

def test_tuning_clamping():
  controller = make_controller()
  controller.set_tuning(5.0, 9.0)
  assert controller.HYBRID_EXP_BIAS == 1.0
  assert controller.VISION_BRAKE_SENSITIVITY == 2.0
  controller.set_tuning(-5.0, -1.0)
  assert controller.HYBRID_EXP_BIAS == -1.0
  assert controller.VISION_BRAKE_SENSITIVITY == 0.0


def test_reset_seeds_active_acceleration():
  controller = make_controller(prev=0.0)
  controller.reset(a_ego=-1.8)
  assert controller.prev_a_target == -1.8


def test_missing_or_corrupt_model_v2_fallbacks():
  controller = make_controller()
  model_none = FakeModel(velocity=None, position=None)
  a = run(controller, lead=FakeLead(status=False), model=model_none, a_chill=0.5, a_exp=0.2)
  assert a > 0.35, "Controller should gracefully fallback when trajectory is None"

  model_empty = FakeModel(velocity=[], position=[])
  a = run(controller, lead=FakeLead(status=False), model=model_empty, a_chill=0.6, a_exp=0.7)
  assert a > 0.55, "Controller should gracefully handle empty trajectory lists"


def test_nan_a_exp_falls_back_to_chill_channel():
  controller = make_controller(prev=0.0)
  model = FakeModel(velocity=[20.0] * 33, position=list(np.linspace(0.0, 100.0, 33)))
  a = run(controller, model=model, a_chill=0.5, a_exp=float("nan"), frames=10)
  assert np.isfinite(a), f"NaN a_exp must never propagate, got {a}"
  assert 0.4 <= a <= 0.6, f"NaN a_exp should degrade to the Chill channel, got {a}"


def test_inf_a_exp_does_not_propagate():
  controller = make_controller(prev=0.0)
  model = FakeModel(velocity=[20.0] * 33, position=list(np.linspace(0.0, 100.0, 33)))
  for a_exp in (float("inf"), float("-inf")):
    controller.reset(0.0)
    a = run(controller, model=model, a_chill=0.5, a_exp=a_exp, frames=10)
    assert np.isfinite(a), f"Non-finite a_exp={a_exp} must not propagate, got {a}"


def test_nan_a_chill_falls_back_to_last_target():
  controller = make_controller(prev=0.0)
  model = FakeModel(velocity=[20.0] * 33, position=list(np.linspace(0.0, 100.0, 33)))
  a = run(controller, model=model, a_chill=float("nan"), a_exp=0.8, frames=10)
  assert np.isfinite(a), f"NaN a_chill must never propagate, got {a}"


def test_nan_in_trajectory_does_not_propagate():
  controller = make_controller(prev=0.0)
  v = [20.0] * 20 + [float("nan")] + [20.0] * 12
  model = FakeModel(velocity=v, position=list(np.linspace(0.0, 100.0, 33)))
  a = run(controller, model=model, a_chill=0.5, a_exp=0.8, frames=10)
  assert np.isfinite(a), f"NaN trajectory must not propagate, got {a}"


def test_reset_rejects_non_finite_seed():
  controller = make_controller(prev=-1.5)
  controller.reset(float("nan"))
  assert controller.prev_a_target == 0.0


def test_standstill_holds_without_model_brake_at_predicted_stop():
  controller = make_controller(prev=0.0)
  model = FakeModel(velocity=np.zeros(33), position=np.zeros(33))
  a = run(controller, v_ego=0.0, v_cruise=25.0, lead=FakeLead(status=False),
          model=model, a_chill=0.8, a_exp=0.0, frames=40)
  assert a <= -0.4, f"Anchor must hold at a predicted stop even with cruise creep, got {a}"


def test_green_light_departure_releases_anchor():
  controller = make_controller(prev=-0.5)
  traj_v = np.linspace(0.0, 12.0, 33)
  traj_x = np.linspace(0.0, 45.0, 33)
  model = FakeModel(velocity=traj_v, position=traj_x)
  a = run(controller, v_ego=0.0, v_cruise=20.0, lead=FakeLead(status=False),
          model=model, a_chill=1.0, a_exp=1.5, frames=40)
  assert a > 1.0, f"Green light departure must still release the anchor, got {a}"


def test_vision_stop_uses_emergency_brake_ramp():
  controller = make_controller(prev=0.0)
  model = FakeModel(velocity=list(np.linspace(20.0, 0.2, 33)), position=list(np.linspace(0.0, 40.0, 33)))
  out = [controller.update(20.0, 25.0, FakeLead(status=False), model, 0.0, -3.5) for _ in range(5)]
  assert out[-1] <= -2.5, f"Vision stop should ramp braking fast, got {out}"


def test_last_exp_dominant_true_when_vision_braking():
  controller = make_controller(prev=0.0)
  model = FakeModel(velocity=list(np.linspace(20.0, 0.2, 33)), position=list(np.linspace(0.0, 40.0, 33)))
  run(controller, v_ego=20.0, v_cruise=25.0, lead=FakeLead(status=False),
      model=model, a_chill=0.0, a_exp=-3.0, frames=60)
  assert controller.last_exp_dominant


def test_last_exp_dominant_false_when_chill_brakes_for_lead():
  controller = make_controller(prev=0.0)
  lead = FakeLead(status=True, d_rel=5.0, v_lead=0.0)
  model = FakeModel(velocity=[15.0] * 33, position=list(np.linspace(0.0, 100.0, 33)))
  run(controller, v_ego=15.0, v_cruise=20.0, lead=lead, model=model, a_chill=-2.5, a_exp=1.0, frames=40)
  assert not controller.last_exp_dominant


def test_last_exp_dominant_false_at_neutral_cruise():
  controller = make_controller(prev=0.0)
  run(controller, a_chill=0.0, a_exp=0.05, frames=10)
  assert not controller.last_exp_dominant