// Copyright 2020 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef RGB_MATRIX_REST_MODE
#    if defined(SPLIT_KEYBOARD) || defined(KEYBOARD_ergodox_ez) || defined(KEYBOARD_zsa)
#        define RGB_MATRIX_REST_MODE RGB_MATRIX_CYCLE_OUT_IN_DUAL
#    else // defined(SPLIT_KEYBOARD) || defined(KEYBOARD_ergodox_ez) || defined(KEYBOARD_zsa)
#        define RGB_MATRIX_REST_MODE RGB_MATRIX_CYCLE_OUT_IN
#    endif // defined(SPLIT_KEYBOARD) || defined(KEYBOARD_ergodox_ez) || defined(KEYBOARD_zsa)
#endif     // RGB_MATRIX_REST_MODE

#undef RGB_MATRIX_DEFAULT_MODE
#define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_REST_MODE

#ifndef RGB_MATRIX_LED_PROCESS_LIMIT
#    define RGB_MATRIX_LED_PROCESS_LIMIT 5
#endif // RGB_MATRIX_LED_PROCESS_LIMIT
#ifndef RGB_MATRIX_LED_FLUSH_LIMIT
#    define RGB_MATRIX_LED_FLUSH_LIMIT 26
#endif // RGB_MATRIX_LED_FLUSH_LIMIT
