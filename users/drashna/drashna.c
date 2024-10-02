// Copyright 2020 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "drashna.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>

userspace_config_t userspace_config;

/**
 * @brief Handle registering a keycode, with optional modifer based on timed event
 *
 * @param code keycode to send to host
 * @param mod_code modifier to send with code, if held for tapping term or longer
 * @param pressed the press/release event (can use "record->event.pressed" for this)
 * @return true exits function
 * @return false exits function
 */
bool mod_key_press_timer(uint16_t code, uint16_t mod_code, bool pressed) {
    static uint16_t this_timer;
    mod_key_press(code, mod_code, pressed, this_timer);
    return false;
}

/**
 * @brief Handle registation of keycode, with optional modifier based on custom timer
 *
 * @param code keycode to send to host
 * @param mod_code modifier keycode to send with code, if held for tapping term or longer
 * @param pressed the press/release event
 * @param this_timer custom timer to use
 * @return true
 * @return false
 */
bool mod_key_press(uint16_t code, uint16_t mod_code, bool pressed, uint16_t this_timer) {
    if (pressed) {
        this_timer = timer_read();
    } else {
        if (timer_elapsed(this_timer) < TAPPING_TERM) {
            tap_code(code);
        } else {
            register_code(mod_code);
            tap_code(code);
            unregister_code(mod_code);
        }
    }
    return false;
}

/**
 * @brief Performs exact match for modifier values
 *
 * @param value the modifer varible (get_mods/get_oneshot_mods/get_weak_mods)
 * @param mask the modifier mask to check for
 * @return true Has the exact modifiers specifed
 * @return false Does not have the exact modifiers specified
 */
bool hasAllBitsInMask(uint8_t value, uint8_t mask) {
    value &= 0xF;
    mask &= 0xF;

    return (value & mask) == mask;
}

/**
 * @brief Tap keycode, with no mods
 *
 * @param kc keycode to use
 */
void tap_code16_nomods(uint16_t kc) {
    uint8_t temp_mod = get_mods();
    clear_mods();
    clear_oneshot_mods();
    tap_code16(kc);
    set_mods(temp_mod);
}

#if defined(__arm__) && HAL_USE_I2C == TRUE
#    include "i2c_master.h"
#    include "debug.h"

#    ifndef I2C_SCANNER_TIMEOUT
#        define I2C_SCANNER_TIMEOUT 50
#    endif // I2C_SCANNER_TIMEOUT

void do_scan(void) {
    if (!userspace_config.debug.i2c_scanner_enable) {
        return;
    }
    uint8_t nDevices = 0;

    xprintf("Scanning for I2C Devices...\n");

    for (uint8_t address = 1; address < 127; address++) {
        // The i2c_scanner uses the return value of
        // i2c_ping_address to see if a device did acknowledge to the address.
        i2c_status_t error = i2c_ping_address(address << 1, I2C_SCANNER_TIMEOUT);
        if (error == I2C_STATUS_SUCCESS) {
            xprintf("  I2C device found at address 0x%02X\n", address);
            nDevices++;
        } else {
            // xprintf("  Unknown error (%u) at address 0x%02X\n", error, address);
        }
    }

    if (nDevices == 0) {
        xprintf("No I2C devices found\n");
    }
}

uint16_t scan_timer = 0;

void housekeeping_task_i2c_scanner(void) {
    if (timer_elapsed(scan_timer) > 5000) {
        do_scan();
        scan_timer = timer_read();
    }
}

void keyboard_post_init_i2c(void) {
    i2c_init();
    scan_timer = timer_read();
}
#endif // HAL_USE_I2C == TRUE

#if defined(AUTOCORRECT_ENABLE)
#    if defined(AUDIO_ENABLE)
#        ifdef USER_SONG_LIST
float autocorrect_song[][2] = SONG(MARIO_GAMEOVER);
#        else // USER_SONG_LIST
float autocorrect_song[][2] = SONG(PLOVER_GOODBYE_SOUND);
#        endif // USER_SONG_LIST
#    endif
// 2 strings, 2q chars each + null terminator. max autocorrect length is 19 chars but 128px/6 supports 21 chars
char autocorrected_str[2][21]     = {"    automatically\0", "      corrected\0"};
char autocorrected_str_raw[2][21] = {"automatically\0", "corrected\0"};
bool autocorrect_str_has_changed  = false;

