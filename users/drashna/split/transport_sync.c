// Copyright 2020 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "transport_sync.h"
#include "_wait.h"
#include "drashna.h"
#include "transactions.h"
#include <string.h>
#include "split_util.h"

#ifdef UNICODE_COMMON_ENABLE
#    include "process_unicode_common.h"
extern unicode_config_t unicode_config;
#    include "keyrecords/unicode.h"
#endif
#ifdef AUDIO_ENABLE
#    include "audio.h"
extern audio_config_t audio_config;
extern bool           delayed_tasks_run;
#endif
#ifdef SWAP_HANDS_ENABLE
extern bool swap_hands;
#endif
#ifdef DISPLAY_DRIVER_ENABLE
#    include "display/display.h"
#endif // DISPLAY_DRIVER_ENABLEj
#ifndef FORCED_SYNC_THROTTLE_MS
#    define FORCED_SYNC_THROTTLE_MS 100
#endif // FORCED_SYNC_THROTTLE_MS

// Make sure that the structs are not larger than the buffer size for synchronization
_Static_assert(sizeof(userspace_config_t) <= RPC_M2S_BUFFER_SIZE,
               "userspace_config_t is larger than split buffer size!");
_Static_assert(sizeof(user_runtime_config_t) <= RPC_M2S_BUFFER_SIZE,
               "user_runtime_config_t is larger than split buffer size!");

/**
 * @brief Syncs user state between halves of split keyboard
 *
 * @param initiator2target_buffer_size
 * @param initiator2target_buffer
 * @param target2initiator_buffer_size
 * @param target2initiator_buffer
 */
void user_runtime_state_sync(uint8_t initiator2target_buffer_size, const void* initiator2target_buffer,
                             uint8_t target2initiator_buffer_size, void* target2initiator_buffer) {
    if (initiator2target_buffer_size == sizeof(user_runtime_state)) {
        memcpy(&user_runtime_state, initiator2target_buffer, initiator2target_buffer_size);
        static bool suspend_state = false;
        if (user_runtime_state.internals.is_device_suspended != suspend_state) {
            suspend_state = user_runtime_state.internals.is_device_suspended;
            set_is_device_suspended(suspend_state);
        }
    }
}

/**
 * @brief Syncs userspace_config between halves of split keyboard
 *
 * @param initiator2target_buffer_size
 * @param initiator2target_buffer
 * @param target2initiator_buffer_size
 * @param target2initiator_buffer
 */
void user_config_sync(uint8_t initiator2target_buffer_size, const void* initiator2target_buffer,
                      uint8_t target2initiator_buffer_size, void* target2initiator_buffer) {
    if (initiator2target_buffer_size == sizeof(userspace_config)) {
        memcpy(&userspace_config, initiator2target_buffer, initiator2target_buffer_size);
    }
}

#if defined(AUTOCORRECT_ENABLE)
extern char autocorrected_str[2][22];
_Static_assert(sizeof(autocorrected_str) <= RPC_M2S_BUFFER_SIZE, "Autocorrect array larger than buffer size!");
#endif
/**
 * @brief Sycn Autoccetion string between halves of split keyboard
 *
 * @param initiator2target_buffer_size
 * @param initiator2target_buffer
 * @param target2initiator_buffer_size
 * @param target2initiator_buffer
 */
void autocorrect_string_sync(uint8_t initiator2target_buffer_size, const void* initiator2target_buffer,
                             uint8_t target2initiator_buffer_size, void* target2initiator_buffer) {
#if defined(AUTOCORRECT_ENABLE)
    if (initiator2target_buffer_size == (sizeof(autocorrected_str))) {
        memcpy(&autocorrected_str, initiator2target_buffer, initiator2target_buffer_size);
    }
#endif
}

/**
 * @brief Sync keylogger string between halves of split keyboard
 *
 * @param initiator2target_buffer_size
 * @param initiator2target_buffer
 * @param target2initiator_buffer_size
 * @param target2initiator_buffer
 */
void keylogger_string_sync(uint8_t initiator2target_buffer_size, const void* initiator2target_buffer,
                           uint8_t target2initiator_buffer_size, void* target2initiator_buffer) {
#if defined(DISPLAY_DRIVER_ENABLE) && defined(DISPLAY_KEYLOGGER_ENABLE)
    if (initiator2target_buffer_size == (DISPLAY_KEYLOGGER_LENGTH + 1)) {
        memcpy(&display_keylogger_string, initiator2target_buffer, initiator2target_buffer_size);
    }
#endif // DISPLAY_DRIVER_ENABLE && DISPLAY_KEYLOGGER_ENABLE
}

