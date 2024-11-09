////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// User Settings

static bool menu_handler_overwatch_mode(menu_input_t input) {
    switch (input) {
        case menu_input_left:
        case menu_input_right:
            userspace_config.gaming.is_overwatch = !userspace_config.gaming.is_overwatch;
            eeconfig_update_user_datablock(&userspace_config);
            return false;
        default:
            return true;
    }
}

void display_handler_overwatch_mode(char *text_buffer, size_t buffer_len) {
    snprintf(text_buffer, buffer_len - 1, "%s", userspace_config.gaming.is_overwatch ? "on" : "off");
}

static bool menu_handler_gamepad_swap(menu_input_t input) {
    switch (input) {
        case menu_input_left:
        case menu_input_right:
            userspace_config.gaming.swapped_numbers = !userspace_config.gaming.swapped_numbers;
            eeconfig_update_user_datablock(&userspace_config);
            return false;
        default:
            return true;
    }
}

void display_handler_gamepad_swap(char *text_buffer, size_t buffer_len) {
    snprintf(text_buffer, buffer_len - 1, "%s", userspace_config.gaming.swapped_numbers ? "swapped" : "normal");
}

static bool menu_handler_clap_trap(menu_input_t input) {
    switch (input) {
        case menu_input_left:
        case menu_input_right:
            userspace_config.gaming.clap_trap_enable = !userspace_config.gaming.clap_trap_enable;
            eeconfig_update_user_datablock(&userspace_config);
            return false;
        default:
            return true;
    }
}

void display_handler_clap_trap(char *text_buffer, size_t buffer_len) {
    snprintf(text_buffer, buffer_len - 1, "%s", userspace_config.gaming.clap_trap_enable ? "on" : "off");
}

static bool menu_handler_i2c_scanner(menu_input_t input) {
    switch (input) {
        case menu_input_left:
        case menu_input_right:
            userspace_config.debug.i2c_scanner_enable = !userspace_config.debug.i2c_scanner_enable;
            eeconfig_update_user_datablock(&userspace_config);
            return false;
        default:
            return true;
    }
}

void display_handler_i2c_scanner(char *text_buffer, size_t buffer_len) {
    snprintf(text_buffer, buffer_len - 1, "%s", userspace_config.debug.i2c_scanner_enable ? "on" : "off");
}

static bool menu_handler_scan_rate(menu_input_t input) {
    switch (input) {
        case menu_input_left:
        case menu_input_right:
            userspace_config.debug.matrix_scan_print = !userspace_config.debug.matrix_scan_print;
            eeconfig_update_user_datablock(&userspace_config);
            return false;
        default:
            return true;
    }
}

void display_handler_scan_rate(char *text_buffer, size_t buffer_len) {
    snprintf(text_buffer, buffer_len - 1, "%s", userspace_config.debug.matrix_scan_print ? "on" : "off");
}

menu_entry_t user_settings_option_entries[] = {
    MENU_ENTRY_CHILD("Overwatch Mode", overwatch_mode),
    MENU_ENTRY_CHILD("Gamepad 1<->2 Swap", gamepad_swap),
    MENU_ENTRY_CHILD("SOCD Cleaner", clap_trap),
};