bool apply_autocorrect(uint8_t backspaces, const char *str, char *typo, char *correct) {
    if (is_gaming_layer_active(layer_state)) {
        return false;
    }
    strncpy(autocorrected_str_raw[0], typo, sizeof(autocorrected_str_raw[0]) - 1);
    strncpy(autocorrected_str_raw[1], correct, sizeof(autocorrected_str_raw[1]) - 1);

    center_text(typo, autocorrected_str[0], sizeof(autocorrected_str[0]) - 1);
    center_text(correct, autocorrected_str[1], sizeof(autocorrected_str[1]) - 1);
    // printf("Autocorrected %s to %s (original: %s)\n", typo, correct, str);
    autocorrect_str_has_changed = true;
#    if defined(WPM_ENABLE) && defined(WPM_ALLOW_COUNT_REGRESSION)
    for (uint8_t i = 0; i < backspaces; i++) {
        update_wpm(KC_BSPC);
    }
#    endif // WPM_ENABLE

#    if defined(AUDIO_ENABLE)
    audio_play_melody(&autocorrect_song, NOTE_ARRAY_SIZE(autocorrect_song), false);
#    endif // AUDIO_ENABLE
    return true;
}
#endif

#if defined(CAPS_WORD_ENABLE)
bool caps_word_press_user(uint16_t keycode) {
    switch (keycode) {
        // Keycodes that continue Caps Word, with shift applied.
        case KC_MINS:
            if (!keymap_config.swap_lctl_lgui) {
                return true;
            }
        case KC_A ... KC_Z:
            add_weak_mods(MOD_BIT(KC_LSFT)); // Apply shift to next key.
            return true;

        // Keycodes that continue Caps Word, without shifting.
        case KC_1 ... KC_0:
        case KC_BSPC:
        case KC_DEL:
        case KC_UNDS:
            return true;

        default:
            return false; // Deactivate Caps Word.
    }
}

#    if !defined(NO_ACTION_ONESHOT)
void oneshot_locked_mods_changed_user(uint8_t mods) {
    if (mods == MOD_BIT_LSHIFT || mods == MOD_BIT_RSHIFT) {
        del_mods(MOD_MASK_SHIFT);
        set_oneshot_locked_mods(~MOD_MASK_SHIFT & get_oneshot_locked_mods());
        caps_word_on();
    }
}
#    endif // !NO_ACTION_ONESHOT
#endif // CAPS_WORD_ENABLE

#if defined(OS_DETECTION_ENABLE)
typedef struct {
    bool    swap_ctl_gui;
    uint8_t unicode_input_mode;
} os_detection_config_t;

bool process_detected_host_os_user(os_variant_t detected_os) {
    if (is_keyboard_master()) {
        os_detection_config_t os_detection_config = {
            .swap_ctl_gui       = false,
            .unicode_input_mode = UNICODE_MODE_WINCOMPOSE,
        };
        switch (detected_os) {
            case OS_UNSURE:
                xprintf("unknown OS Detected\n");
                break;
            case OS_LINUX:
                xprintf("Linux Detected\n");
                os_detection_config.unicode_input_mode = UNICODE_MODE_LINUX;
                break;
            case OS_WINDOWS:
                xprintf("Windows Detected\n");
                break;
#    if 0
            case OS_WINDOWS_UNSURE:
                xprintf("Windows? Detected\n");
                break;
#    endif
            case OS_MACOS:
                xprintf("MacOS Detected\n");
                os_detection_config = (os_detection_config_t){
                    .swap_ctl_gui       = true,
                    .unicode_input_mode = UNICODE_MODE_MACOS,
                };
                break;
            case OS_IOS:
                xprintf("iOS Detected\n");
                os_detection_config = (os_detection_config_t){
                    .swap_ctl_gui       = true,
                    .unicode_input_mode = UNICODE_MODE_MACOS,
                };
                break;
#    if 0
            case OS_PS5:
                xprintf("PlayStation 5 Detected\n");
                os_detection_config.unicode_input_mode = UNICODE_MODE_LINUX;
                break;
            case OS_HANDHELD:
                xprintf("Nintend Switch/Quest 2 Detected\n");
                os_detection_config.unicode_input_mode = UNICODE_MODE_LINUX;
                break;
#    endif
            default:
                xprintf("Unknown OS Detected\n");
                break;
        }
        keymap_config.swap_lctl_lgui = keymap_config.swap_rctl_rgui = os_detection_config.swap_ctl_gui;
#    ifdef UNICODE_COMMON_ENABLE
        set_unicode_input_mode_soft(os_detection_config.unicode_input_mode);
#    endif // UNICODE_COMMON_ENABLE
    }

    return true;
}
#endif // OS_DETECTION_ENABLE

