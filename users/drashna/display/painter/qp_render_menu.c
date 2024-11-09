#include "display/menu/menu.h"
#include "display/painter/painter.h"

bool render_menu(painter_device_t display, painter_font_handle_t font, uint16_t start_x, uint16_t start_y,
                 uint16_t width, uint16_t height) {
    static menu_state_t last_state;
    uint8_t             scroll_offset = 0;

    if (memcmp(&last_state, &userspace_runtime_state.menu_state, sizeof(menu_state_t)) == 0) {
        return userspace_runtime_state.menu_state.is_in_menu;
    }

    userspace_runtime_state.menu_state.dirty = false;
    memcpy(&last_state, &userspace_runtime_state.menu_state, sizeof(menu_state_t));

    uint16_t render_width = width - start_x;

    if (userspace_runtime_state.menu_state.is_in_menu) {
        qp_rect(display, start_x, start_y, render_width - 1, height - 1, 0, 0, 0, true);

        menu_entry_t *menu     = get_current_menu();
        menu_entry_t *selected = get_selected_menu_item();
        dual_hsv_t    hsv      = painter_get_dual_hsv();

        uint16_t y = start_y;
        qp_rect(display, start_x, y, render_width, y + 6 + font->line_height + 2, hsv.primary.h, hsv.primary.s,
                hsv.primary.v, true);
        qp_drawtext_recolor(display, start_x + 4, y + 4, font, menu->text, 0, 0, 0, hsv.primary.h, hsv.primary.s,
                            hsv.primary.v);
        y += font->line_height + 8;

        uint8_t visible_entries = (height - y) / (font->line_height + 5);

        scroll_offset = get_menu_scroll_offset(menu, visible_entries);

        for (uint8_t i = scroll_offset; i < menu->parent.child_count && i <= (scroll_offset + visible_entries - 1);
             i++) {
            y += 3;
            menu_entry_t *child = &menu->parent.children[i];
            uint16_t      x     = start_x + 2 + qp_textwidth(font, ">");
            if (child == selected) {
                qp_rect(display, start_x, y - 2, render_width, y + font->line_height + 1, hsv.secondary.h,
                        hsv.secondary.s, hsv.secondary.v, true);
                qp_drawtext_recolor(display, start_x + 1, y, font, ">", 0, 0, 0, hsv.secondary.h, hsv.secondary.s,
                                    hsv.secondary.v);
                x += qp_drawtext_recolor(display, x, y, font,
                                         truncate_text(child->text, render_width, font, false, true), 0, 0, 0,
                                         hsv.secondary.h, hsv.secondary.s, hsv.secondary.v);
            } else {
                if ((i == scroll_offset && scroll_offset > 0) ||
                    (i == scroll_offset + visible_entries - 1 &&
                     scroll_offset + visible_entries < menu->parent.child_count)) {
                    qp_drawtext_recolor(display, start_x + 1, y, font, "+", hsv.primary.h, hsv.primary.s, hsv.primary.v,
                                        0, 255, 0);
                }
                x += qp_drawtext_recolor(display, x, y, font,
                                         truncate_text(child->text, render_width, font, false, true), hsv.primary.h,
                                         hsv.primary.s, hsv.primary.v, 0, 255, 0);
            }
            if (child->flags & menu_flag_is_parent) {
                if (child == selected) {
                    qp_drawtext_recolor(display, render_width - (qp_textwidth(font, ">") + 2), y, font, ">", 0, 0, 0,
                                        hsv.secondary.h, hsv.secondary.s, hsv.secondary.v);
                } else {
                    qp_drawtext_recolor(display, render_width - (qp_textwidth(font, ">") + 2), y, font, ">",
                                        hsv.primary.h, hsv.primary.s, hsv.primary.v, 0, 0, 0);
                }
            }
            if (child->flags & menu_flag_is_value) {
                char buf[32] = {0}, val[29] = {0};
                child->child.display_handler(val, sizeof(val));

                if (child->flags & menu_flag_is_parent) {
                    snprintf(buf, sizeof(buf), " [%s]", val);
                } else {
                    snprintf(buf, sizeof(buf), ": %s", val);
                }
                if (child == selected) {
                    qp_drawtext_recolor(display, x, y, font, buf, 0, 0, 0, hsv.secondary.h, hsv.secondary.s,
                                        hsv.secondary.v);
                } else {
                    qp_drawtext_recolor(display, x, y, font, buf, hsv.primary.h, hsv.primary.s, hsv.primary.v, 0, 0, 0);
                }
            }
            y += font->line_height + 2;
            qp_rect(display, start_x, y, render_width, y, hsv.primary.h, hsv.primary.s, hsv.primary.v, true);
        }
        return true;
    }
    return false;
}
