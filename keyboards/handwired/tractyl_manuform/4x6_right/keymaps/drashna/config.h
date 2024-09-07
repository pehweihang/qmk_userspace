// Copyright 2023 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

// #define USE_I2C
// #define SELECT_SOFT_SERIAL_SPEED 1
#define SPLIT_MODS_ENABLE
#define EE_HANDS

#define TRACKBALL_DPI_OPTIONS {1200, 1800, 2600, 3400}

#define RGBLIGHT_MAX_BRIGHTNESS 60

#undef DEBOUNCE
#define DEBOUNCE 10

#define SOLENOID_PIN           F1
#define SOLENOID_DEFAULT_DWELL 8

#define OLED_DISPLAY_128X64
#define OLED_BRIGHTNESS 50
