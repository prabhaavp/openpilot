#!/usr/bin/env python3
import json
from typing import Any

from openpilot.common.params import Params, ParamKeyType, UnknownKeyName
from openpilot.starpilot.common.starpilot_variables import EXCLUDED_KEYS, LEGACY_STARPILOT_PARAM_RENAMES


def get_valid_param_keys() -> set[str]:
    """Get the set of all valid parameter key"""
    params = Params(return_defaults=True)
    return {k.decode() if isinstance(k, bytes) else k for k in params.all_keys()}


def validate_param_key(key: str) -> bool:
    """Check if a parameter key is valid (exists in params_keys.h)."""
    try:
        params = Params(return_defaults=True)
        params.check_key(key)
        return True
    except UnknownKeyName:
        return False


def validate_param_value(key: str, value: Any) -> tuple[bool, str | None]:
    """
    Validate a parameter value against its expected type
    Returns (is_valid, error_message).
    """
    if value is None:
        return True, None

    params = Params(return_defaults=True)
    key_type = params.get_type(key)

    try:
        if key_type == ParamKeyType.BOOL:
            if isinstance(value, bool):
                return True, None
            if isinstance(value, (int, float)):
                return True, None
            if isinstance(value, str):
                lower_val = value.strip().lower()
                if lower_val in ("0", "1", "true", "false", ""):
                    return True, None
                return False, f"Invalid boolean value: {value}"
            return False, f"Cannot coerce to bool: {type(value).__name__}"

        elif key_type == ParamKeyType.INT:
            if isinstance(value, int):
                return True, None
            if isinstance(value, (float, str)):
                float(value)
                return True, None
            return False, f"Cannot coerce to int: {type(value).__name__}"

        elif key_type == ParamKeyType.FLOAT:
            if isinstance(value, (int, float)):
                return True, None
            if isinstance(value, str):
                float(value)
                return True, None
            return False, f"Cannot coerce to float: {type(value).__name__}"

        elif key_type == ParamKeyType.STRING:
            return True, None

        elif key_type == ParamKeyType.JSON:
            if isinstance(value, (dict, list, str)):
                if isinstance(value, str):
                    json.loads(value)
                return True, None
            return False, f"Invalid JSON value: {type(value).__name__}"

        elif key_type == ParamKeyType.BYTES:
            return True, None

        elif key_type == ParamKeyType.TIME:
            return True, None

        else:
            return True, None

    except (ValueError, TypeError, json.JSONDecodeError) as e:
        return False, str(e)


def sanitize_toggle_payload(toggle_values: dict[str, Any]) -> tuple[dict[str, Any], list[str]]:
    """
    Sanitize a toggle payload by filtering out invalid/deprecated keys and values
    Returns (sanitized_payload, list_of_invalid_keys)
    """
    sanitized = {}
    invalid_keys = []

    for key, value in toggle_values.items():
        mapped_key = LEGACY_STARPILOT_PARAM_RENAMES.get(key, key)

        if mapped_key in EXCLUDED_KEYS:
            invalid_keys.append(key)
            continue

        if not validate_param_key(mapped_key):
            invalid_keys.append(key)
            continue

        is_valid, _error = validate_param_value(mapped_key, value)
        if is_valid:
            sanitized[mapped_key] = value
        else:
            invalid_keys.append(key)

    return sanitized, invalid_keys


def scan_invalid_toggles() -> dict[str, list[str]]:
    """
    Scan currently stored params for invalid/deprecated toggle key
    Returns dict with 'invalid' and 'deprecated' lists.
    """
    params = Params()
    valid_keys = get_valid_param_keys()

    invalid_keys = []
    deprecated_keys = []

    for key in params.all_keys():
        key_str = key.decode() if isinstance(key, bytes) else str(key)

        if key_str in LEGACY_STARPILOT_PARAM_RENAMES:
            deprecated_keys.append(key_str)
        elif key_str not in valid_keys and key_str not in EXCLUDED_KEYS:
            invalid_keys.append(key_str)

    return {"invalid": invalid_keys, "deprecated": deprecated_keys}


def remove_invalid_toggles(toggles: dict[str, Any] | None = None) -> tuple[int, list[str]]:
    """
    Remove invalid/deprecated toggles from params
    Returns (count_removed, list_of_removed_keys)
    """
    params = Params()
    removed_keys = []
    valid_keys = get_valid_param_keys()

    if toggles is not None:
        for key in toggles:
            mapped_key = LEGACY_STARPILOT_PARAM_RENAMES.get(key, key)
            if not validate_param_key(mapped_key) or mapped_key in EXCLUDED_KEYS:
                removed_keys.append(key)
        return len(removed_keys), removed_keys

    for key in list(params.all_keys()):
        key_str = key.decode() if isinstance(key, bytes) else str(key)

        if key_str in LEGACY_STARPILOT_PARAM_RENAMES:
            # Deprecated key - remove the old key, keep the renamed version
            params.remove(key_str)
            removed_keys.append(key_str)
        elif key_str in EXCLUDED_KEYS:
            continue
        elif key_str not in valid_keys:
            params.remove(key_str)
            removed_keys.append(key_str)

    return len(removed_keys), removed_keys