/**
 * @brief Send the suspend state to the other half of the split keyboard
 *
 * @param status
 */
void send_device_suspend_state(bool status) {
    if (is_device_suspended() != status && is_keyboard_master()) {
        user_runtime_state.internals.is_device_suspended = status;
        transaction_rpc_send(RPC_ID_USER_RUNTIME_STATE_SYNC, sizeof(user_runtime_state), &user_runtime_state);
        wait_ms(5);
    }
}

/**
 * @brief Initialize the transport sync
 *
 */
void keyboard_post_init_transport_sync(void) {
    // Register keyboard state sync split transaction
    transaction_register_rpc(RPC_ID_USER_RUNTIME_STATE_SYNC, user_runtime_state_sync);
    transaction_register_rpc(RPC_ID_USER_CONFIG_SYNC, user_config_sync);
    transaction_register_rpc(RPC_ID_USER_AUTOCORRECT_STR, autocorrect_string_sync);
    transaction_register_rpc(RPC_ID_USER_DISPLAY_KEYLOG_STR, keylogger_string_sync);
}

/**
 * @brief Updates config values for split syncing
 *
 */
void user_transport_update(void) {
    if (is_keyboard_master()) {
        user_runtime_state.internals.host_driver_disabled = get_keyboard_lock();
#if defined(POINTING_DEVICE_ENABLE) && defined(POINTING_DEVICE_AUTO_MOUSE_ENABLE)
        user_runtime_state.internals.tap_toggling = get_auto_mouse_toggle();
#endif // POINTING_DEVICE_ENABLE && POINTING_DEVICE_AUTO_MOUSE_ENABLE
#ifdef UNICODE_COMMON_ENABLE
        user_runtime_state.unicode.mode = get_unicode_input_mode();
#endif // UNICODE_COMMON_ENABLE
#ifdef SWAP_HANDS_ENABLE
        user_runtime_state.internals.swap_hands = swap_hands;
#endif // SWAP_HANDS_ENABLE
#ifdef CAPS_WORD_ENABLE
        user_runtime_state.internals.is_caps_word = is_caps_word_on();
#endif // CAPS_WORD_ENABLE
        user_runtime_state.mods = (sync_mods_t){
            .mods      = get_mods(),
            .weak_mods = get_weak_mods(),
#ifndef NO_ACTION_ONESHOT
            .oneshot_mods        = get_oneshot_mods(),
            .oneshot_locked_mods = get_oneshot_locked_mods(),
#endif // NO_ACTION_ONESHOT
        };
        user_runtime_state.layers = (sync_layer_t){
            .layer_state         = layer_state,
            .default_layer_state = default_layer_state,
        };
        user_runtime_state.leds = host_keyboard_led_state();
#ifdef WPM_ENABLE
        user_runtime_state.wpm_count = get_current_wpm();
#endif // WPM_ENABLE
        user_runtime_state.activity = (sync_activity_t){
            .matrix_timestamp          = last_matrix_activity_time(),
            .encoder_timestamp         = last_encoder_activity_time(),
            .pointing_device_timestamp = last_pointing_device_activity_time(),
        };
        user_runtime_state.keymap_config = keymap_config;
    } else {
#ifdef UNICODE_COMMON_ENABLE
        unicode_config.input_mode = user_runtime_state.unicode.mode;
#endif // UNICODE_COMMON_ENABLE
#if defined(POINTING_DEVICE_ENABLE) && defined(POINTING_DEVICE_AUTO_MOUSE_ENABLE)
        if (get_auto_mouse_toggle() != user_runtime_state.internals.tap_toggling) {
            auto_mouse_toggle();
        }
#endif // POINTING_DEVICE_ENABLE && POINTING_DEVICE_AUTO_MOUSE_ENABLE
#ifdef SWAP_HANDS_ENABLE
        swap_hands = user_runtime_state.internals.swap_hands;
#endif // SWAP_HANDS_ENABLE
#ifdef CAPS_WORD_ENABLE
        if (user_runtime_state.internals.is_caps_word) {
            caps_word_on();
        } else {
            caps_word_off();
        }
#endif // CAPS_WORD_ENABLE
        set_keyboard_lock(user_runtime_state.internals.host_driver_disabled);

        if (get_mods() != user_runtime_state.mods.mods) {
            set_mods(user_runtime_state.mods.mods);
        }
        if (get_weak_mods() != user_runtime_state.mods.weak_mods) {
            set_weak_mods(user_runtime_state.mods.weak_mods);
        }
#ifndef NO_ACTION_ONESHOT
        if (get_oneshot_mods() != user_runtime_state.mods.oneshot_mods) {
            set_oneshot_mods(user_runtime_state.mods.oneshot_mods);
        }
        if (get_oneshot_locked_mods() != user_runtime_state.mods.oneshot_locked_mods) {
            set_oneshot_locked_mods(user_runtime_state.mods.oneshot_locked_mods);
        }
#endif // NO_ACTION_ONESHOT

        if (layer_state != user_runtime_state.layers.layer_state) {
            layer_state = user_runtime_state.layers.layer_state;
        }
        if (default_layer_state != user_runtime_state.layers.default_layer_state) {
            default_layer_state = user_runtime_state.layers.default_layer_state;
        }

        void set_split_host_keyboard_leds(uint8_t led_state);
        if (host_keyboard_led_state().raw != user_runtime_state.leds.raw) {
            set_split_host_keyboard_leds(user_runtime_state.leds.raw);
        }
        if (get_current_wpm() != user_runtime_state.wpm_count) {
            set_current_wpm(user_runtime_state.wpm_count);
        }
        if (last_matrix_activity_time() != user_runtime_state.activity.matrix_timestamp ||
            last_encoder_activity_time() != user_runtime_state.activity.encoder_timestamp ||
            last_pointing_device_activity_time() != user_runtime_state.activity.pointing_device_timestamp) {
            set_activity_timestamps(user_runtime_state.activity.matrix_timestamp,
                                    user_runtime_state.activity.encoder_timestamp,
                                    user_runtime_state.activity.pointing_device_timestamp);
        }
        if (keymap_config.raw != user_runtime_state.keymap_config.raw) {
            keymap_config = user_runtime_state.keymap_config;
        }
#if defined(QUANTUM_PAINTER_ENABLE) && defined(QUANTUM_PAINTER_ILI9341_ENABLE)
        static bool    last_inverted = false;
        static uint8_t last_rotation = 0;
        if (userspace_config.painter.inverted != last_inverted || userspace_config.painter.rotation != last_rotation) {
            last_inverted = userspace_config.painter.inverted;
            last_rotation = userspace_config.painter.rotation;
            void init_display_ili9341_rotation(void);
            init_display_ili9341_rotation();
        }
#endif // QUANTUM_PAINTER_ENABLE && QUANTUM_PAINTER_ILI9341_ENABLE
    }
}

