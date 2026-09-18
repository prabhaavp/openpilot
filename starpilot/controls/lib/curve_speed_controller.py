#!/usr/bin/env python3
import math

import numpy as np

from cereal import custom

from openpilot.common.constants import CV
from openpilot.common.filter_simple import FirstOrderFilter
from openpilot.common.realtime import DT_MDL

from openpilot.starpilot.common.starpilot_variables import (
  CITY_SPEED_LIMIT,
  CRUISING_SPEED,
  DEFAULT_LATERAL_ACCELERATION,
  PLANNER_TIME,
)

CALIBRATION_PROGRESS_THRESHOLD = 10 / DT_MDL
CSC_MIN_SPEED = CITY_SPEED_LIMIT * CV.MPH_TO_MS

CSC_APPROACH_DECEL = 0.3
CSC_TARGET_UP_RATE = 3.0
CSC_TARGET_DOWN_RATE = 2.5
CSC_TARGET_FILTER_RC = 0.4
CSC_EGO_HEADROOM = 2.0
CSC_RELEASE_DEBOUNCE = 0.25
CSC_ACTIVE_ON_DELTA = 0.5
CSC_ACTIVE_OFF_DELTA = 0.25
CSC_GLOW_ON_DELTA = 1.0
CSC_GLOW_HOLD_TIME = 3.0

CSC_COUNT_CAP = 600
CSC_PRIOR_COUNT = 100
CSC_LAT_ACCEL_MIN = 1.2
CSC_LAT_ACCEL_MAX = 3.2
CSC_MAX_LATERAL_ACCEL = 4.0
CSC_NUDGE = 0.15
CSC_NUDGE_WEIGHT = 20
CSC_OVERRIDE_WATCH_TIME = 6.0
CSC_TRAINING_QUIET_TIME = 5.0
CSC_TRAINING_SETTLE_TIME = 2.0
CSC_COMFORT_MARGIN = 1.0

CSC_SETTLE_TIME = 1.2
CSC_BRAKE_LEAD_MIN = 0.0
CSC_BRAKE_LEAD_MAX = 3.0

CSC_SPEED_COMFORT_BP = [20.0, 35.0]      # m/s (~45 mph to ~78 mph)
CSC_SPEED_COMFORT_SCALE = [1.0, 0.88]

# Speed at which a curve observation/query switches between the low-speed and
# high-speed learned habits (~45 mph). Separate buckets keep highway and local
# cornering comfort from averaging into one aggressive number.
CSC_SPEED_TIER_SPLIT = 20.0

CSC_MAP_STREAM_MARGIN = 0.5

CSC_FARFIELD_MIN_CURVATURE = 0.004
CSC_FARFIELD_MIN_DISTANCE = 30.0
CSC_FARFIELD_GAIN = 1.23

MIN_CURVATURE = 0.0005
MAX_CURVATURE = 0.02
CURVATURE_BUCKETS = 24
ROUNDING_PRECISION = 6
CURVATURE_GRID = MIN_CURVATURE * np.power(MAX_CURVATURE / MIN_CURVATURE,
                                          np.arange(CURVATURE_BUCKETS) / (CURVATURE_BUCKETS - 1))
LOG_CURVATURE_GRID = np.log(CURVATURE_GRID)


PRIOR_CURVATURE_BP = [0.001, 0.003, 0.01, 0.03, 0.1]
PRIOR_LAT_ACCEL_V = [1.5, 1.8, 2.2, 2.6, 2.9]


def weighted_isotonic(values, weights):
  """Weighted non-decreasing fit (pool adjacent violators).

  Keeps comfort from falling as curves tighten, without letting a sparse bucket
  overrule a well-sampled neighbour the way a running maximum would.
  """
  block_values: list[float] = []
  block_weights: list[float] = []
  block_sizes: list[int] = []

  for value, weight in zip(values, weights, strict=True):
    block_values.append(float(value))
    block_weights.append(float(weight))
    block_sizes.append(1)

    while len(block_values) > 1 and block_values[-2] > block_values[-1]:
      merged_weight = block_weights[-2] + block_weights[-1]
      merged_value = ((block_values[-2] * block_weights[-2]) + (block_values[-1] * block_weights[-1])) / merged_weight
      block_values.pop()
      block_weights.pop()
      merged_size = block_sizes.pop()
      block_values[-1] = merged_value
      block_weights[-1] = merged_weight
      block_sizes[-1] += merged_size

  fitted = np.empty(len(values))
  index = 0
  for value, size in zip(block_values, block_sizes, strict=True):
    fitted[index:index + size] = value
    index += size
  return fitted


