// Copyright 2021 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "drashna.h"

#ifdef TAPPING_TERM_PER_KEY
__attribute__((weak)) uint16_t get_tapping_term_keymap(uint16_t keycode, keyrecord_t *record) {
    return TAPPING_TERM;
}

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    uint16_t keymap_tapping_term = get_tapping_term_keymap(keycode, record);
    if (keymap_tapping_term != TAPPING_TERM) {
        return keymap_tapping_term;
    }
    switch (keycode) {
        case BK_LWER:
            return TAPPING_TERM + 25;
        case QK_MOD_TAP ... QK_MOD_TAP_MAX:
            if (QK_MOD_TAP_GET_MODS(keycode) & MOD_LGUI) {
                return 300;
            }
        default:
            return TAPPING_TERM;
    }
}
#endif // TAPPING_TERM_PER_KEY

#ifdef PERMISSIVE_HOLD_PER_KEY
__attribute__((weak)) bool get_permissive_hold_keymap(uint16_t keycode, keyrecord_t *record) {
    return false;
}

bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    // Immediately select the hold action when another key is tapped:
    // return true;
    // Do not select the hold action when another key is tapped.
    // return false;
    switch (keycode) {
        default:
            return get_permissive_hold_keymap(keycode, record);
    }
}
#endif // PERMISSIVE_HOLD_PER_KEY

#ifdef HOLD_ON_OTHER_KEY_PRESS_PER_KEY
__attribute__((weak)) bool get_hold_on_other_key_press_keymap(uint16_t keycode, keyrecord_t *record) {
    return false;
}

bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    // Immediately select the hold action when another key is pressed.
    // return true;
    // Do not select the hold action when another key is pressed.
    // return false;
    switch (keycode) {
            //        case QK_LAYER_TAP ... QK_LAYER_TAP_MAX:
            //            return true;
        default:
            return get_hold_on_other_key_press_keymap(keycode, record);
    }
}
#endif // HOLD_ON_OTHER_KEY_PRESS_PER_KEY

#ifdef QUICK_TAP_TERM_PER_KEY
__attribute__((weak)) uint16_t get_quick_tap_term_keymap(uint16_t keycode, keyrecord_t *record) {
    return QUICK_TAP_TERM;
}

uint16_t get_quick_tap_term(uint16_t keycode, keyrecord_t *record) {
    uint16_t keymap_quick_tap_term = get_quick_tap_term_keymap(keycode, record);
    if (keymap_quick_tap_term != QUICK_TAP_TERM) {
        return keymap_quick_tap_term;
    }

    switch (keycode) {
        default:
            return QUICK_TAP_TERM;
    }
}
#endif // QUICK_TAP_TERM_PER_KEY

#ifdef RETRO_TAPPING_PER_KEY
__attribute__((weak)) bool get_retro_tapping_keymap(uint16_t keycode, keyrecord_t *record) {
    return false;
}

bool get_retro_tapping(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        default:
            return get_retro_tapping_keymap(keycode, record);
    }
}
#endif // RETRO_TAPPING_PER_KEY

bool achordion_chord(uint16_t tap_hold_keycode, keyrecord_t *tap_hold_record, uint16_t other_keycode, keyrecord_t *other_record) {
    // Exceptionally consider the following chords as holds, even though they
    // are on the same hand in Dvorak.
    switch (tap_hold_keycode) {
        case QK_LAYER_TAP ... QK_LAYER_TAP_MAX:
        return true;
       case HOME_A: // A + U.
            if (other_keycode == HOME_U) {
                return true;
            }
            break;

        case HOME_S: // S + H and S + G.
            if (other_keycode == HOME_H || other_keycode == KC_G) {
                return true;
            }
            break;
    }

    // Also allow same-hand holds when the other key is in the rows below the
    // alphas. I need the `% (MATRIX_ROWS / 2)` because my keyboard is split.
    if (other_record->event.key.row % (MATRIX_ROWS / 2) >= 4) {
        return true;
    }

    // Otherwise, follow the opposite hands rule.
    return achordion_opposite_hands(tap_hold_record, other_record);
}
