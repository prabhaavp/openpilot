import numpy as np
import pytest

from types import SimpleNamespace

from cereal import custom

from openpilot.common.realtime import DT_MDL
from openpilot.starpilot.common.starpilot_variables import DEFAULT_LATERAL_ACCELERATION
from openpilot.starpilot.common.starpilot_utilities import extract_curve_profile
from openpilot.starpilot.controls.lib.curve_speed_controller import (
  CSC_APPROACH_DECEL,
  CSC_COMFORT_MARGIN,
  CSC_COUNT_CAP,
  CSC_EGO_HEADROOM,
  CSC_FARFIELD_GAIN,
  CSC_LAT_ACCEL_MAX,
  CSC_MAX_LATERAL_ACCEL,
  CSC_MIN_SPEED,
  MAX_CURVATURE,
  PRIOR_CURVATURE_BP,
  PRIOR_LAT_ACCEL_V,
  CSC_BRAKE_LEAD_MAX,
  CSC_NUDGE,
  CSC_NUDGE_WEIGHT,
  CSC_OVERRIDE_WATCH_TIME,
  CSC_SETTLE_TIME,
  CSC_SPEED_COMFORT_BP,
  CSC_SPEED_COMFORT_SCALE,
  CSC_SPEED_TIER_SPLIT,
  CSC_TARGET_UP_RATE,
  CSC_TRAINING_SETTLE_TIME,
  CurveSpeedController,
  weighted_isotonic,
)


class FakeParams:
  def __init__(self, values=None):
    self.values = dict(values or {})

  def get(self, *args, **kwargs):
    key = args[0] if args else None
    return self.values.get(key)

  def put_nonblocking(self, key, value):
    self.values[key] = value


def make_controller(curve_profile=None, curvature_data=None, weather_id=0, reduce_lat=0.0, road_curvature=0.02, driving_in_curve=False):
  if curve_profile is None:
    curve_profile = (np.zeros(33), np.linspace(0.0, 300.0, 33))

  planner = SimpleNamespace(
    params=FakeParams({"CurvatureData": curvature_data} if curvature_data is not None else None),
    curve_profile=curve_profile,
    starpilot_weather=SimpleNamespace(weather_id=weather_id, reduce_lateral_acceleration=reduce_lat),
    road_curvature=road_curvature,
    driving_in_curve=driving_in_curve,
    tracking_lead=False,
    lateral_acceleration=0.0,
  )
  controller = CurveSpeedController(SimpleNamespace(starpilot_planner=planner))
  return planner, controller


def make_sm(*, gas=False, brake=False, long_active=True, blinker=False, accel_pressed=False):
  return {
    "carControl": SimpleNamespace(longActive=long_active),
    "carState": SimpleNamespace(gasPressed=gas, brakePressed=brake, leftBlinker=blinker, rightBlinker=False),
    "starpilotCarState": SimpleNamespace(accelPressed=accel_pressed),
    "onroadEvents": [],
  }


def single_apex_profile(curvature, distance):
  distances = np.linspace(0.0, max(distance * 1.5, 1.0), 33)
  curvatures = np.zeros(33)
  index = int(np.argmin(np.abs(distances - distance)))
  distances[index] = distance
  curvatures[index] = curvature
  return curvatures, distances


def converge(controller, v_ego, v_cruise, frames=600):
  for _ in range(frames):
    controller.update_target(v_ego, v_cruise)
  return controller.target


def envelope_speed(controller, curvature, distance):
  curve_speed = min(
    max(float(np.sqrt(controller.lat_accel_for_curvature(curvature) / curvature)), CSC_MIN_SPEED),
    float(np.sqrt(CSC_MAX_LATERAL_ACCEL / curvature)),
  )
  settle_distance = curve_speed * CSC_SETTLE_TIME
  effective_distance = max(0.0, distance - settle_distance)
  return float(np.sqrt(curve_speed**2 + 2.0 * CSC_APPROACH_DECEL * effective_distance))


def test_straight_road_target_is_cruise_speed():
  _, controller = make_controller()

  controller.update_target(30.0, 30.0)

  assert controller.target == pytest.approx(30.0)


