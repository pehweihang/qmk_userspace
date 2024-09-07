// Copyright 2023 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#define EE_HANDS

#ifdef RGBLIGHT_ENABLE
#    undef RGBLED_NUM
#    define RGBLED_NUM 18 // Number of LEDs
#    undef RGBLED_SPLIT
#    define RGBLED_SPLIT                  {9, 9}
#    define RGBLIGHT_HUE_STEP             8
#    define RGBLIGHT_SAT_STEP             8
#    define RGBLIGHT_VAL_STEP             8
#    define RGBLIGHT_EFFECT_KNIGHT_LENGTH 2
#    define RGBLIGHT_EFFECT_SNAKE_LENGTH  2

#    ifndef RGBLIGHT_LIMIT_VAL
#        define RGBLIGHT_LIMIT_VAL 225
#    endif
#endif // RGBLIGHT_ENABLE

#ifdef AUDIO_ENABLE
#    define AUDIO_PIN C6
#    ifdef RGBLIGHT_ENABLE
#        ifndef __arm__
#            define NO_MUSIC_MODE
#        endif
#    endif // RGBLIGHT_ENABLE
#endif     // AUDIO_ENABLE

#if defined(KEYBOARD_keebio_iris_rev1) || defined(KEYBOARD_keebio_iris_rev2)
#    define QMK_ESC_OUTPUT F6 // usually COL
#    define QMK_ESC_INPUT  D7 // usually ROW
#    define QMK_LED        B0
#    define QMK_SPEAKER    C6
#endif

#if defined(KEYBOARD_keebio_iris_rev2)
#    undef PRODUCT
#    define PRODUCT "Drashna Hacked Iris Rev 2"
#elif defined(KEYBOARD_keebio_iris_rev3)
#    undef PRODUCT
#    define PRODUCT "Drashna Hacked Iris Rev 3"
#elif defined(KEYBOARD_keebio_iris_rev4)
#    undef PRODUCT
#    define PRODUCT "Drashna Hacked Iris Rev 4"
#endif

#define SHFT_LED1 6
#define SHFT_LED2 11

#define CTRL_LED1 7
#define CTRL_LED2 10

#define ALT_LED1 8
#define GUI_LED1 9

#define BOOTMAGIC_ROW    4
#define BOOTMAGIC_COLUMN 3