/**
 * @brief Send data over split to the other half of the keyboard
 *
 */
void user_transport_sync(void) {
    if (is_keyboard_master()) {
        // Keep track of the last state, so that we can tell if we need to propagate to slave
        bool                         needs_sync      = false;
        static uint32_t              last_sync[4]    = {0};
        static user_runtime_config_t last_user_state = {0};
        static userspace_config_t    last_config     = {0};
#if defined(DISPLAY_DRIVER_ENABLE) && defined(DISPLAY_KEYLOGGER_ENABLE)
        static char keylog_temp[DISPLAY_KEYLOGGER_LENGTH + 1] = {0};
#endif // DISPLAY_DRIVER_ENABLE && DISPLAY_KEYLOGGER_ENABLE
#if defined(AUTOCORRECT_ENABLE)
        static char temp_autocorrected_str[2][22] = {0};
#endif // AUTOCORRECT_ENABLE
       // Check if the state values are different
        if (memcmp(&user_runtime_state, &last_user_state, sizeof(user_runtime_state))) {
            needs_sync = true;
            memcpy(&last_user_state, &user_runtime_state, sizeof(user_runtime_state));
        }
        // Send to slave every FORCED_SYNC_THROTTLE_MS regardless of state change
        if (timer_elapsed32(last_sync[0]) > FORCED_SYNC_THROTTLE_MS) {
            needs_sync = true;
        }

        // Perform the sync if requested
        if (needs_sync) {
            if (transaction_rpc_send(RPC_ID_USER_RUNTIME_STATE_SYNC, sizeof(user_runtime_state), &user_runtime_state)) {
                last_sync[0] = timer_read32();
            }
            needs_sync = false;
        }

        // Check if the state values are different
        if (memcmp(&userspace_config, &last_config, sizeof(userspace_config))) {
            needs_sync = true;
            memcpy(&last_config, &userspace_config, sizeof(userspace_config));
        }

        // Send to slave every FORCED_SYNC_THROTTLE_MS regardless of state change
        if (timer_elapsed32(last_sync[1]) > FORCED_SYNC_THROTTLE_MS) {
            needs_sync = true;
        }

        // Perform the sync if requested
        if (needs_sync) {
            if (transaction_rpc_send(RPC_ID_USER_CONFIG_SYNC, sizeof(userspace_config), &userspace_config)) {
                last_sync[1] = timer_read32();
            }
            needs_sync = false;
        }

#if defined(DISPLAY_DRIVER_ENABLE) && defined(DISPLAY_KEYLOGGER_ENABLE)
        // Check if the state values are different
        if (memcmp(&display_keylogger_string, &keylog_temp, (DISPLAY_KEYLOGGER_LENGTH + 1))) {
            needs_sync = true;
            memcpy(&keylog_temp, &display_keylogger_string, (DISPLAY_KEYLOGGER_LENGTH + 1));
        }
        if (timer_elapsed32(last_sync[2]) > FORCED_SYNC_THROTTLE_MS) {
            needs_sync = true;
        }

        // Perform the sync if requested
        if (needs_sync) {
            if (transaction_rpc_send(RPC_ID_USER_DISPLAY_KEYLOG_STR, (DISPLAY_KEYLOGGER_LENGTH + 1),
                                     &display_keylogger_string)) {
                last_sync[2] = timer_read32();
            }
            needs_sync = false;
        }
#endif // DISPLAY_DRIVER_ENABLE && DISPLAY_KEYLOGGER_ENABLE
#if defined(AUTOCORRECT_ENABLE)
        if (memcmp(&autocorrected_str, &temp_autocorrected_str, sizeof(autocorrected_str))) {
            needs_sync = true;
            memcpy(&temp_autocorrected_str, &autocorrected_str, sizeof(autocorrected_str));
        }
        if (timer_elapsed32(last_sync[3]) > FORCED_SYNC_THROTTLE_MS) {
            needs_sync = true;
        }

        // Perform the sync if requested
        if (needs_sync) {
            if (transaction_rpc_send(RPC_ID_USER_AUTOCORRECT_STR, sizeof(autocorrected_str), &autocorrected_str)) {
                last_sync[3] = timer_read32();
            }
            needs_sync = false;
        }
#endif // AUTOCORRECT_ENABLE
    }
}