def test_distant_apex_does_not_constrain_until_braking_is_due():

  _, probe = make_controller()
  curve_speed = max(float(np.sqrt(probe.lat_accel_for_curvature(0.02) / 0.02)), CSC_MIN_SPEED)
  beyond_braking = 1.3 * (30.0**2 - curve_speed**2) / (2 * CSC_APPROACH_DECEL)

  _, controller = make_controller(curve_profile=single_apex_profile(0.02, beyond_braking))

  target = converge(controller, 30.0, 30.0)

  assert target == pytest.approx(30.0)


def test_apex_in_braking_range_constrains_to_kinematic_envelope():
  _, controller = make_controller(curve_profile=single_apex_profile(0.02, 150.0))

  target = converge(controller, 30.0, 30.0)

  assert target == pytest.approx(envelope_speed(controller, 0.02, 150.0), abs=0.1)
  assert target < 30.0


def test_exit_recovery_rises_immediately_without_freeze():
  planner, controller = make_controller(curve_profile=single_apex_profile(0.03, 20.0))
  low_target = converge(controller, 15.0, 30.0)
  assert low_target < 20.0

  planner.curve_profile = (np.zeros(33), np.linspace(0.0, 300.0, 33))
  controller.update_target(15.0, 30.0)
  assert controller.target > low_target
  assert controller.target - low_target == pytest.approx(CSC_TARGET_UP_RATE * DT_MDL)


  frames = int((15.0 + CSC_EGO_HEADROOM - controller.target) / (CSC_TARGET_UP_RATE * DT_MDL)) + 1
  for _ in range(frames):
    controller.update_target(15.0, 30.0)
  assert controller.target >= 15.0 + CSC_EGO_HEADROOM

  recovered = converge(controller, 15.0, 30.0)
  assert recovered == pytest.approx(30.0)


def test_upward_jitter_in_the_envelope_is_rate_limited():


  planner, controller = make_controller(curve_profile=single_apex_profile(0.002, 40.0))
  steady = converge(controller, 30.0, 32.0)
  assert steady < 32.0

  flat = (np.zeros(33), np.linspace(0.0, 300.0, 33))
  grazing = planner.curve_profile
  peak = steady
  for i in range(40):
    planner.curve_profile = flat if i % 2 else grazing
    controller.update_target(30.0, 32.0)
    assert controller.target - peak <= CSC_TARGET_UP_RATE * DT_MDL + 1e-6
    peak = controller.target


def test_firm_distant_curvature_is_corrected_for_the_model_under_read():

  distance = 90.0
  _, plain = make_controller(curve_profile=single_apex_profile(0.0045, distance))
  _, probe = make_controller()
  corrected = probe._correct_far_field(*single_apex_profile(0.0045, distance))

  assert corrected.max() == pytest.approx(0.0045 * CSC_FARFIELD_GAIN)
  assert converge(plain, 30.0, 30.0) < envelope_speed(plain, 0.0045, distance) + 1e-6


def test_weak_or_near_readings_are_left_alone():
  _, probe = make_controller()


  weak = probe._correct_far_field(*single_apex_profile(0.002, 90.0))
  assert weak.max() == pytest.approx(0.002)


  near = probe._correct_far_field(*single_apex_profile(0.0045, 10.0))
  assert near.max() == pytest.approx(0.0045)


def test_far_field_correction_brings_the_slowdown_forward():
  profile = single_apex_profile(0.0045, 120.0)
  _, controller = make_controller(curve_profile=profile)

  corrected = converge(controller, 30.0, 30.0)
  raw_curvatures, distances = profile
  uncorrected = float(np.sqrt(
    max(np.sqrt(controller.lat_accel_for_curvature(0.0045) / 0.0045), CSC_MIN_SPEED) ** 2
    + 2.0 * CSC_APPROACH_DECEL * 120.0))

  assert corrected < uncorrected


def test_fresh_activation_seeds_at_envelope_not_cruise():
  _, controller = make_controller(curve_profile=(np.full(33, 0.05), np.linspace(0.0, 60.0, 33)))

  controller.update_target(6.0, 30.0)

  assert controller.target < 15.0


