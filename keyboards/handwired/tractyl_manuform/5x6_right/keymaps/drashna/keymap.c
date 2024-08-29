// Copyright 2023 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "drashna.h"

#if defined(KEYBOARD_handwired_tractyl_manuform_5x6_right_f405)
#    include "keymap_full.h"
#else // KEYBOARD_handwired_tractyl_manuform_5x6_right_f405
#    include "keymap_regular.h"
#endif // KEYBOARD_handwired_tractyl_manuform_5x6_right_f405

// clang-format off
#ifdef LAYER_MAP_ENABLE
keypos_t layer_remap[LAYER_MAP_ROWS][LAYER_MAP_COLS] = {
    { {   0,   0 }, {   1,   0 }, {   2,   0 }, {   3,   0 }, {   4,   0 }, {   5,   0 }, { 255, 255 }, { 255, 255 }, { 255, 255 }, { 255, 255 }, { 255, 255 }, {   0,   6 }, {   1,   6 }, {   2,   6 }, {   3,   6 }, {   4,   6 }, {   5,   6 } },
    { {   0,   1 }, {   1,   1 }, {   2,   1 }, {   3,   1 }, {   4,   1 }, {   5,   1 }, { 255, 255 }, { 255, 255 }, { 255, 255 }, { 255, 255 }, { 255, 255 }, {   0,   7 }, {   1,   7 }, {   2,   7 }, {   3,   7 }, {   4,   7 }, {   5,   7 } },
    { {   0,   2 }, {   1,   2 }, {   2,   2 }, {   3,   2 }, {   4,   2 }, {   5,   2 }, { 255, 255 }, { 255, 255 }, { 255, 255 }, { 255, 255 }, { 255, 255 }, {   0,   8 }, {   1,   8 }, {   2,   8 }, {   3,   8 }, {   4,   8 }, {   5,   8 } },
    { {   0,   3 }, {   1,   3 }, {   2,   3 }, {   3,   3 }, {   4,   3 }, {   5,   3 }, { 255, 255 }, { 255, 255 }, { 255, 255 }, { 255, 255 }, { 255, 255 }, {   0,   9 }, {   1,   9 }, {   2,   9 }, {   3,   9 }, {   4,   9 }, {   5,   9 } },
    { {   0,   4 }, {   1,   4 }, {   2,   4 }, {   3,   4 }, { 255, 255 }, { 255, 255 }, { 255, 255 }, { 255, 255 }, { 255, 255 }, { 255, 255 }, { 255, 255 }, { 255, 255 }, { 255, 255 }, {   2,  10 }, {   3,  10 }, {   4,  10 }, {   5,  10 } },
    { { 255, 255 }, { 255, 255 }, {   0, 252 }, { 255, 255 }, {   4,   4 }, {   5,   4 }, { 255, 255 }, { 255, 255 }, { 255, 255 }, { 255, 255 }, { 255, 255 }, { 255, 255 }, {   1,  10 }, { 255, 255 }, {   1, 252 }, { 255, 255 }, { 255, 255 } },
    { { 255, 255 }, { 255, 255 }, {   0, 253 }, { 255, 255 }, { 255, 255 }, { 255, 255 }, {   5,   5 }, {   3,   5 }, { 255, 255 }, {   2,  11 }, { 255, 255 }, { 255, 255 }, { 255, 255 }, { 255, 255 }, {   1, 253 }, { 255, 255 }, { 255, 255 } },
    { { 255, 255 }, { 255, 255 }, { 255, 255 }, { 255, 255 }, { 255, 255 }, { 255, 255 }, {   4,   5 }, {   2,   5 }, { 255, 255 }, { 255, 255 }, {   3,  11 }, {   1,  11 }, { 255, 255 }, { 255, 255 }, { 255, 255 }, { 255, 255 }, { 255, 255 } },
};
#endif

