// Copyright 2020 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include QMK_KEYBOARD_H

#include "keyrecords/wrappers.h"
#include "keyrecords/process_records.h"
#include "callbacks.h"
#include "names.h"

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

/* Define layer names */
enum userspace_layers {
    _QWERTY             = 0,
    _NUMLOCK            = 0,
    FIRST_DEFAULT_LAYER = 0,
    _COLEMAK_DH,
    _COLEMAK,
    _DVORAK,
    LAST_DEFAULT_LAYER = _DVORAK,
    _GAMEPAD,
    _DIABLO,
    _DIABLOII,
    _MOUSE,
    _MEDIA,
    _LOWER,
    _RAISE,
    _ADJUST,
    MAX_USER_LAYERS,
};

#define _MACROS          _MOUSE
#define _DEFAULT_LAYER_1 FIRST_DEFAULT_LAYER
#define _DEFAULT_LAYER_2 (FIRST_DEFAULT_LAYER + 1)
#define _DEFAULT_LAYER_3 (FIRST_DEFAULT_LAYER + 2)
#define _DEFAULT_LAYER_4 (FIRST_DEFAULT_LAYER + 3)
#if LAST_DEFAULT_LAYER > (FIRST_DEFAULT_LAYER + 3)
#    define _DEFAULT_LAYER_2 (FIRST_DEFAULT_LAYER + 4)
#    define _DEFAULT_LAYER_3 (FIRST_DEFAULT_LAYER + 5)
#    define _DEFAULT_LAYER_4 (FIRST_DEFAULT_LAYER + 6)
#    define _DEFAULT_LAYER_2 (FIRST_DEFAULT_LAYER + 7)
#    if LAST_DEFAULT_LAYER > (FIRST_DEFAULT_LAYER + 7)
#        define _DEFAULT_LAYER_2 (FIRST_DEFAULT_LAYER + 8)
#        define _DEFAULT_LAYER_3 (FIRST_DEFAULT_LAYER + 9)
#        define _DEFAULT_LAYER_4 (FIRST_DEFAULT_LAYER + 10)
#        define _DEFAULT_LAYER_4 (FIRST_DEFAULT_LAYER + 11)
#    endif // LAST_DEFAULT_LAYER > (FIRST_DEFAULT_LAYER + 7)
#endif     // LAST_DEFAULT_LAYER > (FIRST_DEFAULT_LAYER + 3)

#define DEFAULT_LAYER_1_HSV HSV_CYAN
#define DEFAULT_LAYER_2_HSV HSV_CHARTREUSE
#define DEFAULT_LAYER_3_HSV HSV_MAGENTA
#define DEFAULT_LAYER_4_HSV HSV_GOLDENROD

#define DEFAULT_LAYER_1_RGB RGB_CYAN
#define DEFAULT_LAYER_2_RGB RGB_CHARTREUSE
#define DEFAULT_LAYER_3_RGB RGB_MAGENTA
#define DEFAULT_LAYER_4_RGB RGB_GOLDENROD

bool mod_key_press_timer(uint16_t code, uint16_t mod_code, bool pressed);
bool mod_key_press(uint16_t code, uint16_t mod_code, bool pressed, uint16_t this_timer);
bool hasAllBitsInMask(uint8_t value, uint8_t mask);
void tap_code16_nomods(uint16_t kc);
void format_layer_bitmap_string(char *buffer, layer_state_t state, layer_state_t default_state);
void center_text(const char *text, char *output, uint8_t width);

#if !defined(RGB_MATRIX_ENABLE) && !defined(RGBLIGHT_ENABLE)
#    include <quantum/color.h>
#endif // !defined(RGB_MATRIX_ENABLE) && !defined(RGBLIGHT_ENABLE)

typedef struct PACKED {
    HSV primary;
    HSV secondary;
} dual_hsv_t;

typedef struct PACKED {
    struct {
        bool layer_change : 1;
        bool idle_anim    : 1;
    } rgb;
    struct {
        bool is_overwatch     : 1;
        bool swapped_numbers  : 1;
        bool clap_trap_enable : 1;
        bool song_enable      : 1;
    } gaming;
    struct {
        uint8_t brightness  : 8;
        bool    screen_lock : 1;
    } oled;
    struct {
        bool i2c_scanner_enable : 1;
        bool matrix_scan_print  : 1;
    } debug;
    struct {
        bool enable_acceleration : 1;
        float growth_rate;
        float offset;
        float limit;
        float takeoff;
    } pointing;
    struct {
        uint8_t display_mode : 2;
        uint8_t display_logo : 4;
        dual_hsv_t hsv;
    } painter;
    bool nuke_switch : 1;
    bool check       : 1;
} userspace_config_t;

_Static_assert(sizeof(userspace_config_t) <= EECONFIG_USER_DATA_SIZE, "User EECONFIG block is not large enough.");

extern userspace_config_t userspace_config;

typedef union {
    uint32_t raw;
    struct {
        bool     audio_enable         : 1;
        bool     audio_clicky_enable  : 1;
        bool     tap_toggling         : 1;
        uint8_t  unicode_mode         : 3;
        bool     swap_hands           : 1;
        bool     host_driver_disabled : 1;
        uint8_t  unicode_typing_mode  : 3;
        bool     is_caps_word         : 1;
        uint32_t reserved             : 20;
    };
} user_runtime_config_t;

_Static_assert(sizeof(user_runtime_config_t) == sizeof(uint32_t), "Userspace Runtime config out of spec.");

extern user_runtime_config_t user_runtime_state;

void        set_keyboard_lock(bool enable);
bool        get_keyboard_lock(void);
void        toggle_keyboard_lock(void);
const char *get_layer_name_string(layer_state_t state, bool alt_name, bool is_default);

bool     is_device_suspended(void);
void     set_is_device_suspended(bool status);
bool     is_gaming_layer_active(layer_state_t state);
uint16_t extract_basic_keycode(uint16_t keycode, keyrecord_t *record, bool check_hold);

void     matrix_scan_rate_task(void);
uint32_t get_matrix_scan_rate(void);

#ifdef OS_DETECTION_ENABLE
const char *os_variant_to_string(os_variant_t os);
#endif

#ifdef AUDIO_ENABLE
void set_doom_song(layer_state_t state);
#endif // AUDIO_ENABLE