def test_target_never_trails_accelerating_car_when_unconstrained():
  planner, controller = make_controller(curve_profile=single_apex_profile(0.03, 20.0))
  converge(controller, 15.0, 30.0)

  planner.curve_profile = (np.zeros(33), np.linspace(0.0, 300.0, 33))
  v_ego = 15.0
  caught_up = None
  for frame in range(200):
    v_ego = min(v_ego + 2.0 * DT_MDL, 30.0)
    controller.update_target(v_ego, 30.0)

    if controller.target >= v_ego:
      caught_up = caught_up if caught_up is not None else frame
    assert caught_up is None or controller.target >= min(30.0, v_ego) - 1e-6

  assert caught_up is not None and caught_up * DT_MDL < 2.0
  assert controller.target == pytest.approx(30.0)


def test_target_does_not_ratchet_down_with_ego_speed():
  _, controller = make_controller(curve_profile=single_apex_profile(0.02, 150.0))
  target = converge(controller, 30.0, 30.0)
  assert target > CSC_MIN_SPEED

  controller.update_target(14.0, 30.0)

  assert controller.target == pytest.approx(target, abs=0.2)


def test_sharp_curve_target_floors_at_min_speed():
  _, controller = make_controller(curve_profile=(np.full(33, 0.1), np.linspace(0.0, 100.0, 33)))

  target = converge(controller, 15.0, 30.0)

  assert target == pytest.approx(np.sqrt(CSC_MAX_LATERAL_ACCEL / 0.1), abs=0.05)


def test_sharp_curve_target_respects_lateral_acceleration_cap():
  _, controller = make_controller(curve_profile=(np.full(33, 0.1), np.linspace(0.0, 100.0, 33)))

  target = converge(controller, 15.0, 30.0)

  assert target**2 * 0.1 <= CSC_MAX_LATERAL_ACCEL + 0.05


def test_weather_reduces_curve_speed():
  _, dry = make_controller(curve_profile=single_apex_profile(0.01, 0.0))
  _, wet = make_controller(curve_profile=single_apex_profile(0.01, 0.0), weather_id=1, reduce_lat=0.2)

  dry_target = converge(dry, 20.0, 30.0)
  wet_target = converge(wet, 20.0, 30.0)

  assert wet_target < dry_target
  assert wet_target == pytest.approx(dry_target * np.sqrt(0.8), abs=0.1)


def test_prior_gives_higher_lat_accel_for_sharper_curves():
  _, controller = make_controller()

  assert controller.learned_lat_accel(0.001) == pytest.approx(1.5, abs=0.05)
  assert controller.learned_lat_accel(MAX_CURVATURE) > controller.learned_lat_accel(0.001)
  assert controller.learned_lat_accel(MAX_CURVATURE) == pytest.approx(
    float(np.interp(MAX_CURVATURE, PRIOR_CURVATURE_BP, PRIOR_LAT_ACCEL_V)), abs=0.05)
  assert controller.lateral_acceleration == pytest.approx(DEFAULT_LATERAL_ACCELERATION)


def test_comfort_margin_matches_the_learned_habit():

  _, controller = make_controller()

  assert CSC_COMFORT_MARGIN == pytest.approx(1.0)
  assert controller.lat_accel_for_curvature(0.01) == pytest.approx(controller.learned_lat_accel(0.01))


def test_binding_distance_reports_the_constraining_point():
  _, controller = make_controller(curve_profile=single_apex_profile(0.02, 150.0))
  converge(controller, 30.0, 30.0)

  assert controller.binding_distance == pytest.approx(150.0, abs=1.0)


def test_binding_distance_is_zero_when_unconstrained():
  _, controller = make_controller()
  converge(controller, 30.0, 30.0)

  assert controller.binding_distance == 0.0


def test_heavily_sampled_bucket_dominates_prior():
  _, controller = make_controller(curvature_data={"0.05": {"average": 3.0, "count": 100000}})

  assert controller.learned_lat_accel(0.05) == pytest.approx(3.0, abs=0.05)
  assert controller.learned_lat_accel(0.08) >= controller.learned_lat_accel(0.05)


def test_learned_curve_stays_monotonic_despite_low_outlier_bucket():
  _, controller = make_controller(curvature_data={"0.05": {"average": 0.5, "count": 100000}})

  assert controller.learned_lat_accel(0.05) >= controller.learned_lat_accel(0.03)


