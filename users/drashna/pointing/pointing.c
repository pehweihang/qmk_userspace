// Copyright 2021 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "pointing.h"
#include "math.h"
#include "pointing_device_internal.h"

static uint16_t mouse_debounce_timer = 0;

#ifdef TAPPING_TERM_PER_KEY
#    define TAP_CHECK get_tapping_term(KC_BTN1, NULL)
#else
#    ifndef TAPPING_TERM
#        define TAPPING_TERM 200
#    endif
#    define TAP_CHECK TAPPING_TERM
#endif

#ifndef POINTING_DEVICE_ACCEL_CURVE_A
#    define POINTING_DEVICE_ACCEL_CURVE_A 7 // steepness of accel curve
#endif
#ifndef POINTING_DEVICE_ACCEL_CURVE_B
#    define POINTING_DEVICE_ACCEL_CURVE_B 0.05 // X-offset of accel curve
#endif
#ifndef POINTING_DEVICE_ACCEL_CURVE_C
#    define POINTING_DEVICE_ACCEL_CURVE_C 0.3 // Y-offset of accel curve
#endif
#ifndef POINTING_DEVICE_ACCEL_CURVE_D
#    define POINTING_DEVICE_ACCEL_CURVE_D .1 // speed scaling factor
#endif
#ifndef POINTING_DEVICE_ACCEL_HISTORY_TIME
#    define POINTING_DEVICE_ACCEL_HISTORY_TIME 100 // milliseconds of history to keep
#endif
#define POINTING_DEVICE_ACCEL_ACCUM

static uint32_t     maccel_timer        = 0;
static uint16_t     mouse_jiggler_timer = 0;
static const int8_t deltas[32]          = {0, -1, -2, -2, -3, -3, -4, -4, -4, -4, -3, -3, -2, -2, -1, 0,
                                           0, 1,  2,  2,  3,  3,  4,  4,  4,  4,  3,  3,  2,  2,  1,  0};

static float maccel_a = POINTING_DEVICE_ACCEL_CURVE_A;
static float maccel_b = POINTING_DEVICE_ACCEL_CURVE_B;
static float maccel_c = POINTING_DEVICE_ACCEL_CURVE_C;
static float maccel_d = POINTING_DEVICE_ACCEL_CURVE_D;

static float maccel_accum_x = 0;
static float maccel_accum_y = 0;

__attribute__((weak)) void pointing_device_init_keymap(void) {}

void pointing_device_init_user(void) {
    set_auto_mouse_layer(_MOUSE);
    set_auto_mouse_enable(true);

    pointing_device_init_keymap();
}

__attribute__((weak)) report_mouse_t pointing_device_task_keymap(report_mouse_t mouse_report) {
    return mouse_report;
}

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if (!(mouse_report.x == 0 && mouse_report.y == 0) && (timer_elapsed(mouse_debounce_timer) > TAP_CHECK)) {
        const float speed = maccel_d * (sqrtf(mouse_report.x * mouse_report.x + mouse_report.y * mouse_report.y)) /
                            timer_elapsed32(maccel_timer);
        float scale_factor = 1 - (1 - maccel_c) * expf(-1 * (speed - maccel_b) * maccel_a);
        if (speed <= maccel_b) {
            scale_factor = maccel_c;
        }
        const float x = (mouse_report.x * scale_factor);
        const float y = (mouse_report.y * scale_factor);
        maccel_timer  = timer_read32();

        pd_dprintf("maccel: x: %i, y: %i, speed %f -> factor %f; x': %3f, y': %3f\n", mouse_report.x, mouse_report.y,
                   speed, scale_factor, x, y);

#ifdef POINTING_DEVICE_ACCEL_ACCUM
        // report the integer part
        mouse_report.x = (mouse_xy_report_t)x;
        mouse_report.y = (mouse_xy_report_t)y;

        // accumulate remaining fraction
        maccel_accum_x += (x - mouse_report.x);
        maccel_accum_y += (y - mouse_report.y);

        // pay out accumulated fraction once it's whole
        if (maccel_accum_x >= 1) {
            mouse_report.x += 1;
            maccel_accum_x -= 1;
        } else if (maccel_accum_x <= -1) {
            mouse_report.x -= 1;
            maccel_accum_x += 1;
        }
        if (maccel_accum_y >= 1) {
            mouse_report.y += 1;
            maccel_accum_y -= 1;
        } else if (maccel_accum_y <= -1) {
            mouse_report.y -= 1;
            maccel_accum_y += 1;
        }
        pd_dprintf("maccel accum: x: %3f, y: %3f\n", maccel_accum_x, maccel_accum_y);
#endif // MACCEL_ACCUM
        mouse_report.x = x;
        mouse_report.y = y;
    }
    if (userspace_config.mouse_jiggler && timer_elapsed(mouse_jiggler_timer) > 16) {
        static uint8_t phase = 0;
        mouse_report.x += deltas[phase];
        mouse_report.y += deltas[(phase + 8) & 31];
        phase               = (phase + 1) & 31;
        mouse_jiggler_timer = timer_read();
    }

    return pointing_device_task_keymap(mouse_report);
}

