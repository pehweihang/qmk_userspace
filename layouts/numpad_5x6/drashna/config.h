// Copyright 2023 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#if defined(RGBLIGHT_ENABLE) && !defined(RGBLED_NUM)
#    define WS2812_DI_PIN B1
#    define RGBLED_NUM    16 // Number of LEDs

#    define RGBLIGHT_HUE_STEP             12
#    define RGBLIGHT_SAT_STEP             12
#    define RGBLIGHT_VAL_STEP             12
#    define RGBLIGHT_EFFECT_KNIGHT_LENGTH 2
#    define RGBLIGHT_EFFECT_SNAKE_LENGTH  2
#endif // RGBLIGHT_ENABLE

#define RGBLIGHT_ALL_ANIMATIONS

#if defined(KEYBOARD_primekb_prime_m)
#    undef PRODUCT
#    define PRODUCT "Drashna Hacked Prime_M Macro Pad"
#endif
