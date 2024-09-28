// Copyright 2020 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "drashna_layers.h"

/**
 * @brief Generates a string of the layer state bitmask
 *
 * @param buffer char string buffer to write to
 * @param state layer state bitmask
 * @param default_state default layer state bitmask (so we can represent default layer differently)
 */
void format_layer_bitmap_string(char *buffer, layer_state_t state, layer_state_t default_state) {
    for (uint8_t i = 0; i < 16; i++) {
        if (i == 0 || i == 4 || i == 8 || i == 12) {
            *buffer = ' ';
            ++buffer;
        }

        uint8_t layer = i;
        if ((default_state & ((layer_state_t)1 << layer)) != 0) {
            *buffer = 'D';
        } else if ((state & ((layer_state_t)1 << layer)) != 0) {
            *buffer = '1';
        } else {
            *buffer = '_';
        }
        ++buffer;
    }
    *buffer = 0;
}

/**
 * @brief Get the layer name string object
 *
 * @param state layer state bitmask
 * @param alt_name Use altname?
 * @param is_default do we want the default layer's name?
 * @return const char* Layer name in string format
 */
const char *get_layer_name_string(layer_state_t state, bool alt_name, bool is_default) {
    switch (get_highest_layer(state)) {
        case _QWERTY:
            return alt_name ? "Num Pad" : is_default ? "QWERTY" : "Base";
        case _COLEMAK:
            return "Colemak";
        case _COLEMAK_DH:
            return "Colemak-DH";
        case _DVORAK:
            return "Dvorak";
        case _GAMEPAD:
            return "Gamepad";
        case _DIABLO:
            return "Diablo";
        case _DIABLOII:
            return "Diablo II";
        case _MOUSE:
            return alt_name ? "Macros" : "Mouse";
        case _MEDIA:
            return "Media";
        case _LOWER:
            return "Lower";
        case _RAISE:
            return "Raise";
        case _ADJUST:
            return "Adjust";
        default:
            return "Unknown";
    }
}

/**
 * @brief Checks to see if one or more gaming layers are active
 *
 * @param state layer state bitmask
 * @return true A gaming layer is active
 * @return false No gaming layers active
 */

bool is_gaming_layer_active(layer_state_t state) {
    return (state & (1 << _GAMEPAD)) || (state & (1 << _DIABLO)) || (state & (1 << _DIABLOII));
}