bool process_record_pointing(uint16_t keycode, keyrecord_t* record) {
    switch (keycode) {
        case KC_ACCEL:
            if (record->event.pressed) {
                userspace_config.enable_acceleration ^= 1;
                userspace_config.mouse_jiggler = false;
                eeconfig_update_user_config(&userspace_config.raw);
            }
            break;
        case PD_JIGGLER:
            if (record->event.pressed) {
                mouse_jiggler_timer            = timer_read();
                userspace_config.mouse_jiggler = !userspace_config.mouse_jiggler;
                eeconfig_update_user_config(&userspace_config.raw);
            }
            break;
        default:
            mouse_debounce_timer = timer_read();
            if (userspace_config.mouse_jiggler) {
                userspace_config.mouse_jiggler = false;
                eeconfig_update_user_config(&userspace_config.raw);
            }
            break;
    }
    return true;
}

layer_state_t layer_state_set_pointing(layer_state_t state) {
    if (layer_state_cmp(state, _GAMEPAD) || layer_state_cmp(state, _DIABLO) || layer_state_cmp(state, _DIABLOII)) {
        state |= ((layer_state_t)1 << _MOUSE);
        set_auto_mouse_enable(false); // auto mouse can be disabled any time during run time
    } else {
        set_auto_mouse_enable(true);
    }
    return state;
}

bool has_mouse_report_changed(report_mouse_t* new_report, report_mouse_t* old_report) {
    return ((new_report->buttons != old_report->buttons) || (new_report->x != 0 && new_report->x != old_report->x) ||
            (new_report->y != 0 && new_report->y != old_report->y) ||
            (new_report->h != 0 && new_report->h != old_report->h) ||
            (new_report->v != 0 && new_report->v != old_report->v));
}

#if defined(POINTING_DEVICE_AUTO_MOUSE_ENABLE)
__attribute__((weak)) bool is_mouse_record_keymap(uint16_t keycode, keyrecord_t* record) {
    return false;
}

bool is_mouse_record_user(uint16_t keycode, keyrecord_t* record) {
    if (is_mouse_record_keymap(keycode, record)) {
        return true;
    }
    switch (keycode) {
#    if defined(KEYBOARD_ploopy)
        case DPI_CONFIG:
#    elif (defined(KEYBOARD_bastardkb_charybdis) || defined(KEYBOARD_handwired_tractyl_manuform)) && \
            !defined(NO_CHARYBDIS_KEYCODES) ||                                                       \
        (defined(KEYBOARD_bastardkb_dilemma) && !defined(NO_DILEMMA_KEYCODES))
        case QK_KB ... QK_KB_MAX:
#    endif
        case KC_ACCEL:
        case PD_JIGGLER:
            return true;
    }
    return false;
}
#endif

#ifdef POINTING_MODE_MAP_ENABLE
enum keymap_pointing_mode_ids {
    PM_BROW = POINTING_MODE_MAP_START, // BROWSER TAB Manipulation                      [mode id  6]
    PM_APP,                            // Open App browsing                             [mode id  7]
    POSTMAP_PM_SAFE_RANGE              // To avoid overlap when adding additional modes [mode id  8]
};
// (optional) enum to make things easier to read (index numbers can be used directly)
// Must be in the same order as the above modes
enum keymap_pointing_mode_maps_index {
    _PM_BROW, // first mode map  [index  0]
    _PM_APP   // second mode map [index  1]
};

const uint16_t PROGMEM pointing_mode_maps[POINTING_MODE_MAP_COUNT][POINTING_NUM_DIRECTIONS] = {
    [_PM_BROW] = POINTING_MODE_LAYOUT(C(S(KC_PGUP)), C(S(KC_TAB)), C(KC_TAB), C(S(KC_PGDN))),
    [_PM_APP]  = POINTING_MODE_LAYOUT(KC_NO, A(S(KC_TAB)), A(KC_TAB), KC_NO)};
#endif
