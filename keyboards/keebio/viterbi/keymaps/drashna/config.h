// Copyright 2023 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#undef MATRIX_ROWS
#define MATRIX_ROWS 5

#ifdef RGBLIGHT_ENABLE
#    undef RGBLED_NUM
#    define RGBLED_NUM 16

#    define RGBLIGHT_HUE_STEP 8
#    define RGBLIGHT_SAT_STEP 8
#    define RGBLIGHT_VAL_STEP 8
#    define RGBLIGHT_EFFECT_KNIGHT_LENGTH 2
#    define RGBLIGHT_EFFECT_SNAKE_LENGTH 2
#endif  // RGBLIGHT_ENABLE

#undef PRODUCT
#define PRODUCT "Drashnas Viterbi Macro Pad"

#ifdef AUDIO_ENABLE
#    define AUDIO_PIN C6
#    define NO_MUSIC_MODE
#endif

// clang-format off
#define LAYOUT_ortho_5x7( \
    L00, L01, L02, L03, L04, L05, L06, \
    L10, L11, L12, L13, L14, L15, L16, \
    L20, L21, L22, L23, L24, L25, L26, \
    L30, L31, L32, L33, L34, L35, L36, \
    L40, L41, L42, L43, L44, L45, L46 \
    ) \
	{ \
		{ L00, L01, L02, L03, L04, L05, L06 }, \
		{ L10, L11, L12, L13, L14, L15, L16 }, \
		{ L20, L21, L22, L23, L24, L25, L26 }, \
		{ L30, L31, L32, L33, L34, L35, L36 }, \
		{ L40, L41, L42, L43, L44, L45, L46 }, \
  }
// clang-format on

#define QMK_ESC_OUTPUT F5  // usually COL
#define QMK_ESC_INPUT D4   // usually ROW
#define QMK_LED B0
#define QMK_SPEAKER C6
