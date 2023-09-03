/**
 * Copyright 2021 Charly Delay <charly@codesink.dev> (@0xcharly)
 * Copyright 2023 casuanoob <casuanoob@hotmail.com> (@casuanoob)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "drashna.h"

// clang-format off
#define LAYOUT_wrapper(...) LAYOUT(__VA_ARGS__)
#define LAYOUT_base( \
    K01, K02, K03, K04, K05, K06, K07, K08, K09, K0A, \
    K11, K12, K13, K14, K15, K16, K17, K18, K19, K1A, K1B, \
    K21, K22, K23, K24, K25, K26, K27, K28, K29, K2A  \
    ) \
    LAYOUT_wrapper ( \
     KC_ESC,  ________________NUMBER_LEFT________________,            ________________NUMBER_RIGHT_______________, UC_CLUE, \
     SH_TT,   K01,    K02,      K03,     K04,     K05,                K06,     K07,     K08,     K09,     K0A,     SH_TT, \
     LALT_T(KC_TAB), K11, K12,  K13,     K14,     K15,                K16,     K17,     K18,     K19,     K1A,     RALT_T(K1B), \
     OS_LSFT,CTL_T(K21),ALT_T(K22),GUI_T(K23),K24,K25,                K26,K27,RGUI_T(K28),RALT_T(K29),RCTL_T(K2A), OS_RSFT, \
                        KC_NO,   KC_GRV,  KC_SPC,  BK_LWER,    DL_RAIS, RSFT_T(KC_ENT), KC_RALT, KC_MUTE \
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
                          _______, KC_V,    KC_SPC,  KC_H,       _______, _______, _______, TG_GAME
    ),
    [_DIABLO] = LAYOUT(
        KC_ESC,  KC_V,    KC_D,    KC_LALT, KC_NO,   KC_NO,      KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_NO,   KC_NO,
        KC_TAB,  KC_S,    KC_I,    KC_F,    KC_M,    KC_T,       _______, _______, _______, _______, _______, _______,
        KC_Q,    KC_1,    KC_2,    KC_3,    KC_4,    KC_G,       _______, _______, _______, _______, _______, _______,
        KC_LCTL, KC_D3_1, KC_D3_2, KC_D3_3, KC_D3_4, KC_Z,       _______, _______, _______, _______, _______, _______,
                          _______, KC_G,    KC_LSFT, KC_LCTL,    _______, _______, TO(_DIABLOII), TG_DBLO
    ),
    [_DIABLOII] = LAYOUT(
        KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_GRV,     _______, _______, _______, _______, _______, _______,
        KC_TAB,  KC_A,    KC_T,    KC_Q,    KC_I,    KC_M,       _______, _______, _______, _______, _______, _______,
        KC_S,    KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,      _______, _______, _______, _______, _______, _______,
        KC_LCTL, KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,     _______, _______, _______, _______, _______, _______,
                  _______, KC_DIABLO_CLEAR, _______, _______,    _______, _______, _______, TG(_DIABLOII)
    ),
    [_MOUSE] = LAYOUT(
        _______, _______, _______, _______, _______, _______,   _______, DPI_RMOD,DPI_MOD, S_D_RMOD,S_D_MOD, PD_JIGGLER,
        _______, _______, _______, _______, _______, _______,   KC_WH_U, _______, _______, _______, _______, DRGSCRL,
        _______, _______, _______, _______, _______, _______,   KC_WH_D, KC_BTN1, KC_BTN3, KC_BTN2, KC_BTN6, SNIPING,
        _______, _______, _______, _______, _______, _______,   KC_BTN7, KC_BTN4, KC_BTN5, KC_BTN8, _______, _______,
                          _______, _______, SNIPING, _______,   _______, _______, _______, _______
    ),
    [_LOWER] = LAYOUT_wrapper(
        KC_F12,  _________________FUNC_LEFT_________________,    _________________FUNC_RIGHT________________, KC_F11,
        _______, _________________LOWER_L1__________________,    _________________LOWER_R1__________________, _______,
        _______, _________________LOWER_L2__________________,    _________________LOWER_R2__________________, KC_PIPE,
        _______, _________________LOWER_L3__________________,    _________________LOWER_R3__________________, _______,
                          _______, _______, _______, _______,    _______, AUTO_CTN, _______, _______
    ),
    [_RAISE] = LAYOUT_wrapper(
        KC_F12,  _________________FUNC_LEFT_________________,    _________________FUNC_RIGHT________________, KC_F11,
        KC_GRV,  _________________RAISE_L1__________________,    _________________RAISE_R1__________________, _______,
        _______, _________________RAISE_L2__________________,    _________________RAISE_R2__________________, KC_BSLS,
        _______, _________________RAISE_L3__________________,    _________________RAISE_R3__________________, _______,
                          _______, _______, _______, _______,    _______, _______, _______, _______
    ),
    [_ADJUST] = LAYOUT_wrapper(
        QK_MAKE, KC_WIDE,KC_AUSSIE,KC_SCRIPT,KC_ZALGO,KC_SUPER, KC_NOMODE,KC_COMIC,KC_REGIONAL,TG_GAME,TG_DBLO, QK_BOOT,
        VRSN,    _________________ADJUST_L1_________________,    _________________ADJUST_R1_________________, EE_CLR,
        KEYLOCK, _________________ADJUST_L2_________________,    _________________ADJUST_R2_________________, TG_MODS,
        UC_NEXT, _________________ADJUST_L3_________________,    _________________ADJUST_R3_________________, KC_MPLY,
                          _______, _______, KC_NUKE, _______,    _______, QK_BOOT, _______, _______
    ),
};


#ifdef ENCODER_MAP_ENABLE
// clang-format off
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [_DEFAULT_LAYER_1] = { ENCODER_CCW_CW( KC_VOLD, KC_VOLU ), ENCODER_CCW_CW( KC_WH_D, KC_WH_U ) },
    [_DEFAULT_LAYER_2] = { ENCODER_CCW_CW( _______, _______ ), ENCODER_CCW_CW( _______, _______ ) },
    [_DEFAULT_LAYER_3] = { ENCODER_CCW_CW( _______, _______ ), ENCODER_CCW_CW( _______, _______ ) },
    [_DEFAULT_LAYER_4] = { ENCODER_CCW_CW( _______, _______ ), ENCODER_CCW_CW( _______, _______ ) },
    [_GAMEPAD]         = { ENCODER_CCW_CW( _______, _______ ), ENCODER_CCW_CW( _______, _______ ) },
    [_DIABLO]          = { ENCODER_CCW_CW( _______, _______ ), ENCODER_CCW_CW( _______, _______ ) },
    [_MOUSE]           = { ENCODER_CCW_CW( _______, _______ ), ENCODER_CCW_CW( _______, _______ ) },
    [_MEDIA]           = { ENCODER_CCW_CW( _______, _______ ), ENCODER_CCW_CW( _______, _______ ) },
    [_RAISE]           = { ENCODER_CCW_CW( OL_BINC, OL_BDEC ), ENCODER_CCW_CW( KC_PGDN, KC_PGUP ) },
    [_LOWER]           = { ENCODER_CCW_CW( RGB_MOD, RGB_RMOD), ENCODER_CCW_CW( RGB_HUI, RGB_HUD ) },
    [_ADJUST]          = { ENCODER_CCW_CW( CK_UP,   CK_DOWN ), ENCODER_CCW_CW( _______, _______ ) },
};
// clang-format on
#endif // ENCODER_MAP_ENABLE

#if defined(OLED_ENABLE) && defined(OLED_DISPLAY_128X128)
#    ifdef UNICODE_COMMON_ENABLE
#        include "process_unicode_common.h"
#        include "keyrecords/unicode.h"
#    endif

extern const char PROGMEM display_border[3];

extern bool is_oled_enabled;


bool oled_task_keymap(void) {
    static const char PROGMEM header_image[] = {
        0, 192, 32, 16, 8, 4, 2, 1, 1, 1, 1, 1, 1, 1, 1, 3, 7, 15, 31, 63, 127, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 127, 63, 31, 15, 7, 3, 1, 1, 1, 1, 1, 1, 1, 1, 2, 4, 8, 16, 32, 192, 0,
        //         0,255,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  3,  7, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,  7,  3,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,255,  0
    };
    oled_write_raw_P(header_image, sizeof(header_image));
    oled_set_cursor(4, 0);
    oled_write_P(PSTR(" Dilemma Max"), true);

    render_default_layer_state(1, 1);
    render_layer_state(1, 2);
    render_pet(0, 5);
    render_wpm(1, 7, 5);
#   if defined(POINTING_DEVICE_ENABLE)
    render_pointing_dpi_status(dilemma_get_pointer_sniping_enabled() ? dilemma_get_pointer_sniping_dpi() : dilemma_get_pointer_default_dpi(), 1, 7, 6);
    render_mouse_mode(17, 6);
#    else
    render_matrix_scan_rate(1, 7, 6);
#    endif
    render_bootmagic_status(7, 7);
    render_user_status(1, 9);

    render_mod_status(get_mods() | get_oneshot_mods(), 1, 10);
    render_keylock_status(host_keyboard_led_state(), 1, 11);
    render_unicode_mode(1, 12);

//    render_rgb_hsv(1, 13);
    oled_set_cursor(1, 13);
    oled_write_P(PSTR("OS: "), false);
    os_variant_t os_type = detected_host_os();
        switch (os_type) {
            case OS_LINUX:
                oled_write_ln_P(PSTR("Linux"), false);
                break;
            case OS_WINDOWS:
                oled_write_ln_P(PSTR("Windows"), false);
                break;
            case OS_MACOS:
                oled_write_ln_P(PSTR("MacOS"), false);
                break;
            case OS_IOS:
                oled_write_ln_P(PSTR("iOS"), false);
                break;
            default:
                break;
        }

    render_keylogger_status(1, 14);

    for (uint8_t i = 1; i < 15; i++) {
        oled_set_cursor(0, i);
        oled_write_raw_P(display_border, sizeof(display_border));
        oled_set_cursor(21, i);
        oled_write_raw_P(display_border, sizeof(display_border));
    }

    static const char PROGMEM footer_image[] = {0, 3, 4, 8, 16, 32, 64, 128, 128, 128, 128, 128, 128, 128, 192, 224, 240, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 248, 240, 224, 192, 128, 128, 128, 128, 128, 128, 128, 64, 32, 16, 8, 4, 3, 0};
    oled_set_cursor(0, 15);
    oled_write_raw_P(footer_image, sizeof(footer_image));

    return false;
}
#endif


#ifdef SWAP_HANDS_ENABLE
const keypos_t PROGMEM hand_swap_config[MATRIX_ROWS][MATRIX_COLS] = {
    /* Left hand, matrix positions */
    {{0, 5}, {1, 5}, {2, 5}, {3, 5}, {4, 5}, {5, 5}},
    {{0, 6}, {1, 6}, {2, 6}, {3, 6}, {4, 6}, {5, 6}},
    {{0, 7}, {1, 7}, {2, 7}, {3, 7}, {4, 7}, {5, 7}},
    {{0, 8}, {1, 8}, {2, 8}, {3, 8}, {4, 8}, {5, 8}},
    {{0, 9}, {1, 9}, {2, 9}, {3, 9}, {4, 9}, {5, 9}},
    /* Right hand, matrix positions */
    {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}},
    {{0, 1}, {1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1}},
    {{0, 2}, {1, 2}, {2, 2}, {3, 2}, {4, 2}, {5, 2}},
    {{0, 3}, {1, 3}, {2, 3}, {3, 3}, {4, 3}, {5, 3}},
    {{0, 4}, {1, 4}, {2, 4}, {3, 4}, {4, 4}, {5, 4}}
};

#    ifdef ENCODER_MAP_ENABLE
const uint8_t PROGMEM encoder_hand_swap_config[NUM_ENCODERS] = {1, 0};
#    endif
#endif
