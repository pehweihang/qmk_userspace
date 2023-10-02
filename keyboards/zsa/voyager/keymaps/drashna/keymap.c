// Copyright 2023 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "drashna.h"

// clang-format off
#define LAYOUT_wrapper(...) LAYOUT(__VA_ARGS__)
#define LAYOUT_base( \
    K01, K02, K03, K04, K05, K06, K07, K08, K09, K0A, \
    K11, K12, K13, K14, K15, K16, K17, K18, K19, K1A, K1B, \
    K21, K22, K23, K24, K25, K26, K27, K28, K29, K2A  \
  ) \
  LAYOUT_wrapper( \
     KC_ESC,  ________________NUMBER_LEFT________________,            ________________NUMBER_RIGHT_______________, UC_CLUE, \
     SH_TT,   K01,    K02,      K03,     K04,     K05,                K06,     K07,     K08,     K09,     K0A,     SH_TT, \
     LALT_T(KC_TAB), K11, K12,  K13,     K14,     K15,                K16,     K17,     K18,     K19,     K1A,     RALT_T(K1B), \
     OS_LSFT,CTL_T(K21),ALT_T(K22),GUI_T(K23),K24,K25,                K26,K27,RGUI_T(K28),RALT_T(K29),RCTL_T(K2A), OS_RSFT, \
                                         KC_SPC,  BK_LWER,            DL_RAIS, KC_ENT \
    )

#define LAYOUT_base_wrapper(...)       LAYOUT_base(__VA_ARGS__)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_DEFAULT_LAYER_1] = LAYOUT_base_wrapper(
        _________________QWERTY_L1_________________, _________________QWERTY_R1_________________,
        _________________QWERTY_L2_________________, _________________QWERTY_R2_________________,
        _________________QWERTY_L3_________________, _________________QWERTY_R3_________________
    ),

    [_DEFAULT_LAYER_2] = LAYOUT_base_wrapper(
        ______________COLEMAK_MOD_DH_L1____________, ______________COLEMAK_MOD_DH_R1____________,
        ______________COLEMAK_MOD_DH_L2____________, ______________COLEMAK_MOD_DH_R2____________,
        ______________COLEMAK_MOD_DH_L3____________, ______________COLEMAK_MOD_DH_R3____________
    ),
    [_DEFAULT_LAYER_3] = LAYOUT_base_wrapper(
        _________________COLEMAK_L1________________, _________________COLEMAK_R1________________,
        _________________COLEMAK_L2________________, _________________COLEMAK_R2________________,
        _________________COLEMAK_L3________________, _________________COLEMAK_R3________________
    ),

    [_DEFAULT_LAYER_4] = LAYOUT_base_wrapper(
        _________________DVORAK_L1_________________, _________________DVORAK_R1_________________,
        _________________DVORAK_L2_________________, _________________DVORAK_R2_________________,
        _________________DVORAK_L3_________________, _________________DVORAK_R3_________________
    ),

    [_GAMEPAD] = LAYOUT(
        KC_ESC,  KC_NO,   KC_1,    KC_2,    KC_3,    KC_4,       _______, _______, _______, _______, _______, _______,
        KC_F1,   KC_K,    KC_Q,    KC_W,    KC_E,    KC_R,       _______, _______, _______, _______, _______, _______,
        KC_TAB,  KC_G,    KC_A,    KC_S,    KC_D,    KC_F,       _______, _______, _______, _______, _______, _______,
        KC_LCTL, KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_H,       _______, _______, _______, _______, _______, _______,
                                            KC_V,    KC_SPC,     _______, TG_GAME
    ),
    [_DIABLO] = LAYOUT(
        KC_ESC,  KC_V,    KC_D,    KC_LALT, KC_NO,   KC_NO,      KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_NO,   KC_NO,
        KC_TAB,  KC_S,    KC_I,    KC_F,    KC_M,    KC_T,       _______, _______, _______, _______, _______, _______,
        KC_Q,    KC_1,    KC_2,    KC_3,    KC_4,    KC_G,       _______, _______, _______, _______, _______, _______,
        KC_LCTL, KC_D3_1, KC_D3_2, KC_D3_3, KC_D3_4, KC_Z,       _______, _______, _______, _______, _______, _______,
                                           KC_LSFT, KC_LCTL,      _______, TG_DBLO
    ),
    [_LOWER] = LAYOUT_wrapper(
        KC_F12,  _________________FUNC_LEFT_________________,    _________________FUNC_RIGHT________________, KC_F11,
        _______, _________________LOWER_L1__________________,    _________________LOWER_R1__________________, _______,
        _______, _________________LOWER_L2__________________,    _________________LOWER_R2__________________, KC_PIPE,
        _______, _________________LOWER_L3__________________,    _________________LOWER_R3__________________, _______,
                                            _______, _______,    _______, _______
    ),

    [_RAISE] = LAYOUT_wrapper(
        KC_F12,  _________________FUNC_LEFT_________________,    _________________FUNC_RIGHT________________, KC_F11,
        KC_GRV,  _________________RAISE_L1__________________,    _________________RAISE_R1__________________, _______,
        _______, _________________RAISE_L2__________________,    _________________RAISE_R2__________________, KC_BSLS,
        _______, _________________RAISE_L3__________________,    _________________RAISE_R3__________________, _______,
                                            _______, _______,    _______, _______
    ),

    [_ADJUST] = LAYOUT_wrapper(
        QK_MAKE, KC_WIDE,KC_AUSSIE,KC_SCRIPT,KC_ZALGO,KC_SUPER, KC_NOMODE,KC_COMIC,KC_REGIONAL,TG_GAME,TG_DBLO, QK_BOOT,
        VRSN,    _________________ADJUST_L1_________________,    _________________ADJUST_R1_________________, EE_CLR,
        KEYLOCK, _________________ADJUST_L2_________________,    _________________ADJUST_R2_________________, TG_MODS,
        UC_NEXT, _________________ADJUST_L3_________________,    _________________ADJUST_R3_________________, KC_MPLY,
                                            QK_RBT,  _______,    _______, KC_NUKE
    )
};
// clang-format on
