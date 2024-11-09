////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Pointing Device

#ifdef POINTING_DEVICE_ENABLE
#    include "pointing/pointing.h"

static bool menu_handler_auto_mouse_enable(menu_input_t input) {
    switch (input) {
        case menu_input_left:
        case menu_input_right:
            userspace_config.pointing.auto_mouse_layer_enable = !userspace_config.pointing.auto_mouse_layer_enable;
            eeconfig_update_user_datablock(&userspace_config);
            set_auto_mouse_enable(userspace_config.pointing.auto_mouse_layer_enable);
            return false;
        default:
            return true;
    }
}

void display_handler_auto_mouse_enable(char *text_buffer, size_t buffer_len) {
    snprintf(text_buffer, buffer_len - 1, "%s",
             userspace_config.pointing.auto_mouse_layer_enable ? "enabled" : "disabled");
}

static bool menu_handler_auto_mouse_layer(menu_input_t input) {
    switch (input) {
        case menu_input_left:
            set_auto_mouse_layer((get_auto_mouse_layer() - 1) % MAX_USER_LAYERS);
            return false;
        case menu_input_right:
            set_auto_mouse_layer((get_auto_mouse_layer() + 1) % MAX_USER_LAYERS);
            return false;
        default:
            return true;
    }
}

void display_handler_auto_mouse_layer(char *text_buffer, size_t buffer_len) {
    snprintf(text_buffer, buffer_len - 1, "%s", layer_name(get_auto_mouse_layer()));
}

extern uint16_t mouse_jiggler_timer;

static bool menu_handler_mouse_jiggler(menu_input_t input) {
    switch (input) {
        case menu_input_left:
        case menu_input_right:
            userspace_runtime_state.pointing.mouse_jiggler_enable =
                !userspace_runtime_state.pointing.mouse_jiggler_enable;
            mouse_jiggler_timer = timer_read();
            return false;
        default:
            return true;
    }
}

void display_handler_mouse_jiggler(char *text_buffer, size_t buffer_len) {
    snprintf(text_buffer, buffer_len - 1, "%s", userspace_runtime_state.pointing.mouse_jiggler_enable ? "on" : "off");
}

static bool menu_handler_mouse_jiggler_interrupt(menu_input_t input) {
    switch (input) {
        case menu_input_left:
        case menu_input_right:
            userspace_config.pointing.mouse_jiggler_interrupt = !userspace_config.pointing.mouse_jiggler_interrupt;
            mouse_jiggler_timer                               = timer_read();
            return false;
        default:
            return true;
    }
}

void display_handler_mouse_jiggler_interrupt(char *text_buffer, size_t buffer_len) {
    snprintf(text_buffer, buffer_len - 1, "%s", userspace_config.pointing.mouse_jiggler_interrupt ? "on" : "off");
}

#    if defined(KEYBOARD_handwired_tractyl_manuform) || defined(KEYBOARD_bastardkb_charybdis)
#        include QMK_KEYBOARD_H

static bool menu_handler_dpi_config(menu_input_t input) {
    switch (input) {
        case menu_input_left:
            charybdis_cycle_pointer_default_dpi(false);
            return false;
        case menu_input_right:
            charybdis_cycle_pointer_default_dpi(true);
            return false;
        default:
            return true;
    }
}

void display_handler_dpi_config(char *text_buffer, size_t buffer_len) {
    snprintf(text_buffer, buffer_len - 1, "%d", charybdis_get_pointer_default_dpi());
}
#    endif

static bool menu_handler_mouse_accel_toggle(menu_input_t input) {
    switch (input) {
        case menu_input_left:
        case menu_input_right:
            pointing_device_accel_toggle_enabled();
            return false;
        default:
            return true;
    }
}

void display_handler_mouse_accel_toggle(char *text_buffer, size_t buffer_len) {
    snprintf(text_buffer, buffer_len - 1, "%s", pointing_device_accel_get_enabled() ? "on" : "off");
}

static bool menu_handler_mouse_accel_takeoff(menu_input_t input) {
    switch (input) {
        case menu_input_left:
            pointing_device_accel_set_takeoff(pointing_device_accel_get_takeoff() -
                                              pointing_device_accel_get_mod_step(POINTING_DEVICE_ACCEL_TAKEOFF_STEP));
            return false;
        case menu_input_right:
            pointing_device_accel_set_takeoff(pointing_device_accel_get_takeoff() +
                                              pointing_device_accel_get_mod_step(POINTING_DEVICE_ACCEL_TAKEOFF_STEP));
            return false;
        default:
            return true;
    }
}

