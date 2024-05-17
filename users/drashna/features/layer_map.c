// Copyright 2023 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "keymap_common.h"
#include "action_layer.h"
#include "print.h"

volatile uint16_t layer_map[MATRIX_ROWS][MATRIX_COLS] = {0};
static bool       layer_map_set                       = false;

void set_layer_map(void) {
    layer_map_set = true;
}

void populate_layer_map(void) {
    // xprintf("Layer map set\n\n");
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        for (uint8_t j = 0; j < MATRIX_COLS; j++) {
            keypos_t key    = {j, i};
            layer_map[i][j] = keymap_key_to_keycode(layer_switch_get_layer(key), key);
            // xprintf("0x%04x, ", layer_map[i][j]);
        }
        // xprintf("\n");
    }
}

void housekeeping_task_layer_map(void) {
    if (layer_map_set) {
        populate_layer_map();
        layer_map_set = false;
    }
}

#ifdef VIA_ENABLE
#    include "via.h"

bool via_command_kb(uint8_t *data, uint8_t length) {
    switch (data[0]) {
        case id_dynamic_keymap_set_keycode:
        case id_dynamic_keymap_reset:
        case id_dynamic_keymap_set_buffer:
        case id_dynamic_keymap_set_encoder:
            layer_map_set = true;
    }
    return false;
}
#endif