def test_dense_bucket_is_not_overridden_by_sparse_neighbour():

  _, dense_low = make_controller(curvature_data={
    "0.003": {"average": 1.95, "count": 20},
    "0.005": {"average": 1.38, "count": 80},
  })
  _, dense_high = make_controller(curvature_data={
    "0.003": {"average": 1.95, "count": 80},
    "0.005": {"average": 1.38, "count": 20},
  })

  assert dense_low.learned_lat_accel(0.005) < 1.95
  assert dense_low.learned_lat_accel(0.005) >= dense_low.learned_lat_accel(0.003)

  assert dense_high.learned_lat_accel(0.005) > dense_low.learned_lat_accel(0.005)


def test_weighted_isotonic_pools_violators_by_weight():
  fitted = weighted_isotonic(np.array([1.0, 3.0, 1.2]), np.array([1.0, 1.0, 1000.0]))

  assert np.all(np.diff(fitted) >= -1e-9)
  assert fitted[-1] == pytest.approx(1.2, abs=0.02)


def test_weighted_isotonic_leaves_sorted_input_untouched():
  values = np.array([1.0, 1.5, 2.0, 2.5])
  fitted = weighted_isotonic(values, np.ones(4))

  assert fitted == pytest.approx(values)


def test_legacy_off_grid_curvature_data_merges_into_buckets():
  _, controller = make_controller(curvature_data={
    "0.0203": {"average": 2.5, "count": 10},
    "0.02": {"average": 2.0, "count": 10},
  })

  assert controller.curvature_data["0.02"]["count"] == 20
  assert controller.curvature_data["0.02"]["average"] == pytest.approx(2.25)


def test_non_finite_persisted_curvature_data_is_ignored():
  _, controller = make_controller(curvature_data={
    "0.01": {"average": float("nan"), "count": 10},
    "0.02": {"average": float("inf"), "count": 10},
  })

  assert controller.curvature_data == {}
  assert np.all(np.isfinite(controller._curve_a))


def test_invalid_curve_profile_is_ignored():
  model = SimpleNamespace(
    orientationRate=SimpleNamespace(z=[0.01, 0.02]),
    velocity=SimpleNamespace(x=[10.0]),
    position=SimpleNamespace(x=[20.0, 40.0]),
  )

  curvatures, distances = extract_curve_profile(model)

  assert curvatures.size == 0
  assert distances.size == 0

  curvatures, distances = extract_curve_profile(SimpleNamespace())

  assert curvatures.size == 0
  assert distances.size == 0


def test_training_update_step_is_capped_by_ema_count():
  planner, controller = make_controller(curvature_data={"0.02": {"average": 2.0, "count": 10000}}, driving_in_curve=True)
  planner.lateral_acceleration = 3.0
  controller.training_timer = CSC_TRAINING_SETTLE_TIME

  controller.log_data(10.0, make_sm(long_active=False))

  data = controller.curvature_data["0.02"]
  assert data["count"] == 10001
  assert data["average"] == pytest.approx((2.0 * CSC_COUNT_CAP + 3.0) / (CSC_COUNT_CAP + 1))


def test_no_passive_training_right_after_csc_limited_speed():
  planner, controller = make_controller(curve_profile=single_apex_profile(0.03, 20.0), driving_in_curve=True)
  planner.lateral_acceleration = 3.0
  converge(controller, 15.0, 30.0)
  assert controller.training_quiet_timer > 0.0

  controller.training_timer = CSC_TRAINING_SETTLE_TIME
  controller.log_data(10.0, make_sm(long_active=False))
  assert "0.02" not in controller.curvature_data
  assert not controller.enable_training

  controller.training_quiet_timer = 0.0
  controller.training_timer = CSC_TRAINING_SETTLE_TIME
  controller.log_data(10.0, make_sm(long_active=False))
  assert controller.curvature_data["0.02"]["count"] == 1


def test_training_settles_within_a_couple_of_seconds():


  planner, controller = make_controller(driving_in_curve=True)
  planner.lateral_acceleration = 2.4
  sm = make_sm(long_active=False)

  for _ in range(int(CSC_TRAINING_SETTLE_TIME / DT_MDL) - 2):
    controller.log_data(10.0, sm)
  assert "0.02" not in controller.curvature_data

  for _ in range(3):
    controller.log_data(10.0, sm)
  assert controller.curvature_data["0.02"]["count"] >= 1