void display_handler_mouse_accel_takeoff(char *text_buffer, size_t buffer_len) {
    snprintf(text_buffer, buffer_len - 1, "%1.2f", pointing_device_accel_get_takeoff());
}

static bool menu_handler_mouse_accel_growth_rate(menu_input_t input) {
    switch (input) {
        case menu_input_left:
            pointing_device_accel_set_growth_rate(
                pointing_device_accel_get_growth_rate() -
                pointing_device_accel_get_mod_step(POINTING_DEVICE_ACCEL_GROWTH_RATE_STEP));
            return false;
        case menu_input_right:
            pointing_device_accel_set_growth_rate(
                pointing_device_accel_get_growth_rate() +
                pointing_device_accel_get_mod_step(POINTING_DEVICE_ACCEL_GROWTH_RATE_STEP));
            return false;
        default:
            return true;
    }
}

void display_handler_mouse_accel_growth_rate(char *text_buffer, size_t buffer_len) {
    snprintf(text_buffer, buffer_len - 1, "%1.2f", pointing_device_accel_get_growth_rate());
}

static bool menu_handler_mouse_accel_offset(menu_input_t input) {
    switch (input) {
        case menu_input_left:
            pointing_device_accel_set_offset(pointing_device_accel_get_offset() -
                                             pointing_device_accel_get_mod_step(POINTING_DEVICE_ACCEL_OFFSET_STEP));
            return false;
        case menu_input_right:
            pointing_device_accel_set_offset(pointing_device_accel_get_offset() +
                                             pointing_device_accel_get_mod_step(POINTING_DEVICE_ACCEL_OFFSET_STEP));
            return false;
        default:
            return true;
    }
}

void display_handler_mouse_accel_offset(char *text_buffer, size_t buffer_len) {
    snprintf(text_buffer, buffer_len - 1, "%2.2f", pointing_device_accel_get_offset());
}

static bool menu_handler_mouse_accel_limit(menu_input_t input) {
    switch (input) {
        case menu_input_left:
            pointing_device_accel_set_limit(pointing_device_accel_get_limit() -
                                            pointing_device_accel_get_mod_step(POINTING_DEVICE_ACCEL_LIMIT_STEP));
            return false;
        case menu_input_right:
            pointing_device_accel_set_limit(pointing_device_accel_get_limit() +
                                            pointing_device_accel_get_mod_step(POINTING_DEVICE_ACCEL_LIMIT_STEP));
            return false;
        default:
            return true;
    }
}

void display_handler_mouse_accel_limit(char *text_buffer, size_t buffer_len) {
    snprintf(text_buffer, buffer_len - 1, "%1.2f", pointing_device_accel_get_limit());
}

menu_entry_t pointing_acceleration_entries[] = {
    MENU_ENTRY_CHILD("Acceleration", mouse_accel_toggle),
    MENU_ENTRY_CHILD("Takeoff", mouse_accel_takeoff),
    MENU_ENTRY_CHILD("Growth Rate", mouse_accel_growth_rate),
    MENU_ENTRY_CHILD("Offset", mouse_accel_offset),
    MENU_ENTRY_CHILD("Limit", mouse_accel_limit),
};

menu_entry_t pointing_entries[] = {
    MENU_ENTRY_MULTI("Mouse Acceleration", pointing_acceleration_entries, mouse_accel_toggle),
#    if defined(KEYBOARD_handwired_tractyl_manuform) || defined(KEYBOARD_bastardkb_charybdis)
    MENU_ENTRY_CHILD("DPI Config", dpi_config),
#    endif // KEYBOARD_handwired_tractyl_manuform || KEYBOARD_bastardkb_charybdis
    MENU_ENTRY_CHILD("Auto Mouse", auto_mouse_enable),
    MENU_ENTRY_CHILD("Auto Mouse Layer", auto_mouse_layer),
    MENU_ENTRY_CHILD("Mouse Jiggler", mouse_jiggler),
    MENU_ENTRY_CHILD("Allow Jiggler Interrupt", mouse_jiggler_interrupt),
#    ifdef AUDIO_ENABLE
    MENU_ENTRY_CHILD("Mouse Clicky", audio_mouse_clicky),
#    endif
};
#endif // POINTING_DEVICE_ENABLE