void housekeeping_task_transport_sync(void) {
    if (!is_transport_connected()) {
        return;
    }
    // Update kb_state so we can send to slave
    user_transport_update();

    // Data sync from master to slave
    user_transport_sync();
}

#if 0
// lets define a custom data type to make things easier to work with
typedef struct {
    uint8_t position; // position of the string on the array
    uint8_t length;
    char    str[RPC_S2M_BUFFER_SIZE - 2]; // this is as big as you can fit on the split comms message
} split_msg_t;
_Static_assert(sizeof(split_msg_t) == RPC_S2M_BUFFER_SIZE, "Wrong size");


// instead of
    transaction_rpc_send(RPC_ID_USER_STR, ARRAY_SIZE(stringToWrite), stringToWrite);
// you now do:
    split_msg_t msg = {0};
    msg.position = <your_variable>;
    msg.length = strlen(<your_string>) + 1;
    if (msg.length > ARRAY_SIZE(split_msg_t.str)) {
        // too big to fit
        // do something here if you like, but do not send the message
        return;
    }
    strcpy(msg.str, <your_string>);
    transaction_rpc_send(RPC_ID_USER_STR, sizeof(msg), &msg);

// instead of
    memcpy(stringToWrite, initiator2target_buffer, initiator2target_buffer_size);
// you now do:
    split_msg_t *msg = (split_msg_t *)initiator2target_buffer;
    memcpy(<your_array>[msg->position], msg->str, msg->length);
}
#endif