def test_brief_ineligibility_does_not_restart_the_settle_timer():
  planner, controller = make_controller(driving_in_curve=True)
  planner.lateral_acceleration = 2.4
  sm = make_sm(long_active=False)
  for _ in range(int(CSC_TRAINING_SETTLE_TIME / DT_MDL) + 1):
    controller.log_data(10.0, sm)
  trained = controller.curvature_data["0.02"]["count"]


  planner.tracking_lead = True
  controller.log_data(10.0, sm)
  controller.log_data(10.0, sm)
  planner.tracking_lead = False

  controller.log_data(10.0, sm)
  assert controller.curvature_data["0.02"]["count"] == trained + 1


def test_sustained_ineligibility_still_drains_the_settle_timer():
  planner, controller = make_controller(driving_in_curve=True)
  planner.lateral_acceleration = 2.4
  engaged = make_sm(long_active=True)
  manual = make_sm(long_active=False)
  for _ in range(int(CSC_TRAINING_SETTLE_TIME / DT_MDL) + 1):
    controller.log_data(10.0, manual)

  for _ in range(int(2 * CSC_TRAINING_SETTLE_TIME / DT_MDL)):
    controller.log_data(10.0, engaged)
  assert controller.training_timer == pytest.approx(0.0)

  controller.log_data(10.0, manual)
  assert not controller.enable_training


def settle_override(controller, sm=None, frames=None):
  """Run the post-override watch out so the pseudo-sample is committed."""
  sm = sm if sm is not None else make_sm()
  for _ in range(frames if frames is not None else int(CSC_OVERRIDE_WATCH_TIME / DT_MDL) + 1):
    controller.handle_override(20.0, False, sm)


def test_gas_override_nudges_bucket_up_once_per_episode():
  _, controller = make_controller()
  prior = controller.learned_lat_accel(0.02)
  controller.target = 10.0

  controller.handle_override(20.0, True, make_sm(gas=True))
  controller.handle_override(20.0, True, make_sm(gas=True))
  assert "0.02" not in controller.curvature_data

  settle_override(controller)
  assert controller.curvature_data["0.02"]["count"] == CSC_NUDGE_WEIGHT
  assert controller.curvature_data["0.02"]["average"] > prior

  controller.handle_override(20.0, False, make_sm())
  controller.target = 10.0
  controller.handle_override(20.0, True, make_sm(gas=True))
  settle_override(controller)
  assert controller.curvature_data["0.02"]["count"] == 2 * CSC_NUDGE_WEIGHT


def test_override_learns_the_cornering_the_driver_actually_held():


  planner, observed = make_controller(driving_in_curve=True)
  observed.target = 10.0
  observed.handle_override(20.0, True, make_sm(gas=True))
  planner.lateral_acceleration = 2.9
  settle_override(observed, make_sm(gas=True))

  _, stepped = make_controller(driving_in_curve=True)
  stepped._apply_nudge(CSC_NUDGE)

  assert observed.curvature_data["0.02"]["average"] == pytest.approx(2.9)
  assert observed.curvature_data["0.02"]["average"] > stepped.curvature_data["0.02"]["average"]
  assert observed.learned_lat_accel(0.02) > stepped.learned_lat_accel(0.02)


def test_override_on_a_straight_still_registers_the_fixed_step():
  planner, controller = make_controller()
  prior = controller.learned_lat_accel(0.02)
  controller.target = 10.0

  controller.handle_override(20.0, True, make_sm(gas=True))
  planner.lateral_acceleration = 0.0
  settle_override(controller)

  assert controller.curvature_data["0.02"]["average"] == pytest.approx(prior + CSC_NUDGE)


def test_res_button_nudges_bucket_up_even_at_target_speed():
  _, controller = make_controller()
  prior = controller.learned_lat_accel(0.02)
  controller.target = 20.0

  controller.handle_override(20.0, True, make_sm(), accel_button=True)
  settle_override(controller)

  assert controller.curvature_data["0.02"]["count"] == CSC_NUDGE_WEIGHT
  assert controller.curvature_data["0.02"]["average"] > prior


