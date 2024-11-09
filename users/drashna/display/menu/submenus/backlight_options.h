
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Backlight

#ifdef BACKLIGHT_ENABLE
static bool menu_handler_bl_enabled(menu_input_t input) {
    switch (input) {
        case menu_input_left:
        case menu_input_right:
            backlight_toggle();
            return false;
        default:
            return true;
    }
}

void display_handler_bl_enabled(char *text_buffer, size_t buffer_len) {
    snprintf(text_buffer, buffer_len - 1, "%s", is_backlight_enabled() ? "on" : "off");
}

static bool menu_handler_bl_level(menu_input_t input) {
    switch (input) {
        case menu_input_left:
            backlight_decrease();
            return false;
        case menu_input_right:
            backlight_increase();
            return false;
        default:
            return true;
    }
}

void display_handler_bl_level(char *text_buffer, size_t buffer_len) {
    snprintf(text_buffer, buffer_len - 1, "%d", get_backlight_level());
}

menu_entry_t backlight_entries[] = {
    MENU_ENTRY_CHILD("Backlight Enabled", bl_enabled),
    MENU_ENTRY_CHILD("Backlight Level", bl_level),
};
#endif // BACKLIGHT_ENABLE