static host_driver_t *host_driver          = 0;
static bool           host_driver_disabled = false;

/**
 * @brief Mount/Umount the keyboard USB driver
 */
void set_keyboard_lock(bool status) {
    if (!status && !host_get_driver()) {
        host_set_driver(host_driver);
    } else if (status && host_get_driver()) {
        host_driver = host_get_driver();
        clear_keyboard();
        host_set_driver(0);
    } else if (status) {
        clear_keyboard();
    }

    host_driver_disabled = status;
}

/**
 * @brief Toggles the keyboard lock status
 *
 */
void toggle_keyboard_lock(void) {
    set_keyboard_lock(!host_driver_disabled);
}

/**
 * @brief Get the keyboard lock status
 *
 * @return true
 * @return false
 */
bool get_keyboard_lock(void) {
    return host_driver_disabled;
}

/**
 * @brief hack for snprintf warning
 *
 */
#define snprintf_nowarn(...) (snprintf(__VA_ARGS__) < 0 ? abort() : (void)0)

/**
 * @brief Center text in a string. Useful for monospaced font rendering such as oled display feature.
 *
 * @param text pointer to input string to center
 * @param output pointer to output string to write to
 * @param width total width of the output string
 */
void center_text(const char *text, char *output, uint8_t width) {
    /* If string is bigger than the available width, trim it */
    if (strlen(text) > width) {
        memcpy(output, text, width);
        return;
    }

    /* Try to center the TEXT, TODO: Handle Even lengths*/
    uint8_t padlen_l = (width - strlen(text)) / 2;
    uint8_t padlen_r = (padlen_l * 2) + strlen(text) == width ? padlen_l : padlen_l + 1;
    snprintf_nowarn(output, width, "%*s%s%*s", padlen_l, " ", text, padlen_r, " ");
}

/**
 * @brief Checks to see if device is suspended
 *
 * @return true
 * @return false
 */
bool is_device_suspended(void) {
    return user_runtime_state.internals.is_device_suspended;
}

/**
 * @brief Sets the local variable for device suspended
 *
 * @param status
 */
void set_is_device_suspended(bool status) {
#if defined(SPLIT_KEYBOARD) && defined(SPLIT_TRANSACTION_IDS_USER)
    send_device_suspend_state(status);
#endif
}

/**
 * @brief Grabs the basic keycode from a quantum keycode
 *
 * @param keycode extended keycode to extract from
 * @param record keyrecord used for checking tap count/hold
 * @param check_hold do we check for hold keycode status
 * @return uint16_t returns the basic keycode
 */
uint16_t extract_basic_keycode(uint16_t keycode, keyrecord_t *record, bool check_hold) {
    if (IS_QK_MOD_TAP(keycode)) {
        if (record->tap.count || !check_hold) {
            keycode = keycode_config(QK_MOD_TAP_GET_TAP_KEYCODE(keycode));
        } else {
            keycode = keycode_config(0xE0 + biton(QK_MOD_TAP_GET_MODS(keycode) & 0xF) +
                                     biton(QK_MOD_TAP_GET_MODS(keycode) & 0x10));
        }
    } else if (IS_QK_LAYER_TAP(keycode) && (record->tap.count || !check_hold)) {
        keycode = keycode_config(QK_LAYER_TAP_GET_TAP_KEYCODE(keycode));
    } else if (IS_QK_MODS(keycode)) {
        keycode = keycode_config(QK_MODS_GET_BASIC_KEYCODE(keycode));
    } else if (IS_QK_ONE_SHOT_MOD(keycode)) {
        keycode = keycode_config(0xE0 + biton(QK_ONE_SHOT_MOD_GET_MODS(keycode) & 0xF) +
                                 biton(QK_ONE_SHOT_MOD_GET_MODS(keycode) & 0x10));
    } else if (IS_QK_BASIC(keycode)) {
        keycode = keycode_config(keycode);
    }

    return keycode;
}

static uint32_t last_matrix_scan_count = 0;
/**
 * @brief Get the matrix scan rate value
 *
 * @return uint32_t scans per second
 */
uint32_t get_matrix_scan_rate(void) {
    return last_matrix_scan_count;
}