def is_user_overriding_longitudinal(sm):
  try:
    if any(getattr(event, "overrideLongitudinal", False) for event in sm["onroadEvents"]):
      return True
  except (KeyError, TypeError):
    pass

  car_state = sm["carState"]
  starpilot_car_state = sm["starpilotCarState"]
  return bool(
    getattr(car_state, "gasPressed", False) or
    getattr(car_state, "brakePressed", False) or
    getattr(starpilot_car_state, "accelPressed", False)
  )


def is_manual_speed_control(sm):
  """Return whether the driver, rather than longitudinal control, owns speed."""
  return not bool(sm["carControl"].longActive) or is_user_overriding_longitudinal(sm)


class CurveSpeedController:
  def __init__(self, StarPilotVCruise):
    self.starpilot_planner = StarPilotVCruise.starpilot_planner

    self.enable_training = False
    self.nudge_applied = False

    self.override_watch_key = None
    self.override_watch_peak = 0.0
    self.override_watch_timer = 0.0

    self.training_timer = 0.0
    self.persistence_timer = 0.0
    self.training_quiet_timer = 0.0
    self.data_dirty = False

    self.target = 0.0
    self.binding_distance = 0.0
    self.release_timer = 0.0
    self.target_filter = FirstOrderFilter(0.0, CSC_TARGET_FILTER_RC, DT_MDL, initialized=False)
    self.seed_pending = True

    self._long_active_prev = False

    curvature_data = self.starpilot_planner.params.get("CurvatureData")
    self.curvature_data = self._normalize_curvature_data(curvature_data)

    self.required_curvatures = [self._bucket_curvature(curvature) for curvature in CURVATURE_GRID]

    self.rebuild_lat_accel_curve()

    self.data_dirty = True

    self._publish_live_values()

  @staticmethod
  def _bucket_curvature(road_curvature):
    clipped_curvature = float(np.clip(abs(road_curvature), MIN_CURVATURE, MAX_CURVATURE))

    bucket_index = int(np.argmin(np.abs(LOG_CURVATURE_GRID - np.log(clipped_curvature))))
    return str(round(float(CURVATURE_GRID[bucket_index]), ROUNDING_PRECISION))

  @staticmethod
  def _weighted_merge(average_a, count_a, average_b, count_b):
    total = count_a + count_b
    if total <= 0:
      return average_a
    return ((average_a * count_a) + (average_b * count_b)) / total

  @classmethod
  def _merge_buckets(cls, existing, entry):
    return {
      "average": cls._weighted_merge(existing["average"], existing["count"], entry["average"], entry["count"]),
      "count": existing["count"] + entry["count"],
      "average_low": cls._weighted_merge(existing["average_low"], existing["count_low"], entry["average_low"], entry["count_low"]),
      "count_low": existing["count_low"] + entry["count_low"],
      "average_high": cls._weighted_merge(existing["average_high"], existing["count_high"], entry["average_high"], entry["count_high"]),
      "count_high": existing["count_high"] + entry["count_high"],
    }

  @classmethod
  def _normalize_curvature_data(cls, curvature_data):
    """Load persisted data, migrating legacy flat buckets into speed tiers.

    Legacy entries only carried "average"/"count". They are treated as low-speed
    observations (count_low = count, count_high = 0) so the established
    calibration is preserved and the high-speed tier falls back to it until real
    high-speed data is collected. The base "average"/"count" keys are kept and
    continue to update, so external tools remain compatible.
    """
    if not isinstance(curvature_data, dict):
      return {}

    normalized = {}
    for key, value in curvature_data.items():
      if not isinstance(value, dict):
        continue

      try:
        raw_curvature = abs(float(key))
        average = float(value["average"])
        count = int(value["count"])
      except (KeyError, TypeError, ValueError):
        continue

      if count <= 0 or not math.isfinite(raw_curvature) or not math.isfinite(average):
        continue

      try:
        count_low = max(int(value.get("count_low", count) or 0), 0)
        average_low = float(value.get("average_low", average))
        count_high = max(int(value.get("count_high", 0) or 0), 0)
        average_high = float(value.get("average_high", average))
      except (TypeError, ValueError):
        count_low, average_low, count_high, average_high = count, average, 0, average
      if not math.isfinite(average_low):
        average_low = average
      if not math.isfinite(average_high):
        average_high = average

      entry = {
        "average": average,
        "count": count,
        "average_low": average_low,
        "count_low": count_low,
        "average_high": average_high,
        "count_high": count_high,
      }

      bucket = cls._bucket_curvature(raw_curvature)
      normalized[bucket] = cls._merge_buckets(normalized[bucket], entry) if bucket in normalized else entry

    return normalized

  def _calibration_progress(self):
    progress = 0.0
    for key in self.required_curvatures:
      if key in self.curvature_data:
        progress += min(self.curvature_data[key]["count"] / CALIBRATION_PROGRESS_THRESHOLD, 1.0)
    return (progress / len(self.required_curvatures)) * 100

  def _publish_live_values(self, progress=None):
    params_memory = getattr(self.starpilot_planner, "params_memory", None)
    if params_memory is None:
      return
    if progress is None:
      progress = self._calibration_progress()
    params_memory.put_nonblocking("CalibratedLateralAcceleration", self.lateral_acceleration)
    params_memory.put_nonblocking("CalibrationProgress", progress)

  def _persist_data(self):
    if not self.data_dirty:
      return

    progress = self._calibration_progress()
    self.starpilot_planner.params.put_nonblocking("CalibratedLateralAcceleration", self.lateral_acceleration)
    self.starpilot_planner.params.put_nonblocking("CalibrationProgress", progress)
    self.starpilot_planner.params.put_nonblocking("CurvatureData", self.curvature_data)
    self._publish_live_values(progress)
    self.data_dirty = False
    self.persistence_timer = 0.0

  def flush_data(self):
    self._persist_data()

  def log_data(self, v_ego, sm):
    self.training_quiet_timer = max(self.training_quiet_timer - DT_MDL, 0.0)

    eligible = (
      v_ego > CRUISING_SPEED and
      not self.starpilot_planner.tracking_lead and
      is_manual_speed_control(sm) and
      self.training_quiet_timer <= 0.0
    )
    self.enable_training = False

    if not eligible:
      self.flush_data()

      self.training_timer = max(self.training_timer - DT_MDL, 0.0)
      self.persistence_timer = 0.0
      return

    self.training_timer += DT_MDL
    if self.data_dirty:
      self.persistence_timer += DT_MDL

    in_curve = (
      self.training_timer >= CSC_TRAINING_SETTLE_TIME and
      self.starpilot_planner.driving_in_curve and
      not (sm["carState"].leftBlinker or sm["carState"].rightBlinker)
    )
    if in_curve:
      lateral_acceleration = abs(self.starpilot_planner.lateral_acceleration)
      road_curvature = self._bucket_curvature(abs(self.starpilot_planner.road_curvature))
      data = self._ensure_entry(road_curvature, lateral_acceleration)

      # Base keys keep updating for backward compatibility; the speed tier keeps
      # highway and local cornering habits from averaging together.
      self._update_ema(data, "average", "count", lateral_acceleration)
      average_key, count_key = self._tier_keys(v_ego)
      self._update_ema(data, average_key, count_key, lateral_acceleration)

      self.data_dirty = True
      self.rebuild_lat_accel_curve()
      self._publish_live_values()
      self.enable_training = True

      if self.persistence_timer >= PLANNER_TIME:
        self.flush_data()
    elif self.data_dirty:
      self.flush_data()

  def handle_override(self, v_ego, was_controlling, sm, accel_button=False):
    long_active = bool(sm["carControl"].longActive)
    long_dropped = self._long_active_prev and not long_active
    self._long_active_prev = long_active

    self._update_override_watch(sm, v_ego)

    if not was_controlling:
      self.nudge_applied = False
      return

    if self.nudge_applied:
      return

    if accel_button or (sm["carState"].gasPressed and self.target < v_ego - 0.5):
      self.override_watch_key = self._bucket_curvature(abs(self.starpilot_planner.road_curvature))
      self.override_watch_peak = abs(self.starpilot_planner.lateral_acceleration)
      self.override_watch_timer = CSC_OVERRIDE_WATCH_TIME
      self.nudge_applied = True
    elif (getattr(sm["carState"], "brakePressed", False) or long_dropped) and self.starpilot_planner.driving_in_curve:
      self._apply_nudge(-CSC_NUDGE, v_ego)

  def _update_override_watch(self, sm, v_ego=None):
    if self.override_watch_key is None:
      return

    lateral_acceleration = abs(self.starpilot_planner.lateral_acceleration)
    if lateral_acceleration > self.override_watch_peak:

      self.override_watch_peak = lateral_acceleration
      self.override_watch_key = self._bucket_curvature(abs(self.starpilot_planner.road_curvature))

    self.override_watch_timer -= DT_MDL
    if self.override_watch_timer > 0.0 and (is_user_overriding_longitudinal(sm) or
                                            self.starpilot_planner.driving_in_curve):
      return

    key = self.override_watch_key
    self.override_watch_key = None

    self._record_pseudo_sample(key, max(self.override_watch_peak,
                                        self.learned_lat_accel(float(key)) + CSC_NUDGE), v_ego)

  def _apply_nudge(self, offset, v_ego=None):
    key = self._bucket_curvature(abs(self.starpilot_planner.road_curvature))

    self._record_pseudo_sample(key, self.learned_lat_accel(float(key)) + offset, v_ego)
    self.nudge_applied = True

  @staticmethod
  def _tier_keys(v_ego):
    return ("average_high", "count_high") if v_ego >= CSC_SPEED_TIER_SPLIT else ("average_low", "count_low")

  def _ensure_entry(self, key, sample):
    data = self.curvature_data.get(key)
    if data is None:
      data = {
        "average": sample,
        "count": 0,
        "average_low": sample,
        "count_low": 0,
        "average_high": sample,
        "count_high": 0,
      }
      self.curvature_data[key] = data
    return data

  @staticmethod
  def _update_ema(data, average_key, count_key, sample, weight=1):
    effective_count = min(data.get(count_key, 0), CSC_COUNT_CAP)
    total = effective_count + weight
    data[average_key] = ((data.get(average_key, sample) * effective_count) + (sample * weight)) / total
    data[count_key] = data.get(count_key, 0) + weight

  def _record_pseudo_sample(self, key, sample, v_ego=None):
    sample = float(np.clip(sample, CSC_LAT_ACCEL_MIN, CSC_LAT_ACCEL_MAX))
    data = self._ensure_entry(key, sample)

    self._update_ema(data, "average", "count", sample, CSC_NUDGE_WEIGHT)
    if v_ego is not None:
      average_key, count_key = self._tier_keys(v_ego)
      self._update_ema(data, average_key, count_key, sample, CSC_NUDGE_WEIGHT)

    self.rebuild_lat_accel_curve()
    self.data_dirty = True
    self.flush_data()

  def _fit_curve(self, prior, average_key, count_key, fallback):
    blended = prior.copy()
    counts = np.zeros(len(self.required_curvatures))
    for i, key in enumerate(self.required_curvatures):
      data = self.curvature_data.get(key)
      if data is None:
        continue

      count = data.get(count_key, 0)
      if count <= 0:
        if fallback is not None:
          blended[i] = fallback[i]
        continue

      average = data.get(average_key, data["average"])
      confidence = count / (count + CSC_PRIOR_COUNT)
      blended[i] = confidence * average + (1.0 - confidence) * prior[i]
      counts[i] = count

    blended = np.clip(blended, CSC_LAT_ACCEL_MIN, CSC_LAT_ACCEL_MAX)
    return weighted_isotonic(blended, counts + CSC_PRIOR_COUNT), counts

  def rebuild_lat_accel_curve(self):
    grid_k = np.array([float(key) for key in self.required_curvatures])
    prior = np.interp(grid_k, PRIOR_CURVATURE_BP, PRIOR_LAT_ACCEL_V)

    base_curve, counts = self._fit_curve(prior, "average", "count", fallback=None)
    low_curve, _ = self._fit_curve(prior, "average_low", "count_low", fallback=base_curve)
    # Until real high-speed data exists, the high tier mirrors the low tier so a
    # highway sweeper never falls back to the aggressive combined prior.
    high_curve, _ = self._fit_curve(prior, "average_high", "count_high", fallback=low_curve)

    self._curve_k = grid_k
    self._curve_a = base_curve
    self._curve_a_low = low_curve
    self._curve_a_high = high_curve

    if counts.sum() > 0:
      self.lateral_acceleration = float(np.average(base_curve, weights=counts))
    else:
      self.lateral_acceleration = DEFAULT_LATERAL_ACCELERATION

  def learned_lat_accel(self, curvature):
    """Comfort level learned for this curvature, before any control margin."""
    return float(np.interp(abs(curvature), self._curve_k, self._curve_a))

  def lat_accel_for_curvature(self, curvature):
    curvature_abs = np.abs(curvature)
    curvature_floor = np.maximum(curvature_abs, 1e-4)

    # Pick the learned habit whose recording speed matches how this curvature is
    # actually taken. Using the curvature's nominal speed (rather than current
    # ego speed) keeps the choice stable through the apex and independent of
    # transient speed.
    combined = np.interp(curvature_abs, self._curve_k, self._curve_a)
    nominal_speed = np.sqrt(np.maximum(combined, 0.0) / curvature_floor)
    low_curve = np.interp(curvature_abs, self._curve_k, self._curve_a_low)
    high_curve = np.interp(curvature_abs, self._curve_k, self._curve_a_high)
    base_lat_accel = np.where(nominal_speed >= CSC_SPEED_TIER_SPLIT, high_curve, low_curve) * CSC_COMFORT_MARGIN

    # Mild high-speed cushion on top of the tier split.
    speed_scale = np.interp(nominal_speed, CSC_SPEED_COMFORT_BP, CSC_SPEED_COMFORT_SCALE)
    lat_accel = base_lat_accel * speed_scale

    weather = self.starpilot_planner.starpilot_weather
    if weather.weather_id != 0:
      lat_accel = lat_accel * (1.0 - weather.reduce_lateral_acceleration)

    return lat_accel

  def _get_map_curve_target(self):
    """Pure curve-induced speed from MapD, without overriding speed limits or SLC.

    MapdOut.suggestedSpeed is a composite of the posted limit and any curve
    advisory, so it is only trusted when it is explicitly below MapD's own
    speed-limit suggestion. The dedicated mapCurveSpeed field is preferred when
    the mapd build populates it.

    The reader is published by the planner so the CSC call site stays a plain
    (v_ego, v_cruise) update.
    """
    mapd_out = getattr(self.starpilot_planner, "mapd_out", None)
    if mapd_out is None:
      return None

    way_selection_type = getattr(mapd_out, "waySelectionType", custom.WaySelectionType.fail)
    if way_selection_type == custom.WaySelectionType.fail:
      return None

    map_curve_speed = float(getattr(mapd_out, "mapCurveSpeed", 0.0) or 0.0)
    if map_curve_speed > 0.0:
      return max(map_curve_speed, CSC_MIN_SPEED)

    suggested_speed = float(getattr(mapd_out, "suggestedSpeed", 0.0) or 0.0)
    speed_limit_suggested = float(getattr(mapd_out, "speedLimitSuggestedSpeed", 0.0) or 0.0)
    if (suggested_speed > 0.0 and speed_limit_suggested > 0.0 and
        suggested_speed < speed_limit_suggested - CSC_MAP_STREAM_MARGIN):
      return max(suggested_speed, CSC_MIN_SPEED)

    return None

  @staticmethod
  def _correct_far_field(curvatures, distances):
    """Undo the model's known under-read of distant curvature, where the reading is firm.

    Only the gained, firm readings are capped at MAX_CURVATURE. Clamping the raw
    near readings down to the grid maximum would erase genuinely sharp bends and
    let the minimum-speed floor override the lateral-acceleration cap.
    """
    firm = (curvatures >= CSC_FARFIELD_MIN_CURVATURE) & (distances >= CSC_FARFIELD_MIN_DISTANCE)
    gained = np.minimum(curvatures * CSC_FARFIELD_GAIN, MAX_CURVATURE)
    return np.where(firm, gained, curvatures)

  def reset(self, v_cruise):
    self.target = float(v_cruise)
    self.release_timer = 0.0
    self.target_filter.x = float(v_cruise)
    self.target_filter.initialized = True
    self.seed_pending = True

  def update_target(self, v_ego, v_cruise):
    if not self.target_filter.initialized:
      self.reset(v_cruise)

    candidates = [float(v_cruise)]

    map_target = self._get_map_curve_target()
    if map_target is not None:
      candidates.append(map_target)

    vision_target = None
    self.binding_distance = 0.0
    curvatures, distances = self.starpilot_planner.curve_profile
    if len(curvatures) > 0:
      curvatures = self._correct_far_field(curvatures, distances)
      lat_accel = self.lat_accel_for_curvature(curvatures)
      curvature_floor = np.maximum(curvatures, 1e-4)
      point_speeds = np.sqrt(lat_accel / curvature_floor)
      point_speeds = np.minimum(
        np.maximum(point_speeds, CSC_MIN_SPEED),
        np.sqrt(CSC_MAX_LATERAL_ACCEL / curvature_floor),
      )

      # GG-Bowl taper: reserve enough distance to finish braking brake_lead
      # seconds before the apex, then glide through the turn without friction
      # brakes. The lead is user-tunable via CurveSpeedControllerBrakeLead.
      brake_lead = getattr(self.starpilot_planner, "csc_brake_lead", None)
      brake_lead = CSC_SETTLE_TIME if brake_lead is None else float(brake_lead)
      brake_lead = float(np.clip(brake_lead, CSC_BRAKE_LEAD_MIN, CSC_BRAKE_LEAD_MAX))
      settle_distances = point_speeds * brake_lead
      effective_distances = np.maximum(distances - settle_distances, 0.0)
      allowed_speeds = np.sqrt(point_speeds**2 + 2.0 * CSC_APPROACH_DECEL * effective_distances)
      binding_index = int(np.argmin(allowed_speeds))
      vision_target = float(allowed_speeds[binding_index])
      candidates.append(vision_target)
      self.binding_distance = float(distances[binding_index])

    raw_target = min(candidates)
    if raw_target >= v_cruise:
      self.binding_distance = 0.0
    elif map_target is not None and raw_target >= map_target - 1e-9 and (
        vision_target is None or map_target <= vision_target):
      # The MapD lookahead is the binding constraint; there is no vision apex to report.
      self.binding_distance = 0.0

    if self.seed_pending:
      seed = min(float(v_cruise), max(raw_target, v_ego + CSC_EGO_HEADROOM))
      self.target = seed
      self.target_filter.x = seed
      self.seed_pending = False

    if raw_target >= v_ego:
      self.release_timer += DT_MDL
    else:
      self.release_timer = 0.0

    filtered = self.target_filter.update(raw_target)
    self.target = float(np.clip(max(filtered, min(raw_target, v_ego + CSC_EGO_HEADROOM)),
                                self.target - CSC_TARGET_DOWN_RATE * DT_MDL,
                                self.target + CSC_TARGET_UP_RATE * DT_MDL))

    if self.release_timer >= CSC_RELEASE_DEBOUNCE:
      self.target = max(self.target, min(raw_target, v_ego))

    if self.target < v_cruise - CSC_ACTIVE_ON_DELTA:
      self.training_quiet_timer = CSC_TRAINING_QUIET_TIME