#ifdef ENCODER_MAP_ENABLE
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [_DEFAULT_LAYER_1] = { ENCODER_CCW_CW( KC_VOLU, KC_VOLD ), ENCODER_CCW_CW( KC_WH_U, KC_WH_D ) },
    [_DEFAULT_LAYER_2] = { ENCODER_CCW_CW( _______, _______ ), ENCODER_CCW_CW( _______, _______ ) },
    [_DEFAULT_LAYER_3] = { ENCODER_CCW_CW( _______, _______ ), ENCODER_CCW_CW( _______, _______ ) },
    [_DEFAULT_LAYER_4] = { ENCODER_CCW_CW( _______, _______ ), ENCODER_CCW_CW( _______, _______ ) },
    [_GAMEPAD]         = { ENCODER_CCW_CW( _______, _______ ), ENCODER_CCW_CW( _______, _______ ) },
    [_DIABLO]          = { ENCODER_CCW_CW( _______, _______ ), ENCODER_CCW_CW( _______, _______ ) },
    [_MOUSE]           = { ENCODER_CCW_CW( _______, _______ ), ENCODER_CCW_CW( _______, _______ ) },
    [_MEDIA]           = { ENCODER_CCW_CW( _______, _______ ), ENCODER_CCW_CW( _______, _______ ) },
    [_RAISE]           = { ENCODER_CCW_CW( OL_BINC, OL_BDEC ), ENCODER_CCW_CW( OL_CCW,  OL_CW   ) },
    [_LOWER]           = { ENCODER_CCW_CW( RGB_MOD, RGB_RMOD), ENCODER_CCW_CW( RGB_HUI, RGB_HUD ) },
    [_ADJUST]          = { ENCODER_CCW_CW( CK_UP,   CK_DOWN ), ENCODER_CCW_CW( KC_PGDN, KC_PGUP ) },
};
#endif
// clang-format on

#ifdef OLED_ENABLE
oled_rotation_t oled_init_keymap(oled_rotation_t rotation, bool has_run) {
    return has_run ? rotation : OLED_ROTATION_180;
}

void render_oled_title(bool side) {
    oled_write_P(side ? PSTR("   Tractyl   ") : PSTR("   Manuform  "), true);
}
#endif

#if defined(RGB_MATRIX_ENABLE) && defined(RGBLIGHT_ENABLE) && defined(RGBLIGHT_CUSTOM)
const uint8_t led_mapping[RGBLIGHT_LED_COUNT] = {0,  1,  2,  3,  32, 31, 12, 13, 26, 30, 29, 27,
                                                 28, 59, 61, 60, 46, 45, 62, 63, 36, 35, 34, 33};
#endif

#if defined(QUANTUM_PAINTER_ENABLE)
#    if defined(CUSTOM_QUANTUM_PAINTER_ENABLE)
#        include "qp_ili9xxx_opcodes.h"
#        include "qp_comms.h"
void init_display_ili9341_inversion(painter_device_t display) {
    qp_comms_start(display);
    qp_comms_command(display, is_keyboard_left() ? ILI9XXX_CMD_INVERT_OFF : ILI9XXX_CMD_INVERT_OFF);
    qp_comms_stop(display);
}
#    else // CUSTOM_QUANTUM_PAINTER_ENABLE
#        include "qp.h"
#        include "display/painter/graphics/assets.h"

static painter_device_t       display;
static painter_image_handle_t my_image;

void keyboard_post_init_keymap(void) {
    display =
        qp_ili9341_make_spi_device(240, 320, DISPLAY_CS_PIN, DISPLAY_DC_PIN, DISPLAY_RST_PIN, DISPLAY_SPI_DIVIDER, 0);
    qp_init(display, QP_ROTATION_180);
    qp_clear(display);
    qp_power(display, true);
    if (is_keyboard_master()) {
        my_image = qp_load_image_mem(gfx_anime_girl_jacket_240x320);
        qp_drawimage_recolor(display, 0, 0, my_image, true ? 213 : 0, 0, 255, 0, 0, 0);
    } else {
        my_image = qp_load_image_mem(gfx_samurai_cyberpunk_minimal_dark_8k_b3_240x320);
        qp_drawimage(display, 0, 0, my_image);
    }
    qp_flush(display);
    // my_image = qp_load_image_mem(gfx_neon_genesis_evangelion_initial_machine_02_240x320);
    // my_image = qp_load_image_mem(gfx_asuka_240x320);
    qp_close_image(my_image);
}
#    endif // CUSTOM_QUANTUM_PAINTER_ENABLE
#endif     // QUANTUM_PAINTER_ENABLE