def test_brake_override_nudges_bucket_down():
  _, controller = make_controller(driving_in_curve=True)
  prior = controller.learned_lat_accel(0.02)

  controller.handle_override(20.0, True, make_sm(brake=True))

  assert controller.curvature_data["0.02"]["count"] == CSC_NUDGE_WEIGHT
  assert controller.curvature_data["0.02"]["average"] < prior


def test_calibrated_lateral_acceleration_param_is_written_on_flush():
  planner, controller = make_controller(curvature_data={"0.02": {"average": 2.8, "count": 5000}})

  assert "CalibratedLateralAcceleration" not in planner.params.values
  controller.flush_data()

  assert planner.params.values["CalibratedLateralAcceleration"] > DEFAULT_LATERAL_ACCELERATION
  assert controller.lateral_acceleration == planner.params.values["CalibratedLateralAcceleration"]


def test_stale_param_from_a_previous_build_is_republished_without_training():

  planner, controller = make_controller(curvature_data={"0.02": {"average": 2.8, "count": 5000}})
  planner.params.values["CalibratedLateralAcceleration"] = 3.71

  controller.log_data(0.0, make_sm())

  assert planner.params.values["CalibratedLateralAcceleration"] <= CSC_LAT_ACCEL_MAX


def make_map_out(*, map_curve=0.0, suggested=0.0, sl_suggested=0.0, way_sel=None):
  if way_sel is None:
    way_sel = custom.WaySelectionType.current

  return SimpleNamespace(
    mapCurveSpeed=map_curve,
    suggestedSpeed=suggested,
    speedLimitSuggestedSpeed=sl_suggested,
    waySelectionType=way_sel,
  )


def converge_map(controller, planner, map_out, v_ego=30.0, v_cruise=30.0, frames=400):
  planner.mapd_out = map_out
  for _ in range(frames):
    controller.update_target(v_ego, v_cruise)
  return controller.target


def test_map_curve_speed_slowdown_is_used_when_below_the_speed_limit_suggestion():
  planner, controller = make_controller()

  target = converge_map(controller, planner, make_map_out(suggested=20.0, sl_suggested=30.0))

  assert target == pytest.approx(20.0, abs=0.2)


def test_map_speed_limit_suggestion_is_not_treated_as_a_curve():
  planner, controller = make_controller()

  target = converge_map(controller, planner, make_map_out(suggested=30.0, sl_suggested=30.0), frames=200)

  assert target == pytest.approx(30.0)


def test_dedicated_map_curve_field_is_preferred_over_the_composite():
  planner, controller = make_controller()

  target = converge_map(controller, planner, make_map_out(map_curve=18.0, suggested=25.0, sl_suggested=30.0))

  assert target == pytest.approx(18.0, abs=0.2)


def test_map_fail_way_selection_is_ignored():
  planner, controller = make_controller()

  target = converge_map(
    controller, planner, make_map_out(map_curve=10.0, way_sel=custom.WaySelectionType.fail), frames=200)

  assert target == pytest.approx(30.0)


def test_missing_mapd_is_ignored():
  _, controller = make_controller()

  for _ in range(200):
    controller.update_target(30.0, 30.0)

  assert controller.target == pytest.approx(30.0)


def test_high_speed_sweepers_get_less_comfort_than_sharp_bends():
  _, controller = make_controller()

  sweeper = 0.001
  assert controller.learned_lat_accel(sweeper) > controller.lat_accel_for_curvature(sweeper)

  bend = 0.02
  assert controller.lat_accel_for_curvature(bend) == pytest.approx(controller.learned_lat_accel(bend))


def test_speed_comfort_scale_spans_the_breakpoints():
  assert CSC_SPEED_COMFORT_BP == [20.0, 35.0]
  assert CSC_SPEED_COMFORT_SCALE[0] == pytest.approx(1.0)
  assert CSC_SPEED_COMFORT_SCALE[-1] == pytest.approx(0.88)


