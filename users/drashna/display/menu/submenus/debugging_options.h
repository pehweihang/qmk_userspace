
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Debugging

static bool menu_handler_debugging_enable(menu_input_t input) {
    switch (input) {
        case menu_input_left:
        case menu_input_right:
            debug_enable = !debug_enable;
            return false;
        default:
            return true;
    }
}

void display_handler_debugging_enable(char *text_buffer, size_t buffer_len) {
    snprintf(text_buffer, buffer_len - 1, "%s", debug_enable ? "enabled" : "disabled");
}

static bool menu_handler_keyboard_debugging(menu_input_t input) {
    switch (input) {
        case menu_input_left:
        case menu_input_right:
            debug_keyboard = !debug_keyboard;
            return false;
        default:
            return true;
    }
}

void display_handler_keyboard_debugging(char *text_buffer, size_t buffer_len) {
    snprintf(text_buffer, buffer_len - 1, "%s", debug_keyboard ? "enabled" : "disabled");
}

static bool menu_handler_matrix_debugging(menu_input_t input) {
    switch (input) {
        case menu_input_left:
        case menu_input_right:
            debug_matrix = !debug_matrix;
            return false;
        default:
            return true;
    }
}

void display_handler_matrix_debugging(char *text_buffer, size_t buffer_len) {
    snprintf(text_buffer, buffer_len - 1, "%s", debug_matrix ? "enabled" : "disabled");
}

static bool menu_handler_mouse_debugging(menu_input_t input) {
    switch (input) {
        case menu_input_left:
        case menu_input_right:
            debug_mouse = !debug_mouse;
            return false;
        default:
            return true;
    }
}

void display_handler_mouse_debugging(char *text_buffer, size_t buffer_len) {
    snprintf(text_buffer, buffer_len - 1, "%s", debug_mouse ? "enabled" : "disabled");
}

static bool menu_handler_pointing_debugging(menu_input_t input) {
    switch (input) {
        case menu_input_left:
        case menu_input_right:
            debug_pointing = !debug_pointing;
            return false;
        default:
            return true;
    }
}

void display_handler_pointing_debugging(char *text_buffer, size_t buffer_len) {
    snprintf(text_buffer, buffer_len - 1, "%s", debug_pointing ? "enabled" : "disabled");
}

static bool menu_handler_action_debugging(menu_input_t input) {
    switch (input) {
        case menu_input_left:
        case menu_input_right:
            debug_action = !debug_action;
            return false;
        default:
            return true;
    }
}

void display_handler_action_debugging(char *text_buffer, size_t buffer_len) {
    snprintf(text_buffer, buffer_len - 1, "%s", debug_action ? "enabled" : "disabled");
}

static bool menu_handler_split_serial_debugging(menu_input_t input) {
    switch (input) {
        case menu_input_left:
        case menu_input_right:
            debug_serial = !debug_serial;
            return false;
        default:
            return true;
    }
}

void display_handler_split_serial_debugging(char *text_buffer, size_t buffer_len) {
    snprintf(text_buffer, buffer_len - 1, "%s", debug_serial ? "enabled" : "disabled");
}

static bool menu_handler_quantum_painter_debugging(menu_input_t input) {
    switch (input) {
        case menu_input_left:
        case menu_input_right:
            debug_quantum_painter = !debug_quantum_painter;
            return false;
        default:
            return true;
    }
}

void display_handler_quantum_painter_debugging(char *text_buffer, size_t buffer_len) {
    snprintf(text_buffer, buffer_len - 1, "%s", debug_quantum_painter ? "enabled" : "disabled");
}

menu_entry_t debug_entries[] = {
    MENU_ENTRY_CHILD("Debugging", debugging_enable), // force formatting
    MENU_ENTRY_CHILD("Keyboard Debugging", keyboard_debugging),
    MENU_ENTRY_CHILD("Matrix Debugging", matrix_debugging),
    MENU_ENTRY_CHILD("Mouse Debugging", mouse_debugging),
    MENU_ENTRY_CHILD("Pointing Device Debugging", pointing_debugging),
    MENU_ENTRY_CHILD("Action Debugging", action_debugging),
    MENU_ENTRY_CHILD("Split Serial Debugging", split_serial_debugging),
    MENU_ENTRY_CHILD("Quantum Painter Debugging", quantum_painter_debugging),
    MENU_ENTRY_CHILD("I2C Scanner", i2c_scanner),
    MENU_ENTRY_CHILD("Matrix Scan Rate Print", scan_rate),
};
