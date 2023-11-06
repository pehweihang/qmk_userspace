// Copyright 2023 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include QMK_KEYBOARD_H

enum local_layers {
    _QWERTY,
    _RGB,
    _FUNC,
};

// Defines for task manager and such
#define CALTDEL LCTL(LALT(KC_DEL))
#define TSKMGR LCTL(LSFT(KC_ESC))

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    /* Qwerty
     * ,-------------.
     * |   1  |  2   |
     * |------+------|
     * |   3  |  4   |
     * |------+------|
     * |   5  |  6   |
     * |------+------|
     * | FUNC | RGB  |
     * `-------------'
     */
    [_QWERTY] = LAYOUT(
        KC_1,      KC_2,
        KC_3,      KC_4,
        KC_5,      KC_6,
        MO(_FUNC), TG(_RGB)
      ),

    /* RGB
     * ,-------------.
     * | Mode-| Mode+|
     * |------+------|
     * | HUE- | HUE+ |
     * |------+------|
     * | SAT- | SAT+ |
     * |------+------|
     * |RGBTOG|      |
     * `-------------'
     */
    [_RGB] = LAYOUT(
        RGB_RMOD, RGB_MOD,
        RGB_HUD,  RGB_HUI,
        RGB_SAD,  RGB_SAI,
        RGB_TOG,  KC_TRNS
      ),

    /* Function
     * ,-------------.
     * |   Q  |CALDEL|
     * |------+------|
     * |   A  |TSKMGR|
     * |------+------|
     * |   Z  |  X   |
     * |------+------|
     * |      |  C   |
     * `-------------'
     */
    [_FUNC] = LAYOUT(
        KC_Q,    CALTDEL,
        KC_A,    TSKMGR,
        KC_Z,    KC_X,
        _______, QK_BOOT
      )

};

#ifdef RGB_MATRIX_ENABLE
  led_config_t g_led_config = {
  {
    {  7,  0 },
    {  6,  1 },
    {  5,  2 },
    {  4,  3 },
  },{
    { 121,   2 }, { 121,  23 },
    { 121,  41 }, { 121,  60 },
    { 103,   2 }, { 103,  23 },
    { 103,  41 }, { 103,  60 },
  },{
    1, 1, 1, 1,
    1, 1, 1, 1,
  }
  };
#endif
// clang-format on
