// Copyright 2023 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "keymap_common.h"
#include "action_layer.h"
#include "print.h"

volatile uint16_t layer_map[MATRIX_ROWS][MATRIX_COLS] = {0};

void set_layer_map(void) {
    //    xprintf("Layer map set\n\n");
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        for (uint8_t j = 0; j < MATRIX_COLS; j++) {
            keypos_t key    = {j, i};
            layer_map[i][j] = keymap_key_to_keycode(layer_switch_get_layer(key), key);
            //            xprintf("0x%04x, ", layer_map[i][j]);
        }
        //        xprintf("\n");
    }
}
