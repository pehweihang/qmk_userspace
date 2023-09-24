// Copyright 2023 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
// SPDX-License-Identifier: GPL-3.0-or-later


#include "drashna.h"
#include "analog.h"
#include "pointing_device.h"

#define KC_X0 LT(_FN, KC_ESC)

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_QWERTY] = LAYOUT(
     KC_VOLU, KC_MPLY, KC_MPRV, QK_BOOT,
     KC_VOLD, KC_MUTE, KC_MNXT, QK_BOOT
  ),

};
// clang-format on
