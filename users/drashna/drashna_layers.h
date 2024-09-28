// Copyright 2024 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "action_layer.h"

/* Define layer names */
enum userspace_layers {
    _QWERTY             = 0,
    _NUMLOCK            = 0,
    FIRST_DEFAULT_LAYER = 0,
    _COLEMAK_DH,
    _COLEMAK,
    _DVORAK,
    LAST_DEFAULT_LAYER = _DVORAK,
    _GAMEPAD,
    _DIABLO,
    _DIABLOII,
    _MOUSE,
    _MEDIA,
    _LOWER,
    _RAISE,
    _ADJUST,
    MAX_USER_LAYERS,
};

#define _MACROS          _MOUSE
#define _DEFAULT_LAYER_1 FIRST_DEFAULT_LAYER
#define _DEFAULT_LAYER_2 (FIRST_DEFAULT_LAYER + 1)
#define _DEFAULT_LAYER_3 (FIRST_DEFAULT_LAYER + 2)
#define _DEFAULT_LAYER_4 (FIRST_DEFAULT_LAYER + 3)
#if LAST_DEFAULT_LAYER > (FIRST_DEFAULT_LAYER + 3)
#    define _DEFAULT_LAYER_2 (FIRST_DEFAULT_LAYER + 4)
#    define _DEFAULT_LAYER_3 (FIRST_DEFAULT_LAYER + 5)
#    define _DEFAULT_LAYER_4 (FIRST_DEFAULT_LAYER + 6)
#    define _DEFAULT_LAYER_2 (FIRST_DEFAULT_LAYER + 7)
#    if LAST_DEFAULT_LAYER > (FIRST_DEFAULT_LAYER + 7)
#        define _DEFAULT_LAYER_2 (FIRST_DEFAULT_LAYER + 8)
#        define _DEFAULT_LAYER_3 (FIRST_DEFAULT_LAYER + 9)
#        define _DEFAULT_LAYER_4 (FIRST_DEFAULT_LAYER + 10)
#        define _DEFAULT_LAYER_4 (FIRST_DEFAULT_LAYER + 11)
#    endif // LAST_DEFAULT_LAYER > (FIRST_DEFAULT_LAYER + 7)
#endif     // LAST_DEFAULT_LAYER > (FIRST_DEFAULT_LAYER + 3)

#define DEFAULT_LAYER_1_HSV HSV_CYAN
#define DEFAULT_LAYER_2_HSV HSV_CHARTREUSE
#define DEFAULT_LAYER_3_HSV HSV_MAGENTA
#define DEFAULT_LAYER_4_HSV HSV_GOLDENROD

#define DEFAULT_LAYER_1_RGB RGB_CYAN
#define DEFAULT_LAYER_2_RGB RGB_CHARTREUSE
#define DEFAULT_LAYER_3_RGB RGB_MAGENTA
#define DEFAULT_LAYER_4_RGB RGB_GOLDENROD

bool        is_gaming_layer_active(layer_state_t state);
void        format_layer_bitmap_string(char *buffer, layer_state_t state, layer_state_t default_state);
const char *get_layer_name_string(layer_state_t state, bool alt_name, bool is_default);