/**
 * @brief Task to monitor and print the matrix scan rate
 */
void matrix_scan_rate_task(void) {
    static uint32_t matrix_timer      = 0;
    static uint32_t matrix_scan_count = 0;

    matrix_scan_count++;

    if (timer_elapsed32(matrix_timer) >= 1000) {
#ifndef NO_PRINT
        if (userspace_config.debug.matrix_scan_print) {
            xprintf("matrix scan frequency: %lu\n", matrix_scan_count);
        }
#endif // NO_PRINT
        last_matrix_scan_count = matrix_scan_count;
        matrix_timer           = timer_read32();
        matrix_scan_count      = 0;
    }
}

#ifdef OS_DETECTION_ENABLE
const char *os_variant_to_string(os_variant_t os_detected) {
    switch (os_detected) {
        case OS_WINDOWS:
            return "Windows";
#    if 0
        case OS_WINDOWS_UNSURE:
            return "Windows?";
#    endif
        case OS_MACOS:
            return "MacOS";
        case OS_IOS:
            return "iOS";
#    if 0
        case OS_PS5:
            return "PS5";
        case OS_HANDHELD:
            return "Handheld";
#    endif
        case OS_LINUX:
            return "Linux";
        default:
            return "Unknown";
    }
}
#endif // OS_DETECTION_ENABLE

#ifdef AUDIO_ENABLE
float doom_song[][2] = SONG(E1M1_DOOM);
extern audio_config_t audio_config;

void set_doom_song(layer_state_t state) {
    static bool is_gaming_layer_active = false, is_doom_song_active = false;

    if (userspace_config.gaming.song_enable != is_doom_song_active ||
        is_gaming_layer_active != layer_state_cmp(state, _GAMEPAD)) {
        is_doom_song_active    = userspace_config.gaming.song_enable;
        is_gaming_layer_active = layer_state_cmp(state, _GAMEPAD);
        if (is_gaming_layer_active && is_doom_song_active) {
            PLAY_LOOP(doom_song);
            audio_config.clicky_enable = false;
        } else {
            audio_stop_all();
            audio_config.raw = eeconfig_read_audio();
        }
    }
}

#endif // AUDIO_ENABLE

#if defined(HAPTIC_ENABLE) && defined(HAPTIC_DRV2605L)
/**
 * @brief Get the name of the DRV2605L effect
 *
 * @param effect the effect to get the name of
 * @return const char* the name of the effect
 */
