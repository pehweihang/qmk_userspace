// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-3.0-or-later

#include QMK_KEYBOARD_H

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    LAYOUT(
                 RGB_MOD,
        RGB_VAD, KC_C,    RGB_VAI,          QK_BOOT, EE_CLR,
                 RGB_RMOD
    )
};
#endif
