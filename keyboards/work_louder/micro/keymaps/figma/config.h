// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

// #define MIDI_BASIC
// #define NO_ACTION_ONESHOT

#define USB_SUSPEND_WAKEUP_DELAY 200

#undef RGBLIGHT_DEFAULT_MODE
#undef RGBLIGHT_DEFAULT_HUE

#define RGBLIGHT_DEFAULT_MODE RGBLIGHT_MODE_STATIC_LIGHT
#define RGBLIGHT_DEFAULT_HUE 159
#define RGBLIGHT_DEFAULT_SAT 8
#define RGBLIGHT_DEFAULT_VAL 255
#undef RGBLIGHT_LIMIT_VAL
#define RGBLIGHT_LIMIT_VAL 255


#define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_SOLID_COLOR
#define RGB_MATRIX_DEFAULT_HUE 176
#define RGB_MATRIX_DEFAULT_SAT 232
#define RGB_MATRIX_DEFAULT_VAL 255
#undef RGB_MATRIX_MAXIMUM_BRIGHTNESS
#define RGB_MATRIX_MAXIMUM_BRIGHTNESS 255

#define DYNAMIC_KEYMAP_LAYER_COUNT 8
