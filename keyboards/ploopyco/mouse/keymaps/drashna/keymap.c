// Copyright 2023 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include QMK_KEYBOARD_H

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
                 C(KC_C), KC_BTN1, KC_BTN3, KC_BTN2, MO(1), KC_BTN4, KC_BTN5, DPI_CONFIG),
    [1] = LAYOUT(
                 RGB_HUI, RGB_MOD, RGB_TOG, RGB_RMOD, MO(1), KC_VOLU, KC_VOLD, QK_BOOT)
};
// clang-format on

#ifdef ENCODER_MAP_ENABLE
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0] = { ENCODER_CCW_CW( KC_WH_D, KC_WH_U ) },
    [1] = { ENCODER_CCW_CW( RGB_HUD, RGB_HUI ) },
};
#endif

#ifdef RGBLIGHT_ENABLE
void eeconkfig_init_user(void) {
    rgblight_enable();
#    ifdef RGBLIGHT_EFFECT_TWINKLE
    rgblight_mode(RGBLIGHT_MODE_TWINKLE + 5);
#    else
    rgblight_mode(RGBLIGHT_MODE_BREATHING + 5);
#    endif
    rgblight_sethsv(HSV_MAGENTA);
}
#endif
