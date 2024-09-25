// Copyright 2021 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
// Copyright 2024 burkfers (@burkfers)
// Copyright 2024 Wimads (@wimads)
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "drashna.h"

void           pointing_device_init_keymap(void);
report_mouse_t pointing_device_task_keymap(report_mouse_t mouse_report);
void           matrix_scan_pointing(void);
bool           process_record_pointing(uint16_t keycode, keyrecord_t* record);
layer_state_t  layer_state_set_pointing(layer_state_t state);
void           pointing_device_mouse_jiggler_toggle(void);

#ifdef POINTING_MODE_MAP_ENABLE
extern const uint16_t PROGMEM pointing_mode_maps[POINTING_MODE_MAP_COUNT][POINTING_NUM_DIRECTIONS];
#endif // POINTING_MODE_MAP_ENABLE

// lower/higher value = curve starts more smoothly/abruptly
#ifndef POINTING_DEVICE_ACCEL_TAKEOFF
#    define POINTING_DEVICE_ACCEL_TAKEOFF 2.0
#endif
// lower/higher value = curve reaches its upper limit slower/faster
#ifndef POINTING_DEVICE_ACCEL_GROWTH_RATE
#    define POINTING_DEVICE_ACCEL_GROWTH_RATE 0.25
#endif
// lower/higher value = acceleration kicks in earlier/later
#ifndef POINTING_DEVICE_ACCEL_OFFSET
#    define POINTING_DEVICE_ACCEL_OFFSET 2.2
#endif
// lower limit of accel curve (minimum acceleration factor)
#ifndef POINTING_DEVICE_ACCEL_LIMIT
#    define POINTING_DEVICE_ACCEL_LIMIT 0.2
#endif
// milliseconds to wait between requesting the device's current DPI
#ifndef POINTING_DEVICE_ACCEL_CPI_THROTTLE_MS
#    define POINTING_DEVICE_ACCEL_CPI_THROTTLE_MS 200
#endif
// upper limit of accel curve, recommended to leave at 1; adjust DPI setting instead.
#ifndef POINTING_DEVICE_ACCEL_LIMIT_UPPER
#    define POINTING_DEVICE_ACCEL_LIMIT_UPPER 1
#endif
// milliseconds after which to reset quantization error correction (forget rounding remainder)
#ifndef POINTING_DEVICE_ACCEL_ROUNDING_CARRY_TIMEOUT_MS
#    define POINTING_DEVICE_ACCEL_ROUNDING_CARRY_TIMEOUT_MS 200
#endif
#ifndef POINTING_DEVICE_ACCEL_TAKEOFF_STEP
#    define POINTING_DEVICE_ACCEL_TAKEOFF_STEP 0.01f
#endif // POINTING_DEVICE_ACCEL_TAKEOFF_STEP
#ifndef POINTING_DEVICE_ACCEL_GROWTH_RATE_STEP
#    define POINTING_DEVICE_ACCEL_GROWTH_RATE_STEP 0.01f
#endif // POINTING_DEVICE_ACCEL_GROWTH_RATE_STEP
#ifndef POINTING_DEVICE_ACCEL_OFFSET_STEP
#    define POINTING_DEVICE_ACCEL_OFFSET_STEP 0.1f
#endif // POINTING_DEVICE_ACCEL_OFFSET_STEP
#ifndef POINTING_DEVICE_ACCEL_LIMIT_STEP
#    define POINTING_DEVICE_ACCEL_LIMIT_STEP 0.01f
#endif // POINTING_DEVICE_ACCEL_LIMIT_STEP

void  pointing_device_accel_enabled(bool enable);
bool  pointing_device_accel_get_enabled(void);
void  pointing_device_accel_toggle_enabled(void);
float pointing_device_accel_get_takeoff(void);
float pointing_device_accel_get_growth_rate(void);
float pointing_device_accel_get_offset(void);
float pointing_device_accel_get_limit(void);
void  pointing_device_accel_set_takeoff(float val);
void  pointing_device_accel_set_growth_rate(float val);
void  pointing_device_accel_set_offset(float val);
void  pointing_device_accel_set_limit(float val);
float pointing_device_accel_get_mod_step(float step);