def test_legacy_curvature_data_migrates_into_the_low_speed_tier():
  _, controller = make_controller(curvature_data={"0.02": {"average": 2.0, "count": 10000}})

  data = controller.curvature_data["0.02"]
  assert data["average"] == pytest.approx(2.0)
  assert data["count"] == 10000
  assert data["average_low"] == pytest.approx(2.0)
  assert data["count_low"] == 10000
  assert data["average_high"] == pytest.approx(2.0)
  assert data["count_high"] == 0


def test_training_splits_into_speed_tiers_and_keeps_base_keys():
  planner, controller = make_controller(driving_in_curve=True)
  planner.road_curvature = 0.02
  controller.training_timer = CSC_TRAINING_SETTLE_TIME

  planner.lateral_acceleration = 1.3
  controller.log_data(25.0, make_sm(long_active=False))
  planner.lateral_acceleration = 2.6
  controller.log_data(10.0, make_sm(long_active=False))

  data = controller.curvature_data["0.02"]
  assert data["count_high"] == 1
  assert data["average_high"] == pytest.approx(1.3)
  assert data["count_low"] == 1
  assert data["average_low"] == pytest.approx(2.6)
  assert data["count"] == 2
  assert data["average"] == pytest.approx(1.95)


def test_tier_split_keeps_highway_and_local_habits_apart():
  planner, controller = make_controller(driving_in_curve=True)
  planner.road_curvature = 0.003
  controller.training_timer = CSC_TRAINING_SETTLE_TIME

  planner.lateral_acceleration = 1.2
  controller.log_data(25.0, make_sm(long_active=False))
  planner.lateral_acceleration = 2.5
  controller.log_data(10.0, make_sm(long_active=False))

  idx = controller.required_curvatures.index(controller._bucket_curvature(0.003))
  assert controller._curve_a_high[idx] < controller._curve_a_low[idx]


def test_high_speed_sweeper_uses_the_high_speed_tier():
  planner, controller = make_controller(driving_in_curve=True)
  planner.road_curvature = 0.003
  controller.training_timer = CSC_TRAINING_SETTLE_TIME

  planner.lateral_acceleration = 1.2
  controller.log_data(25.0, make_sm(long_active=False))
  planner.lateral_acceleration = 2.5
  controller.log_data(10.0, make_sm(long_active=False))

  combined = float(np.interp(0.003, controller._curve_k, controller._curve_a))
  assert np.sqrt(combined / 0.003) >= CSC_SPEED_TIER_SPLIT

  high_lat = float(controller.lat_accel_for_curvature(0.003))
  low_lat = float(np.interp(0.003, controller._curve_k, controller._curve_a_low))
  assert high_lat < low_lat


def test_longer_brake_lead_advances_the_slowdown():
  planner, controller = make_controller(curve_profile=single_apex_profile(0.02, 150.0))
  planner.csc_brake_lead = CSC_SETTLE_TIME
  baseline = converge(controller, 30.0, 30.0)

  planner_earlier, controller_earlier = make_controller(curve_profile=single_apex_profile(0.02, 150.0))
  planner_earlier.csc_brake_lead = CSC_SETTLE_TIME + 1.0
  earlier = converge(controller_earlier, 30.0, 30.0)

  assert earlier < baseline


def test_zero_brake_lead_brakes_up_to_the_apex():
  planner, controller = make_controller(curve_profile=single_apex_profile(0.02, 0.0))
  planner.csc_brake_lead = 0.0

  target = converge(controller, 30.0, 30.0)
  curve_speed = min(
    max(float(np.sqrt(controller.lat_accel_for_curvature(0.02) / 0.02)), CSC_MIN_SPEED),
    float(np.sqrt(CSC_MAX_LATERAL_ACCEL / 0.02)),
  )

  assert target == pytest.approx(curve_speed, abs=0.1)


def test_brake_lead_is_clamped_to_supported_range():
  planner, controller = make_controller(curve_profile=single_apex_profile(0.02, 150.0))
  planner.csc_brake_lead = 99.0
  over = converge(controller, 30.0, 30.0)

  planner_capped, controller_capped = make_controller(curve_profile=single_apex_profile(0.02, 150.0))
  planner_capped.csc_brake_lead = CSC_BRAKE_LEAD_MAX
  capped = converge(controller_capped, 30.0, 30.0)

  assert over == pytest.approx(capped, abs=0.05)
