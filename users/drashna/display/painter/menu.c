// Copyright 2018-2024 Nick Brassel (@tzarc)
// Copyright 2024 Drashna (@drashna)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "drashna.h"
#include <printf.h>
#include "display/painter/menu.h"
#include "display/painter/painter.h"
#include "process_keycode/process_unicode_common.h"
#include "unicode.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Unicode

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
            unicode_typing_mode = (unicode_typing_mode - 1) % UNICODE_MODE_COUNT;
            return false;
        case menu_input_right:
            unicode_typing_mode = (unicode_typing_mode + 1) % UNICODE_MODE_COUNT;
            return false;
        default:
            return true;
    }
}
void display_handler_unicode_typing(char *text_buffer, size_t buffer_len) {
    switch (unicode_typing_mode) {
        case UCTM_NO_MODE:
            strncpy(text_buffer, "Normal", buffer_len - 1); // Normal
            return;
        case UCTM_WIDE:
            strncpy(text_buffer, "Wide", buffer_len - 1); // ｗｉｄｅ
            return;
        case UCTM_SCRIPT:
            strncpy(text_buffer, "Script", buffer_len - 1); // 𝓢𝓬𝓻𝓲𝓹𝓽
            return;
        case UCTM_BLOCKS:
            strncpy(text_buffer, "Blocks", buffer_len - 1); // 🅱🅻🅾🅲🅺🆂
            return;
        case UCTM_REGIONAL:
            strncpy(text_buffer, "Regional",
                    buffer_len - 1); // 🇷‌‌🇪‌‌🇬‌‌🇮‌‌🇴‌‌🇳‌‌🇦‌‌🇱‌‌
            return;
        case UCTM_AUSSIE:
            strncpy(text_buffer, "Aussie", buffer_len - 1); // ǝᴉssnɐ
            return;
        case UCTM_ZALGO:
            strncpy(text_buffer, "Zalgo", buffer_len - 1); // z̴̬̙̐̋͢ā̸̧̺͂ͥ͐͟l̵̪̻͎̈ͭ̋͠g̦ͥo͚ͫͣ
            return;
        case UCTM_SUPER:
            strncpy(text_buffer, "SuperScript", buffer_len - 1); // ˢᵘᵖᵉʳˢᶜʳᶦᵖᵗ
            return;
        case UCTM_COMIC:
            strncpy(text_buffer, "Comic", buffer_len - 1); // ƈơɱıƈ
            return;
        case UCTM_FRAKTUR:
            strncpy(text_buffer, "Fraktur", buffer_len - 1); // 𝔉𝔯𝔞𝔨𝔱𝔲𝔯
            return;
        case UCTM_DOUBLE_STRUCK:
            strncpy(text_buffer, "Double Struck", buffer_len - 1); // 𝕯𝖔𝖚𝖇𝖑𝖊 𝕾𝖙𝖗𝖚𝖈𝖐
            return;
    }

    strncpy(text_buffer, "Unknown", buffer_len);
}

