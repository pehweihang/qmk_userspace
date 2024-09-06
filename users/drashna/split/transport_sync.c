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

uint32_t transport_user_runtime_state = 0;
uint16_t transport_keymap_config      = 0;
uint64_t transport_userspace_config   = 0;

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
    if (initiator2target_buffer_size == sizeof(transport_user_runtime_state)) {
        memcpy(&transport_user_runtime_state, initiator2target_buffer, initiator2target_buffer_size);
    }
}

/**
 * @brief Syncs keymap_config between halves of split keyboard
 *
 * @param initiator2target_buffer_size
 * @param initiator2target_buffer
 * @param target2initiator_buffer_size
 * @param target2initiator_buffer
 */
void user_keymap_sync(uint8_t initiator2target_buffer_size, const void* initiator2target_buffer,
                      uint8_t target2initiator_buffer_size, void* target2initiator_buffer) {
    if (initiator2target_buffer_size == sizeof(transport_keymap_config)) {
        memcpy(&transport_keymap_config, initiator2target_buffer, initiator2target_buffer_size);
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
    if (initiator2target_buffer_size == sizeof(transport_userspace_config)) {
        memcpy(&transport_userspace_config, initiator2target_buffer, initiator2target_buffer_size);
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
 * @brief Sync suspend state between halves of split keyboard
 *
 * @param initiator2target_buffer_size
 * @param initiator2target_buffer
 * @param target2initiator_buffer_size
 * @param target2initiator_buffer
 */
void suspend_state_sync(uint8_t initiator2target_buffer_size, const void* initiator2target_buffer,
                        uint8_t target2initiator_buffer_size, void* target2initiator_buffer) {
    bool suspend_state = false;
    memcpy(&suspend_state, initiator2target_buffer, initiator2target_buffer_size);
    if (suspend_state != is_device_suspended()) {
        set_is_device_suspended(suspend_state);
    }
}

/**
 * @brief Send the suspend state to the other half of the split keyboard
 *
 * @param status
 */
void send_device_suspend_state(bool status) {
    if (is_device_suspended() != status && is_keyboard_master()) {
        transaction_rpc_send(RPC_ID_USER_SUSPEND_STATE_SYNC, sizeof(bool), &status);
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
    transaction_register_rpc(RPC_ID_USER_KEYMAP_SYNC, user_keymap_sync);
    transaction_register_rpc(RPC_ID_USER_CONFIG_SYNC, user_config_sync);
    transaction_register_rpc(RPC_ID_USER_AUTOCORRECT_STR, autocorrect_string_sync);
    transaction_register_rpc(RPC_ID_USER_DISPLAY_KEYLOG_STR, keylogger_string_sync);
    transaction_register_rpc(RPC_ID_USER_SUSPEND_STATE_SYNC, suspend_state_sync);
}

/**
 * @brief Updates config values for split syncing
 *
 */
void user_transport_update(void) {
    if (is_keyboard_master()) {
        transport_user_runtime_state = user_state.raw;
        transport_keymap_config      = keymap_config.raw;
        transport_userspace_config   = userspace_config.raw;
    } else {
        user_state.raw       = transport_user_runtime_state;
        keymap_config.raw    = transport_keymap_config;
        userspace_config.raw = transport_userspace_config;
#ifdef UNICODE_COMMON_ENABLE
        unicode_config.input_mode = user_state.unicode_mode;
        unicode_typing_mode       = user_state.unicode_typing_mode;
#endif
#if defined(POINTING_DEVICE_ENABLE) && defined(POINTING_DEVICE_AUTO_MOUSE_ENABLE)
        if (get_auto_mouse_toggle() != user_state.tap_toggling) {
            auto_mouse_toggle();
        }
#endif
#ifdef SWAP_HANDS_ENABLE
        swap_hands = user_state.swap_hands;
#endif
#ifdef CAPS_WORD_ENABLE
        if (user_state.is_caps_word) {
            caps_word_on();
        } else {
            caps_word_off();
        }
#endif
        set_keyboard_lock(user_state.host_driver_disabled);
    }
}

/**
 * @brief Send data over split to the other half of the keyboard
 *
 */
void user_transport_sync(void) {
    if (is_keyboard_master()) {
        // Keep track of the last state, so that we can tell if we need to propagate to slave
        static uint32_t last_sync[6], last_user_state = 0;
        static uint16_t last_keymap = 0;
        static uint64_t last_config = 0;
        bool            needs_sync  = false;
#if defined(DISPLAY_DRIVER_ENABLE) && defined(DISPLAY_KEYLOGGER_ENABLE)
        static char keylog_temp[DISPLAY_KEYLOGGER_LENGTH + 1] = {0};
#endif
#if defined(AUTOCORRECT_ENABLE)
        static char temp_autocorrected_str[2][22] = {0};
#endif
        if (timer_elapsed32(last_sync[5]) > FORCED_SYNC_THROTTLE_MS && !is_device_suspended()) {
            transaction_rpc_send(RPC_ID_USER_SUSPEND_STATE_SYNC, sizeof(bool), &needs_sync);
            last_sync[5] = timer_read32();
        }
        // Check if the state values are different
        if (memcmp(&transport_user_runtime_state, &last_user_state, sizeof(transport_user_runtime_state))) {
            needs_sync = true;
            memcpy(&last_user_state, &transport_user_runtime_state, sizeof(transport_user_runtime_state));
        }
        // Send to slave every FORCED_SYNC_THROTTLE_MS regardless of state change
        if (timer_elapsed32(last_sync[0]) > FORCED_SYNC_THROTTLE_MS) {
            needs_sync = true;
        }

        // Perform the sync if requested
        if (needs_sync) {
            if (transaction_rpc_send(RPC_ID_USER_RUNTIME_STATE_SYNC, sizeof(transport_user_runtime_state),
                                     &transport_user_runtime_state)) {
                last_sync[0] = timer_read32();
            }
            needs_sync = false;
        }

        // Check if the state values are different
        if (memcmp(&transport_keymap_config, &last_keymap, sizeof(transport_keymap_config))) {
            needs_sync = true;
            memcpy(&last_keymap, &transport_keymap_config, sizeof(transport_keymap_config));
        }

        // Send to slave every FORCED_SYNC_THROTTLE_MS regardless of state change
        if (timer_elapsed32(last_sync[1]) > FORCED_SYNC_THROTTLE_MS) {
            needs_sync = true;
        }

        // Perform the sync if requested
        if (needs_sync) {
            if (transaction_rpc_send(RPC_ID_USER_KEYMAP_SYNC, sizeof(transport_keymap_config),
                                     &transport_keymap_config)) {
                last_sync[1] = timer_read32();
            }
            needs_sync = false;
        }

        // Check if the state values are different
        if (memcmp(&transport_userspace_config, &last_config, sizeof(transport_userspace_config))) {
            needs_sync = true;
            memcpy(&last_config, &transport_userspace_config, sizeof(transport_userspace_config));
        }

        // Send to slave every FORCED_SYNC_THROTTLE_MS regardless of state change
        if (timer_elapsed32(last_sync[2]) > FORCED_SYNC_THROTTLE_MS) {
            needs_sync = true;
        }

        // Perform the sync if requested
        if (needs_sync) {
            if (transaction_rpc_send(RPC_ID_USER_CONFIG_SYNC, sizeof(transport_userspace_config),
                                     &transport_userspace_config)) {
                last_sync[2] = timer_read32();
            }
            needs_sync = false;
        }

#if defined(DISPLAY_DRIVER_ENABLE) && defined(DISPLAY_KEYLOGGER_ENABLE)
        // Check if the state values are different
        if (memcmp(&display_keylogger_string, &keylog_temp, (DISPLAY_KEYLOGGER_LENGTH + 1))) {
            needs_sync = true;
            memcpy(&keylog_temp, &display_keylogger_string, (DISPLAY_KEYLOGGER_LENGTH + 1));
        }
        if (timer_elapsed32(last_sync[3]) > FORCED_SYNC_THROTTLE_MS) {
            needs_sync = true;
        }

        // Perform the sync if requested
        if (needs_sync) {
            if (transaction_rpc_send(RPC_ID_USER_DISPLAY_KEYLOG_STR, (DISPLAY_KEYLOGGER_LENGTH + 1),
                                     &display_keylogger_string)) {
                last_sync[3] = timer_read32();
            }
            needs_sync = false;
        }
#endif
#if defined(AUTOCORRECT_ENABLE)
        if (memcmp(&autocorrected_str, &temp_autocorrected_str, sizeof(autocorrected_str))) {
            needs_sync = true;
            memcpy(&temp_autocorrected_str, &autocorrected_str, sizeof(autocorrected_str));
        }
        if (timer_elapsed32(last_sync[4]) > FORCED_SYNC_THROTTLE_MS) {
            needs_sync = true;
        }

        // Perform the sync if requested
        if (needs_sync) {
            if (transaction_rpc_send(RPC_ID_USER_AUTOCORRECT_STR, sizeof(autocorrected_str), &autocorrected_str)) {
                last_sync[4] = timer_read32();
            }
            needs_sync = false;
        }
#endif
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