const char *get_haptic_drv2605l_effect_name(drv2605l_effect_t effect) {
    switch (effect) {
        case DRV2605L_EFFECT_CLEAR_SEQUENCE:
            return "Clear Sequence";
        case DRV2605L_EFFECT_STRONG_CLICK_100:
            return "Strong Click 100%";
        case DRV2605L_EFFECT_STRONG_CLICK_60:
            return "Strong Click 60%";
        case DRV2605L_EFFECT_STRONG_CLICK_30:
            return "Strong Click 30%";
        case DRV2605L_EFFECT_SHARP_CLICK_100:
            return "Sharp Click 100%";
        case DRV2605L_EFFECT_SHARP_CLICK_60:
            return "Sharp Click 60%";
        case DRV2605L_EFFECT_SHARP_CLICK_30:
            return "Sharp Click 30%";
        case DRV2605L_EFFECT_SOFT_BUMP_100:
            return "Soft Bump 100%";
        case DRV2605L_EFFECT_SOFT_BUMP_60:
            return "Soft Bump 60%";
        case DRV2605L_EFFECT_SOFT_BUMP_30:
            return "Soft Bump 30%";
        case DRV2605L_EFFECT_DOUBLE_CLICK_100:
            return "Double Click 100%";
        case DRV2605L_EFFECT_DOUBLE_CLICK_60:
            return "Double Click 60%";
        case DRV2605L_EFFECT_TRIPLE_CLICK_100:
            return "Triple Click 100%";
        case DRV2605L_EFFECT_SOFT_FUZZ_60:
            return "Soft Fuzz 60%";
        case DRV2605L_EFFECT_STRONG_BUZZ_100:
            return "Strong Buzz 100%";
        case DRV2605L_EFFECT_750_MS_ALERT_100:
            return "750ms Alert 100%";
        case DRV2605L_EFFECT_1000_MS_ALERT_100:
            return "1000ms Alert 100%";
        case DRV2605L_EFFECT_STRONG_CLICK_1_100:
            return "Strong Click 1 100%";
        case DRV2605L_EFFECT_STRONG_CLICK_2_80:
            return "Strong Click 2 80%";
        case DRV2605L_EFFECT_STRONG_CLICK_3_60:
            return "Strong Click 3 60%";
        case DRV2605L_EFFECT_STRONG_CLICK_4_30:
            return "Strong Click 4 30%";
        case DRV2605L_EFFECT_MEDIUM_CLICK_1_100:
            return "Medium Click 1 100%";
        case DRV2605L_EFFECT_MEDIUM_CLICK_2_80:
            return "Medium Click 2 80%";
        case DRV2605L_EFFECT_MEDIUM_CLICK_3_60:
            return "Medium Click 3 60%";
        case DRV2605L_EFFECT_SHARP_TICK_1_100:
            return "Sharp Tick 1 100%";
        case DRV2605L_EFFECT_SHARP_TICK_2_80:
            return "Sharp Tick 2 80%";
        case DRV2605L_EFFECT_SHARP_TICK_3_60:
            return "Sharp Tick 3 60%";
        case DRV2605L_EFFECT_SHORT_DOUBLE_CLICK_STRONG_1_100:
            return "Short Double Click Strong 1 100%";
        case DRV2605L_EFFECT_SHORT_DOUBLE_CLICK_STRONG_2_80:
            return "Short Double Click Strong 2 80%";
        case DRV2605L_EFFECT_SHORT_DOUBLE_CLICK_STRONG_3_60:
            return "Short Double Click Strong 3 60%";
        case DRV2605L_EFFECT_SHORT_DOUBLE_CLICK_STRONG_4_30:
            return "Short Double Click Strong 4 30%";
        case DRV2605L_EFFECT_SHORT_DOUBLE_CLICK_MEDIUM_1_100:
            return "Short Double Click Medium 1 100%";
        case DRV2605L_EFFECT_SHORT_DOUBLE_CLICK_MEDIUM_2_80:
            return "Short Double Click Medium 2 80%";
        case DRV2605L_EFFECT_SHORT_DOUBLE_CLICK_MEDIUM_3_60:
            return "Short Double Click Medium 3 60%";
        case DRV2605L_EFFECT_SHORT_DOUBLE_SHARP_TICK_1_100:
            return "Short Double Sharp Tick 1 100%";
        case DRV2605L_EFFECT_SHORT_DOUBLE_SHARP_TICK_2_80:
            return "Short Double Sharp Tick 2 80%";
        case DRV2605L_EFFECT_SHORT_DOUBLE_SHARP_TICK_3_60:
            return "Short Double Sharp Tick 3 60%";
        case DRV2605L_EFFECT_LONG_DOUBLE_SHARP_CLICK_STRONG_1_100:
            return "Long Double Sharp Click Strong 1 100%";
        case DRV2605L_EFFECT_LONG_DOUBLE_SHARP_CLICK_STRONG_2_80:
            return "Long Double Sharp Click Strong 2 80%";
        case DRV2605L_EFFECT_LONG_DOUBLE_SHARP_CLICK_STRONG_3_60:
            return "Long Double Sharp Click Strong 3 60%";
        case DRV2605L_EFFECT_LONG_DOUBLE_SHARP_CLICK_STRONG_4_30:
            return "Long Double Sharp Click Strong 4 30%";
        case DRV2605L_EFFECT_LONG_DOUBLE_SHARP_CLICK_MEDIUM_1_100:
            return "Long Double Sharp Click Medium 1 100%";
        case DRV2605L_EFFECT_LONG_DOUBLE_SHARP_CLICK_MEDIUM_2_80:
            return "Long Double Sharp Click Medium 2 80%";
        case DRV2605L_EFFECT_LONG_DOUBLE_SHARP_CLICK_MEDIUM_3_60:
            return "Long Double Sharp Click Medium 3 60%";
        case DRV2605L_EFFECT_LONG_DOUBLE_SHARP_TICK_1_100:
            return "Long Double Sharp Tick 1 100%";
        case DRV2605L_EFFECT_LONG_DOUBLE_SHARP_TICK_2_80:
            return "Long Double Sharp Tick 2 80%";
        case DRV2605L_EFFECT_LONG_DOUBLE_SHARP_TICK_3_60:
            return "Long Double Sharp Tick 3 60%";
        case DRV2605L_EFFECT_BUZZ_1_100:
            return "Buzz 1 100%";
        case DRV2605L_EFFECT_BUZZ_2_80:
            return "Buzz 2 80%";
        case DRV2605L_EFFECT_BUZZ_3_60:
            return "Buzz 3 60%";
        case DRV2605L_EFFECT_BUZZ_4_40:
            return "Buzz 4 40%";
        case DRV2605L_EFFECT_BUZZ_5_20:
            return "Buzz 5 20%";
        case DRV2605L_EFFECT_PULSING_STRONG_1_100:
            return "Pulsing Strong 1 100%";
        case DRV2605L_EFFECT_PULSING_STRONG_2_60:
            return "Pulsing Strong 2 60%";
        case DRV2605L_EFFECT_PULSING_MEDIUM_1_100:
            return "Pulsing Medium 1 100%";
        case DRV2605L_EFFECT_PULSING_MEDIUM_2_60:
            return "Pulsing Medium 2 60%";
        case DRV2605L_EFFECT_PULSING_SHARP_1_100:
            return "Pulsing Sharp 1 100%";
        case DRV2605L_EFFECT_PULSING_SHARP_2_60:
            return "Pulsing Sharp 2 60%";
        case DRV2605L_EFFECT_TRANSITION_CLICK_1_100:
            return "Transition Click 1 100%";
        case DRV2605L_EFFECT_TRANSITION_CLICK_2_80:
            return "Transition Click 2 80%";
        case DRV2605L_EFFECT_TRANSITION_CLICK_3_60:
            return "Transition Click 3 60%";
        case DRV2605L_EFFECT_TRANSITION_CLICK_4_40:
            return "Transition Click 4 40%";
        case DRV2605L_EFFECT_TRANSITION_CLICK_5_20:
            return "Transition Click 5 20%";
        case DRV2605L_EFFECT_TRANSITION_CLICK_6_10:
            return "Transition Click 6 10%";
        case DRV2605L_EFFECT_TRANSITION_HUM_1_100:
            return "Transition Hum 1 100%";
        case DRV2605L_EFFECT_TRANSITION_HUM_2_80:
            return "Transition Hum 2 80%";
        case DRV2605L_EFFECT_TRANSITION_HUM_3_60:
            return "Transition Hum 3 60%";
        case DRV2605L_EFFECT_TRANSITION_HUM_4_40:
            return "Transition Hum 4 40%";
        case DRV2605L_EFFECT_TRANSITION_HUM_5_20:
            return "Transition Hum 5 20%";
        case DRV2605L_EFFECT_TRANSITION_HUM_6_10:
            return "Transition Hum 6 10%";
        case DRV2605L_EFFECT_TRANSITION_RAMP_DOWN_LONG_SMOOTH_1_100:
            return "Transition Ramp Down Long Smooth 1 100%";
        case DRV2605L_EFFECT_TRANSITION_RAMP_DOWN_LONG_SMOOTH_2_100:
            return "Transition Ramp Down Long Smooth 2 100%";
        case DRV2605L_EFFECT_TRANSITION_RAMP_DOWN_MEDIUM_SMOOTH_1_100:
            return "Transition Ramp Down Medium Smooth 1 100%";
        case DRV2605L_EFFECT_TRANSITION_RAMP_DOWN_MEDIUM_SMOOTH_2_100:
            return "Transition Ramp Down Medium Smooth 2 100%";
        case DRV2605L_EFFECT_TRANSITION_RAMP_DOWN_SHORT_SMOOTH_1_100:
            return "Transition Ramp Down Short Smooth 1 100%";
        case DRV2605L_EFFECT_TRANSITION_RAMP_DOWN_SHORT_SMOOTH_2_100:
            return "Transition Ramp Down Short Smooth 2 100%";
        case DRV2605L_EFFECT_TRANSITION_RAMP_DOWN_LONG_SHARP_1_100:
            return "Transition Ramp Down Long Sharp 1 100%";
        case DRV2605L_EFFECT_TRANSITION_RAMP_DOWN_LONG_SHARP_2_100:
            return "Transition Ramp Down Long Sharp 2 100%";
        case DRV2605L_EFFECT_TRANSITION_RAMP_DOWN_MEDIUM_SHARP_1_100:
            return "Transition Ramp Down Medium Sharp 1 100%";
        case DRV2605L_EFFECT_TRANSITION_RAMP_DOWN_MEDIUM_SHARP_2_100:
            return "Transition Ramp Down Medium Sharp 2 100%";
        case DRV2605L_EFFECT_TRANSITION_RAMP_DOWN_SHORT_SHARP_1_100:
            return "Transition Ramp Down Short Sharp 1 100%";
        case DRV2605L_EFFECT_TRANSITION_RAMP_DOWN_SHORT_SHARP_2_100:
            return "Transition Ramp Down Short Sharp 2 100%";
        case DRV2605L_EFFECT_TRANSITION_RAMP_UP_LONG_SMOOTH_1_100:
            return "Transition Ramp Up Long Smooth 1 100%";
        case DRV2605L_EFFECT_TRANSITION_RAMP_UP_LONG_SMOOTH_2_100:
            return "Transition Ramp Up Long Smooth 2 100%";
        case DRV2605L_EFFECT_TRANSITION_RAMP_UP_MEDIUM_SMOOTH_1_100:
            return "Transition Ramp Up Medium Smooth 1 100%";
        case DRV2605L_EFFECT_TRANSITION_RAMP_UP_MEDIUM_SMOOTH_2_100:
            return "Transition Ramp Up Medium Smooth 2 100%";
        case DRV2605L_EFFECT_TRANSITION_RAMP_UP_SHORT_SMOOTH_1_100:
            return "Transition Ramp Up Short Smooth 1 100%";
        case DRV2605L_EFFECT_TRANSITION_RAMP_UP_SHORT_SMOOTH_2_100:
            return "Transition Ramp Up Short Smooth 2 100%";
        case DRV2605L_EFFECT_TRANSITION_RAMP_UP_LONG_SHARP_1_100:
            return "Transition Ramp Up Long Sharp 1 100%";
        case DRV2605L_EFFECT_TRANSITION_RAMP_UP_LONG_SHARP_2_100:
            return "Transition Ramp Up Long Sharp 2 100%";
        case DRV2605L_EFFECT_TRANSITION_RAMP_UP_MEDIUM_SHARP_1_100:
            return "Transition Ramp Up Medium Sharp 1 100%";
        case DRV2605L_EFFECT_TRANSITION_RAMP_UP_MEDIUM_SHARP_2_100:
            return "Transition Ramp Up Medium Sharp 2 100%";
        case DRV2605L_EFFECT_TRANSITION_RAMP_UP_SHORT_SHARP_1_100:
            return "Transition Ramp Up Short Sharp 1 100%";
        case DRV2605L_EFFECT_TRANSITION_RAMP_UP_SHORT_SHARP_2_100:
            return "Transition Ramp Up Short Sharp 2 100%";
        case DRV2605L_EFFECT_TRANSITION_RAMP_DOWN_LONG_SMOOTH_1_50:
            return "Transition Ramp Down Long Smooth 1 50%";
        case DRV2605L_EFFECT_TRANSITION_RAMP_DOWN_LONG_SMOOTH_2_50:
            return "Transition Ramp Down Long Smooth 2 50%";
        case DRV2605L_EFFECT_TRANSITION_RAMP_DOWN_MEDIUM_SMOOTH_1_50:
            return "Transition Ramp Down Medium Smooth 1 50%";
        case DRV2605L_EFFECT_TRANSITION_RAMP_DOWN_MEDIUM_SMOOTH_2_50:
            return "Transition Ramp Down Medium Smooth 2 50%";
        case DRV2605L_EFFECT_TRANSITION_RAMP_DOWN_SHORT_SMOOTH_1_50:
            return "Transition Ramp Down Short Smooth 1 50%";
        case DRV2605L_EFFECT_TRANSITION_RAMP_DOWN_SHORT_SMOOTH_2_50:
            return "Transition Ramp Down Short Smooth 2 50%";
        case DRV2605L_EFFECT_TRANSITION_RAMP_DOWN_LONG_SHARP_1_50:
            return "Transition Ramp Down Long Sharp 1 50%";
        case DRV2605L_EFFECT_TRANSITION_RAMP_DOWN_LONG_SHARP_2_50:
            return "Transition Ramp Down Long Sharp 2 50%";
        case DRV2605L_EFFECT_TRANSITION_RAMP_DOWN_MEDIUM_SHARP_1_50:
            return "Transition Ramp Down Medium Sharp 1 50%";
        case DRV2605L_EFFECT_TRANSITION_RAMP_DOWN_MEDIUM_SHARP_2_50:
            return "Transition Ramp Down Medium Sharp 2 50%";
        case DRV2605L_EFFECT_TRANSITION_RAMP_DOWN_SHORT_SHARP_1_50:
            return "Transition Ramp Down Short Sharp 1 50%";
        case DRV2605L_EFFECT_TRANSITION_RAMP_DOWN_SHORT_SHARP_2_50:
            return "Transition Ramp Down Short Sharp 2 50%";
        case DRV2605L_EFFECT_TRANSITION_RAMP_UP_LONG_SMOOTH_1_50:
            return "Transition Ramp Up Long Smooth 1 50%";
        case DRV2605L_EFFECT_TRANSITION_RAMP_UP_LONG_SMOOTH_2_50:
            return "Transition Ramp Up Long Smooth 2 50%";
        case DRV2605L_EFFECT_TRANSITION_RAMP_UP_MEDIUM_SMOOTH_1_50:
            return "Transition Ramp Up Medium Smooth 1 50%";
        case DRV2605L_EFFECT_TRANSITION_RAMP_UP_MEDIUM_SMOOTH_2_50:
            return "Transition Ramp Up Medium Smooth 2 50%";
        case DRV2605L_EFFECT_TRANSITION_RAMP_UP_SHORT_SMOOTH_1_50:
            return "Transition Ramp Up Short Smooth 1 50%";
        case DRV2605L_EFFECT_TRANSITION_RAMP_UP_SHORT_SMOOTH_2_50:
            return "Transition Ramp Up Short Smooth 2 50%";
        case DRV2605L_EFFECT_TRANSITION_RAMP_UP_LONG_SHARP_1_50:
            return "Transition Ramp Up Long Sharp 1 50%";
        case DRV2605L_EFFECT_TRANSITION_RAMP_UP_LONG_SHARP_2_50:
            return "Transition Ramp Up Long Sharp 2 50%";
        case DRV2605L_EFFECT_TRANSITION_RAMP_UP_MEDIUM_SHARP_1_50:
            return "Transition Ramp Up Medium Sharp 1 50%";
        case DRV2605L_EFFECT_TRANSITION_RAMP_UP_MEDIUM_SHARP_2_50:
            return "Transition Ramp Up Medium Sharp 2 50%";
        case DRV2605L_EFFECT_TRANSITION_RAMP_UP_SHORT_SHARP_1_50:
            return "Transition Ramp Up Short Sharp 1 50%";
        case DRV2605L_EFFECT_TRANSITION_RAMP_UP_SHORT_SHARP_2_50:
            return "Transition Ramp Up Short Sharp 2 50%";
        case DRV2605L_EFFECT_LONG_BUZZ_FOR_PROGRAMMATIC_STOPPING:
            return "Long Buzz for Programmatic Stopping";
        case DRV2605L_EFFECT_SMOOTH_HUM_1_50:
            return "Smooth Hum 1 50%";
        case DRV2605L_EFFECT_SMOOTH_HUM_2_40:
            return "Smooth Hum 2 40%";
        case DRV2605L_EFFECT_SMOOTH_HUM_3_30:
            return "Smooth Hum 3 30%";
        case DRV2605L_EFFECT_SMOOTH_HUM_4_20:
            return "Smooth Hum 4 20%";
        case DRV2605L_EFFECT_SMOOTH_HUM_5_10:
            return "Smooth Hum 5 10%";
        case DRV2605L_EFFECT_COUNT:
            return "Unknown";
    }
    return "Unknown";
}
#endif // HAPTIC_ENABLE && HAPTIC_DRV2605L

#if 0
https://github.com/libopencm3/libopencm3/blob/master/lib/stm32/common/desig_common_all.c
#    include "hardware_id.h"
void get_serial_number(void) {
    uint8_t uid[12];
    for (uint8_t index = 0; index < 12; index++) {
        uid[index] = (uint8_t)(get_hardware_id().data[index / 4] >> ((index % 4) * 8));
    }

    uint16_t serial[6];
    serial[0] = uid[11];
    serial[1] = uid[10] + uid[2];
    serial[2] = uid[9];
    serial[3] = uid[8] + uid[0];
    serial[4] = uid[7];
    serial[5] = uid[6];

    xprintf("%02X%02X%02X%02X%02X%02X", serial[0], serial[1], serial[2], serial[3], serial[4], serial[5]);
}
#endif