menu_entry_t unicode_entries[] = {
    {
        .flags                 = menu_flag_is_value,
        .text                  = "Unicode mode",
        .child.menu_handler    = menu_handler_unicode,
        .child.display_handler = display_handler_unicode,
    },
    {
        .flags                 = menu_flag_is_value,
        .text                  = "Unicode Typing Mode",
        .child.menu_handler    = menu_handler_unicode_typing,
        .child.display_handler = display_handler_unicode_typing,
    },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// RGB Matrix

static bool menu_handler_rgbenabled(menu_input_t input) {
    switch (input) {
        case menu_input_left:
        case menu_input_right:
            if (display_menu_state.menu_stack[0] == 2) {
                rgb_matrix_toggle();
            } else if (display_menu_state.menu_stack[0] == 3) {
                rgblight_toggle();
            }
            return false;
        default:
            return true;
    }
}

void display_handler_rgbenabled(char *text_buffer, size_t buffer_len) {
    if (display_menu_state.menu_stack[0] == 2) {
        snprintf(text_buffer, buffer_len - 1, "%s", rgb_matrix_is_enabled() ? "on" : "off");
    } else if (display_menu_state.menu_stack[0] == 3) {
        snprintf(text_buffer, buffer_len - 1, "%s", rgblight_is_enabled() ? "on" : "off");
    }
}

static bool menu_handler_rgbmode(menu_input_t input) {
    switch (input) {
        case menu_input_left:
            if (display_menu_state.menu_stack[0] == 2) {
                rgb_matrix_step_reverse();
            } else if (display_menu_state.menu_stack[0] == 3) {
                rgblight_step_reverse();
            }
            return false;
        case menu_input_right:
            if (display_menu_state.menu_stack[0] == 2) {
                rgb_matrix_step();
            } else if (display_menu_state.menu_stack[0] == 3) {
                rgblight_step();
            }
            return false;
        default:
            return true;
    }
}

void display_handler_rgbmode(char *text_buffer, size_t buffer_len) {
    if (display_menu_state.menu_stack[0] == 2) {
        snprintf(text_buffer, buffer_len - 1, "%s", rgb_matrix_get_effect_name());
    } else if (display_menu_state.menu_stack[0] == 3) {
        snprintf(text_buffer, buffer_len - 1, "%s", rgblight_get_effect_name());
    }
}

static bool menu_handler_rgbhue(menu_input_t input) {
    switch (input) {
        case menu_input_left:
            if (display_menu_state.menu_stack[0] == 2) {
                rgb_matrix_decrease_hue();
            } else if (display_menu_state.menu_stack[0] == 3) {
                rgblight_decrease_hue();
            }
            return false;
        case menu_input_right:
            if (display_menu_state.menu_stack[0] == 2) {
                rgb_matrix_increase_hue();
            } else if (display_menu_state.menu_stack[0] == 3) {
                rgblight_increase_hue();
            }
            return false;
        default:
            return true;
    }
}

void display_handler_rgbhue(char *text_buffer, size_t buffer_len) {
    if (display_menu_state.menu_stack[0] == 2) {
        snprintf(text_buffer, buffer_len - 1, "%d", (int)rgb_matrix_get_hue());
    } else if (display_menu_state.menu_stack[0] == 3) {
        snprintf(text_buffer, buffer_len - 1, "%d", (int)rgblight_get_hue());
    }
}

static bool menu_handler_rgbsat(menu_input_t input) {
    switch (input) {
        case menu_input_left:
            if (display_menu_state.menu_stack[0] == 2) {
                rgb_matrix_decrease_sat();
            } else if (display_menu_state.menu_stack[0] == 3) {
                rgblight_decrease_sat();
            }
            return false;
        case menu_input_right:
            if (display_menu_state.menu_stack[0] == 2) {
                rgb_matrix_increase_sat();
            } else if (display_menu_state.menu_stack[0] == 3) {
                rgblight_increase_sat();
            }
            return false;
        default:
            return true;
    }
}

void display_handler_rgbsat(char *text_buffer, size_t buffer_len) {
    snprintf(text_buffer, buffer_len - 1, "%d", (int)rgb_matrix_get_sat());
}

static bool menu_handler_rgbval(menu_input_t input) {
    switch (input) {
        case menu_input_left:
            if (display_menu_state.menu_stack[0] == 2) {
                rgb_matrix_decrease_val();
            } else if (display_menu_state.menu_stack[0] == 3) {
                rgblight_decrease_val();
            }
            return false;
        case menu_input_right:
            if (display_menu_state.menu_stack[0] == 2) {
                rgb_matrix_increase_val();
            } else if (display_menu_state.menu_stack[0] == 3) {
                rgblight_increase_val();
            }
            return false;
        default:
            return true;
    }
}

void display_handler_rgbval(char *text_buffer, size_t buffer_len) {
    if (display_menu_state.menu_stack[0] == 2) {
        snprintf(text_buffer, buffer_len - 1, "%d", (int)rgb_matrix_get_val());
    } else if (display_menu_state.menu_stack[0] == 3) {
        snprintf(text_buffer, buffer_len - 1, "%d", (int)rgblight_get_val());
    }
}

static bool menu_handler_rgbspeed(menu_input_t input) {
    switch (input) {
        case menu_input_left:
            if (display_menu_state.menu_stack[0] == 2) {
                rgb_matrix_decrease_speed();
            } else if (display_menu_state.menu_stack[0] == 3) {
                rgblight_decrease_speed();
            }
            return false;
        case menu_input_right:
            if (display_menu_state.menu_stack[0] == 2) {
                rgb_matrix_increase_speed();
            } else if (display_menu_state.menu_stack[0] == 3) {
                rgblight_increase_speed();
            }
            return false;
        default:
            return true;
    }
}

void display_handler_rgbspeed(char *text_buffer, size_t buffer_len) {
    if (display_menu_state.menu_stack[0] == 2) {
        snprintf(text_buffer, buffer_len - 1, "%d", (int)rgb_matrix_get_speed());
    } else if (display_menu_state.menu_stack[0] == 3) {
        snprintf(text_buffer, buffer_len - 1, "%d", (int)rgblight_get_speed());
    }
}

menu_entry_t rgb_matrix_entries[] = {
    {
        .flags                 = menu_flag_is_value,
        .text                  = "RGB Enabled",
        .child.menu_handler    = menu_handler_rgbenabled,
        .child.display_handler = display_handler_rgbenabled,
    },
    {
        .flags                 = menu_flag_is_value,
        .text                  = "RGB Mode",
        .child.menu_handler    = menu_handler_rgbmode,
        .child.display_handler = display_handler_rgbmode,
    },
    {
        .flags                 = menu_flag_is_value,
        .text                  = "RGB Hue",
        .child.menu_handler    = menu_handler_rgbhue,
        .child.display_handler = display_handler_rgbhue,
    },
    {
        .flags                 = menu_flag_is_value,
        .text                  = "RGB Saturation",
        .child.menu_handler    = menu_handler_rgbsat,
        .child.display_handler = display_handler_rgbsat,
    },
    {
        .flags                 = menu_flag_is_value,
        .text                  = "RGB Value",
        .child.menu_handler    = menu_handler_rgbval,
        .child.display_handler = display_handler_rgbval,
    },
    {
        .flags                 = menu_flag_is_value,
        .text                  = "RGB Speed",
        .child.menu_handler    = menu_handler_rgbspeed,
        .child.display_handler = display_handler_rgbspeed,
    },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Display options

uint8_t display_mode = 0;

static bool menu_handler_display(menu_input_t input) {
    switch (display_mode) {
        case menu_input_left:
            display_mode = (display_mode - 1) % 4;
            return false;
        case menu_input_right:
            display_mode = (display_mode + 1) % 4;
            return false;
        default:
            return true;
    }
}

void display_handler_display(char *text_buffer, size_t buffer_len) {
    switch (display_mode) {
        case 0:
            strncpy(text_buffer, "Layer Map", buffer_len - 1);
            return;
        case 1:
            strncpy(text_buffer, "Font 1", buffer_len - 1);
            return;
        case 2:
            strncpy(text_buffer, "Font 2", buffer_len - 1);
            return;
        case 3:
            strncpy(text_buffer, "Font 3", buffer_len - 1);
            return;
    }

    strncpy(text_buffer, ": Unknown", buffer_len);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Root menu

menu_entry_t root_entries[] = {
    [0] =
        {
            .flags                 = menu_flag_is_value,
            .text                  = "Display Option",
            .child.menu_handler    = menu_handler_display,
            .child.display_handler = display_handler_display,
        },
    [1] =
        {
            .flags              = menu_flag_is_parent,
            .text               = "Unicode Settings",
            .parent.children    = unicode_entries,
            .parent.child_count = ARRAY_SIZE(unicode_entries),
        },
    [2] =
        {
            .flags              = menu_flag_is_parent,
            .text               = "RGB Matrix Settings",
            .parent.children    = rgb_matrix_entries,
            .parent.child_count = ARRAY_SIZE(rgb_matrix_entries),
        },
    [3] =
        {
            .flags              = menu_flag_is_parent,
            .text               = "RGB Light Settings",
            .parent.children    = rgb_matrix_entries,
            .parent.child_count = ARRAY_SIZE(rgb_matrix_entries),
        },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Root Title

menu_entry_t root = {
    .flags              = menu_flag_is_parent,
    .text               = "Configuration",
    .parent.children    = root_entries,
    .parent.child_count = ARRAY_SIZE(root_entries),
};

menu_state_t display_menu_state = {
#ifndef DISPLAY_MENU_ENABLED_DEFAULT
    .dirty          = false,
    .is_in_menu     = false,
    .menu_stack     = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
    .selected_child = 0xFF,
#else
    .dirty          = true,
    .is_in_menu     = true,
    .menu_stack     = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
    .selected_child = 0x00,
#endif
};

menu_entry_t *get_current_menu(void) {
    if (display_menu_state.menu_stack[0] == 0xFF) {
        return NULL;
    }

    menu_entry_t *entry = &root;
    for (int i = 0; i < sizeof(display_menu_state.menu_stack); ++i) {
        if (display_menu_state.menu_stack[i] == 0xFF) {
            return entry;
        }
        entry = &entry->parent.children[display_menu_state.menu_stack[i]];
    }

    return entry;
}

menu_entry_t *get_selected_menu_item(void) {
    return &(get_current_menu()->parent.children[display_menu_state.selected_child]);
}

bool menu_handle_input(menu_input_t input) {
    menu_entry_t *menu     = get_current_menu();
    menu_entry_t *selected = get_selected_menu_item();
    switch (input) {
        case menu_input_exit:
            display_menu_state.is_in_menu = false;
            memset(display_menu_state.menu_stack, 0xFF, sizeof(display_menu_state.menu_stack));
            display_menu_state.selected_child = 0xFF;
            return false;
        case menu_input_back:
            // Iterate backwards through the stack and remove the last entry
            for (uint8_t i = 0; i < sizeof(display_menu_state.menu_stack); ++i) {
                if (display_menu_state.menu_stack[sizeof(display_menu_state.menu_stack) - 1 - i] != 0xFF) {
                    display_menu_state.selected_child =
                        display_menu_state.menu_stack[sizeof(display_menu_state.menu_stack) - 1 - i];
                    display_menu_state.menu_stack[sizeof(display_menu_state.menu_stack) - 1 - i] = 0xFF;
                    break;
                }

                // If we've dropped out of the last entry in the stack, exit the menu
                if (i == sizeof(display_menu_state.menu_stack) - 1) {
                    display_menu_state.is_in_menu     = false;
                    display_menu_state.selected_child = 0xFF;
                }
            }
            return false;
        case menu_input_enter:
            // Only attempt to enter the next menu if we're a parent object
            if (selected->flags & menu_flag_is_parent) {
                // Iterate forwards through the stack and add the selected entry
                for (uint8_t i = 0; i < sizeof(display_menu_state.menu_stack); ++i) {
                    if (display_menu_state.menu_stack[i] == 0xFF) {
                        display_menu_state.menu_stack[i]  = display_menu_state.selected_child;
                        display_menu_state.selected_child = 0;
                        break;
                    }
                }
            }
            return false;
        case menu_input_up:
            display_menu_state.selected_child =
                (display_menu_state.selected_child + menu->parent.child_count - 1) % menu->parent.child_count;
            return false;
        case menu_input_down:
            display_menu_state.selected_child =
                (display_menu_state.selected_child + menu->parent.child_count + 1) % menu->parent.child_count;
            return false;
        case menu_input_left:
        case menu_input_right:
            if (selected->flags & menu_flag_is_value) {
                display_menu_state.dirty = true;
                return selected->child.menu_handler(input);
            }
            return false;
        default:
            return false;
    }
}

bool process_record_menu(uint16_t keycode, keyrecord_t *record) {
    if (keycode == DISPLAY_MENU && record->event.pressed && !display_menu_state.is_in_menu) {
        display_menu_state.is_in_menu     = true;
        display_menu_state.selected_child = 0;
        return false;
    }

    if (display_menu_state.is_in_menu) {
        if (record->event.pressed) {
            switch (keycode) {
                case DISPLAY_MENU:
                    return menu_handle_input(menu_input_exit);
                case KC_ESC:
                    return menu_handle_input(menu_input_back);
                case KC_SPACE:
                case KC_ENTER:
                case KC_RETURN:
                    return menu_handle_input(menu_input_enter);
                case KC_UP:
                case KC_W:
                    return menu_handle_input(menu_input_up);
                case KC_DOWN:
                case KC_S:
                    return menu_handle_input(menu_input_down);
                case KC_LEFT:
                case KC_A:
                    return menu_handle_input(menu_input_left);
                case KC_RIGHT:
                case KC_D:
                    return menu_handle_input(menu_input_right);
                default:
                    return false;
            }
        }
        return false;
    }

    return true;
}

extern painter_font_handle_t font_thintel, font_mono, font_oled;

bool render_menu(painter_device_t display, uint16_t start_x, uint16_t start_y, uint16_t width, uint16_t height) {
    static menu_state_t last_state;
    if (memcmp(&last_state, &display_menu_state, sizeof(menu_state_t)) == 0) {
        return display_menu_state.is_in_menu;
    }

    display_menu_state.dirty = false;
    memcpy(&last_state, &display_menu_state, sizeof(menu_state_t));

    uint16_t render_width  = width - start_x;
    uint16_t render_height = height - start_y;

    if (display_menu_state.is_in_menu) {
        qp_rect(display, start_x, start_y, render_width - 1, render_height - 1, 0, 0, 0, true);

        // uint8_t       hue      = rgb_matrix_get_hue();
        menu_entry_t *menu     = get_current_menu();
        menu_entry_t *selected = get_selected_menu_item();

        uint16_t y = start_y;
        qp_rect(display, start_x, y, render_width, y + 3, 0, 0, 255, true);
        y += 6;
        qp_drawtext(display, start_x + 4, y, font_oled, menu->text);
        y += font_oled->line_height + 2;
        qp_rect(display, start_x, y, render_width, y + 3, 0, 0, 255, true);
        y += 6;
        for (int i = 0; i < menu->parent.child_count; ++i) {
            menu_entry_t *child = &menu->parent.children[i];
            uint16_t      x     = start_x + 8;
            if (child == selected) {
                x += qp_drawtext_recolor(display, x, y, font_oled, child->text, HSV_GREEN, 85, 255, 0);
            } else {
                x += qp_drawtext_recolor(display, x, y, font_oled, child->text, 0, 0, 255, 0, 255, 0);
            }
            if (child->flags & menu_flag_is_parent) {
                qp_drawtext(display, 8 + x, y, font_oled, "  >");
            }
            if (child->flags & menu_flag_is_value) {
                char buf[32] = {0};
                child->child.display_handler(buf, sizeof(buf));
                qp_drawtext(display, 8 + x, y, font_oled, buf);
            }
            y += font_oled->line_height + 2;
            qp_rect(display, start_x, y, render_width, y, 0, 0, 255, true);
            y += 3;
        }
        return true;
    }
    return false;
}
