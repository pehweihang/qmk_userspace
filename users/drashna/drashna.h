// Copyright 2020 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include QMK_KEYBOARD_H

#include "keyrecords/wrappers.h"
#include "keyrecords/process_records.h"
#include "callbacks.h"
#include "names.h"
#include "user_config.h"
#include "drashna_layers.h"
#include "quantum/unicode/unicode.h"

#if defined(RGBLIGHT_ENABLE)
#    include "rgb/rgb_stuff.h"
#endif // defined(RGBLIGHT_ENABLE)
#if defined(RGB_MATRIX_ENABLE)
#    include "rgb/rgb_matrix_stuff.h"
#endif // defined(RGB_MATRIX_ENABLE)
#if defined(OLED_ENABLE)
#    include "display/oled/oled_stuff.h"
#endif // defined(OLED_ENABLE)
#ifdef SPLIT_KEYBOARD
#    include "split/transport_sync.h"
#endif // SPLIT_KEYBOARD
#ifdef POINTING_DEVICE_ENABLE
#    include "pointing/pointing.h"
#endif // POINTING_DEVICE_ENABLE
#ifdef OS_DETECTION_ENABLE
#    include "os_detection.h"
#endif // OS_DETECTION_ENABLE
#ifdef ORBITAL_MOUSE_ENABLE
#    include "orbital_mouse.h"
#endif // ORBITAL_MOUSE_ENABLE

bool mod_key_press_timer(uint16_t code, uint16_t mod_code, bool pressed);
bool mod_key_press(uint16_t code, uint16_t mod_code, bool pressed, uint16_t this_timer);
bool hasAllBitsInMask(uint8_t value, uint8_t mask);
void tap_code16_nomods(uint16_t kc);
void center_text(const char *text, char *output, uint8_t width);

void        set_keyboard_lock(bool enable);
bool        get_keyboard_lock(void);
void        toggle_keyboard_lock(void);

bool     is_device_suspended(void);
void     set_is_device_suspended(bool status);
uint16_t extract_basic_keycode(uint16_t keycode, keyrecord_t *record, bool check_hold);

void     matrix_scan_rate_task(void);
uint32_t get_matrix_scan_rate(void);

#ifdef OS_DETECTION_ENABLE
const char *os_variant_to_string(os_variant_t os);
#endif

#ifdef AUDIO_ENABLE
void set_doom_song(layer_state_t state);
#endif // AUDIO_ENABLE
