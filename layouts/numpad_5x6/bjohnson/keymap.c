// Copyright 2023 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include QMK_KEYBOARD_H

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_numpad_5x6(
        KC_F1,   LT(1,KC_F2),    KC_NUM,  KC_PSLS, KC_PAST, KC_PMNS,
        KC_F3,   KC_F4,    KC_P7,   KC_P8,   KC_P9,
        KC_F5,   KC_F6,    KC_P4,   KC_P5,   KC_P6,   KC_PPLS,
        KC_F7,   KC_F8,    KC_P1,   KC_P2,   KC_P3,
        KC_F9,   KC_F10,        KC_P0,       KC_PDOT, KC_PENT
    ),
    [1] = LAYOUT_numpad_5x6(
        RGB_TOG, _______,  _______, _______, _______, QK_BOOT,
        RGB_MOD, RGB_RMOD, _______, _______, _______,
        RGB_HUI, RGB_HUD,  _______, _______, _______, _______,
        RGB_SAI, RGB_SAD,  _______, _______, _______,
        RGB_VAI, RGB_VAD,       _______,     _______, _______
    ),
};
// clang-format on

void keyboard_post_init_user(void) {
#ifdef RGBLIGHT_ENABLE
    uint8_t temp_mode = rgblight_get_mode();
    rgblight_enable_noeeprom();
    rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
    for (uint16_t i = 255; i > 0; i--) {
        rgblight_sethsv_noeeprom((i + 128) % 255, 255, 255);
        matrix_scan();
        wait_ms(10);
    }
    led_set_user(host_keyboard_leds());
    rgblight_mode_noeeprom(temp_mode);
#endif
}

bool led_update_user(led_t led_state) {
    if (led_state.num_lock) {
        rgblight_sethsv_noeeprom(HSV_CYAN);
    } else {
        rgblight_sethsv_noeeprom(HSV_MAGENTA);
    }
    return false;
}

bool shutdown_user(bool jump_to_bootloader) {
#ifdef RGBLIGHT_ENABLE
    rgblight_enable_noeeprom();
    rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
    rgblight_setrgb(RGB_RED);
#endif // RGBLIGHT_ENABLE
    return true;
}
