// Copyright 2023 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#define USE_I2C

/* Select hand configuration */

// #define MASTER_LEFT
// #define MASTER_RIGHT
#define EE_HANDS

/* key combination for magic key command */
#undef IS_COMMAND
#define IS_COMMAND() (get_mods() == (MOD_BIT(KC_LSFT) | MOD_BIT(KC_LALT)))

#ifdef RGBLIGHT_ENABLE
#    define WS2812_DI_PIN D3
#    define RGBLED_NUM    16 // Number of LEDs
#    define RGBLED_SPLIT  {8, 8}

#    define RGBLIGHT_HUE_STEP             12
#    define RGBLIGHT_SAT_STEP             12
#    define RGBLIGHT_VAL_STEP             12
#    define RGBLIGHT_EFFECT_KNIGHT_LENGTH 2
#    define RGBLIGHT_EFFECT_SNAKE_LENGTH  2
#endif // RGBLIGHT_ENABLE

#ifdef AUDIO_ENABLE
#    define AUDIO_PIN C6
#    ifdef RGBLIGHT_ENABLE
#        define NO_MUSIC_MODE
#    endif
#endif // AUDIO_ENABLE

#ifdef KEYBOARD_orthodox_rev1
#    undef PRODUCT
#    define PRODUCT "Drashna Hacked Orthodox Rev .1"
#elif KEYBOARD_orthodox_rev3
#    undef PRODUCT
#    define PRODUCT "Drashna Hacked Orthodox Rev .3"
#endif

#define QMK_ESC_OUTPUT D7 // usually COL
#ifdef KEYBOARD_orthodox_rev1
#    define QMK_ESC_INPUT D4 // usually ROW
#else
#    define QMK_ESC_INPUT D2 // usually ROW
#endif
#define QMK_LED     B0
#define QMK_SPEAKER C6

#define SHFT_LED1 5
#define SHFT_LED2 10

#define CTRL_LED1 6
#define CTRL_LED2 9

#define GUI_LED1 8
#define ALT_LED1 7
