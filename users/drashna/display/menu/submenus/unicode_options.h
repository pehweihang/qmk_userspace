////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Unicode

#ifdef UNICODE_COMMON_ENABLE

static bool menu_handler_unicode(menu_input_t input) {
    switch (input) {
        case menu_input_left:
            unicode_input_mode_step_reverse();
            return false;
        case menu_input_right:
            unicode_input_mode_step();
            return false;
        default:
            return true;
    }
}

void display_handler_unicode(char *text_buffer, size_t buffer_len) {
    switch (get_unicode_input_mode()) {
        case UNICODE_MODE_MACOS:
            strncpy(text_buffer, "macOS", buffer_len - 1);
            return;
        case UNICODE_MODE_LINUX:
            strncpy(text_buffer, "Linux", buffer_len - 1);
            return;
        case UNICODE_MODE_BSD:
            strncpy(text_buffer, "BSD", buffer_len - 1);
            return;
        case UNICODE_MODE_WINDOWS:
            strncpy(text_buffer, "Windows", buffer_len - 1);
            return;
        case UNICODE_MODE_WINCOMPOSE:
            strncpy(text_buffer, "WinCompose", buffer_len - 1);
            return;
        case UNICODE_MODE_EMACS:
            strncpy(text_buffer, "Emacs", buffer_len - 1);
            return;
    }

    strncpy(text_buffer, "Unknown", buffer_len);
}

static bool menu_handler_unicode_typing(menu_input_t input) {
    switch (input) {
        case menu_input_left:
            userspace_runtime_state.unicode.typing_mode =
                (userspace_runtime_state.unicode.typing_mode - 1) % UNCODES_MODE_END;
            if (userspace_runtime_state.unicode.typing_mode >= UNCODES_MODE_END) {
                userspace_runtime_state.unicode.typing_mode = UNCODES_MODE_END - 1;
            }
            return false;
        case menu_input_right:
            userspace_runtime_state.unicode.typing_mode =
                (userspace_runtime_state.unicode.typing_mode + 1) % UNCODES_MODE_END;
            if (userspace_runtime_state.unicode.typing_mode >= UNCODES_MODE_END) {
                userspace_runtime_state.unicode.typing_mode = 0;
            }
            return false;
        default:
            return true;
    }
}
void display_handler_unicode_typing(char *text_buffer, size_t buffer_len) {
    strncpy(text_buffer, unicode_typing_mode(userspace_runtime_state.unicode.typing_mode), buffer_len);
}

menu_entry_t unicode_entries[] = {
    MENU_ENTRY_CHILD("Unicode mode", unicode),
    MENU_ENTRY_CHILD("Unicode Typing Mode", unicode_typing),
};
#endif // UNICODE_COMMON_ENABLE
