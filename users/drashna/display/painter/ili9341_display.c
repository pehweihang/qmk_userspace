// Copyright 2024 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
// Copyright 2018-2024 Nick Brassel (@tzarc)
// SPDX-License-Identifier: GPL-3.0-or-later

#include "drashna.h"
#include "qp.h"
#include "qp_surface.h"
#include "qp_ili9xxx_opcodes.h"
#include "qp_comms.h"
#include "display/painter/painter.h"
#include "display/painter/ili9341_display.h"
#include "display/painter/menu.h"
#ifdef SPLIT_KEYBOARD
#    include "split_util.h"
#endif // SPLIT_KEYBOARD
#ifdef CUSTOM_SPLIT_TRANSPORT_SYNC
#    include "split/transport_sync.h"
#endif
#ifdef RTC_ENABLE
#    include "features/rtc/rtc.h"
#endif
#ifdef LAYER_MAP_ENABLE
#    include "features/layer_map.h"
#endif
#ifdef CUSTOM_UNICODE_ENABLE
#    include "keyrecords/unicode.h"
#endif // CUSTOM_UNICODE_ENABLE

#include <math.h>
#include <stdio.h>
#include <ctype.h>

painter_device_t ili9341_display, menu_surface;

painter_font_handle_t font_thintel, font_mono, font_oled;

painter_image_handle_t frame_top, frame_bottom;
painter_image_handle_t lock_caps_on, lock_caps_off;
painter_image_handle_t lock_num_on, lock_num_off;
painter_image_handle_t lock_scrl_on, lock_scrl_off;
painter_image_handle_t windows_logo, apple_logo, linux_logo;
painter_image_handle_t shift_icon, control_icon, alt_icon, command_icon, windows_icon;
painter_image_handle_t mouse_icon;
painter_image_handle_t gamepad_icon;

#define SURFACE_MENU_WIDTH  236
#define SURFACE_MENU_HEIGHT 121

uint8_t menu_buffer[SURFACE_REQUIRED_BUFFER_BYTE_SIZE(SURFACE_MENU_WIDTH, SURFACE_MENU_HEIGHT, 16)];

/**
 * @brief Draws the initial frame on the screen
 *
 * @param display
 */
void render_frame(painter_device_t display) {
    uint16_t width;
    uint16_t height;
    qp_get_geometry(ili9341_display, &width, &height, NULL, NULL, NULL);

    HSV hsv = painter_get_hsv();
    // frame top
    qp_drawimage_recolor(ili9341_display, 1, 2, frame_top, hsv.h, hsv.s, hsv.v, 0, 0, 0);
    // lines for frame sides
    qp_line(ili9341_display, 1, frame_top->height, 1, height - frame_bottom->height, hsv.h, hsv.s, hsv.v);
    qp_line(ili9341_display, width - 2, frame_top->height, width - 2, height - frame_bottom->height, hsv.h, hsv.s,
            hsv.v);

    qp_line(ili9341_display, 2, 54, width - 3, 54, hsv.h, hsv.s, hsv.v);
    qp_line(ili9341_display, 80, 54, 80, 106, hsv.h, hsv.s, hsv.v);

    // lines for unicode typing mode and mode
    qp_line(ili9341_display, 80, 80, width - 3, 80, hsv.h, hsv.s, hsv.v);
    qp_line(ili9341_display, 149, 80, 149, 106, hsv.h, hsv.s, hsv.v);

    // lines for mods and OS detection
    qp_line(ili9341_display, 2, 107, width - 3, 107, hsv.h, hsv.s, hsv.v);
    qp_line(ili9341_display, 155, 107, 155, 122, hsv.h, hsv.s, hsv.v);
    // lines for autocorrect and layers
    qp_line(ili9341_display, 2, 122, width - 3, 122, hsv.h, hsv.s, hsv.v);
    qp_line(ili9341_display, 121, 122, 121, 171, hsv.h, hsv.s, hsv.v);
    qp_line(ili9341_display, 186, 122, 186, 171, hsv.h, hsv.s, hsv.v);
    // frame bottom
    qp_drawimage_recolor(ili9341_display, 1, height - frame_bottom->height, frame_bottom, hsv.h, hsv.s, hsv.v, 0, 0, 0);

    char title[50] = {0};
    snprintf(title, sizeof(title), "%s", PRODUCT);
    uint8_t title_width = qp_textwidth(font_thintel, title);
    if (title_width > (width - 55)) {
        title_width = width - 55;
    }
    uint8_t title_xpos = (width - title_width) / 2;
    qp_drawtext_recolor(ili9341_display, title_xpos, 4, font_thintel,
                        truncate_text(title, title_width, font_thintel, false, false), 0, 0, 0, hsv.h, hsv.s, hsv.v);
}

__attribute__((weak)) bool init_display_ili9341_inversion(void) {
    return false;
}

/**
 * @brief Initializes the display, clears it and sets frame and title
 *
 */
void init_display_ili9341(void) {
    font_thintel = qp_load_font_mem(font_thintel15);
    font_mono    = qp_load_font_mem(font_ProggyTiny15);
    font_oled    = qp_load_font_mem(font_oled_font);

    windows_logo = qp_load_image_mem(gfx_windows_logo);
    apple_logo   = qp_load_image_mem(gfx_apple_logo);
    linux_logo   = qp_load_image_mem(gfx_linux_logo);

    shift_icon   = qp_load_image_mem(gfx_shift_icon);
    control_icon = qp_load_image_mem(gfx_control_icon);
    alt_icon     = qp_load_image_mem(gfx_alt_icon);
    command_icon = qp_load_image_mem(gfx_command_icon);
    windows_icon = qp_load_image_mem(gfx_windows_icon);

    mouse_icon   = qp_load_image_mem(gfx_mouse_icon);
    gamepad_icon = qp_load_image_mem(gfx_gamepad_24x24);

    ili9341_display =
        qp_ili9341_make_spi_device(240, 320, DISPLAY_CS_PIN, DISPLAY_DC_PIN, DISPLAY_RST_PIN, DISPLAY_SPI_DIVIDER, 0);
    menu_surface = qp_make_rgb565_surface(SURFACE_MENU_WIDTH, SURFACE_MENU_HEIGHT, menu_buffer);

    wait_ms(50);

    uint16_t width;
    uint16_t height;

    qp_init(ili9341_display, QP_ROTATION_0);
    qp_init(menu_surface, QP_ROTATION_0);
    qp_get_geometry(ili9341_display, &width, &height, NULL, NULL, NULL);
    qp_clear(ili9341_display);
    qp_rect(ili9341_display, 0, 0, width - 1, height - 1, 0, 0, 0, true);

    // if needs inversion, run it only afetr the clear and rect functions or otherwise it won't work
    if (init_display_ili9341_inversion()) {
        qp_comms_start(ili9341_display);
        qp_comms_command(ili9341_display, ILI9XXX_CMD_INVERT_ON);
        qp_comms_stop(ili9341_display);
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Initial render of frame/logo

    if (is_keyboard_master()) {
        frame_top    = qp_load_image_mem(gfx_frame_top);
        frame_bottom = qp_load_image_mem(gfx_frame_bottom);
        render_frame(ili9341_display);
    }
    qp_power(ili9341_display, true);
    qp_flush(ili9341_display);
}

void ili9341_display_power(bool on) {
    qp_power(ili9341_display, on);
}

__attribute__((weak)) void ili9341_draw_user(void) {
    bool hue_redraw = false;

    static HSV     last_hsv        = {0};
    static uint8_t last_hue_offset = 0;
    HSV            curr_hsv        = painter_get_hsv();
    if (memcmp(&last_hsv, &curr_hsv, sizeof(HSV)) != 0) {
        memcpy(&last_hsv, &curr_hsv, sizeof(HSV));
        hue_redraw = true;
    }
    if (last_hue_offset != painter_get_hue_offset()) {
        last_hue_offset = painter_get_hue_offset();
        hue_redraw      = true;
    }
    const uint8_t offset_hue   = curr_hsv.h + painter_get_hue_offset();
    const uint8_t disabled_val = curr_hsv.v / 2;

    uint16_t width;
    uint16_t height;
    qp_get_geometry(ili9341_display, &width, &height, NULL, NULL, NULL);

#if defined(RGB_MATRIX_ENABLE) || defined(RGBLIGHT_ENABLE)
#    if defined(RGB_MATRIX_ENABLE)
    if (has_rgb_matrix_config_changed()) {
        display_menu_set_dirty();
    }
#    endif
#    if defined(RGBLIGHT_ENABLE)
    if (has_rgblight_config_changed()) {
        display_menu_set_dirty();
    }
#    endif
#endif

    if (is_keyboard_master()) {
        char     buf[50] = {0};
        uint16_t ypos    = 18;
        uint16_t xpos    = 5;

        if (hue_redraw) {
            render_frame(ili9341_display);
            qp_rect(ili9341_display, width - mouse_icon->width - 6, 5, width - 6, 5 + mouse_icon->height - 1, 0, 0, 0,
                    true);
            qp_drawimage_recolor(ili9341_display, width - mouse_icon->width - 6, 5, mouse_icon, curr_hsv.h, curr_hsv.s,
                                 curr_hsv.v, 0, 0, 0);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Matrix Scan rate

        static uint32_t last_scan_update = 0;
        if (hue_redraw || last_scan_update != get_matrix_scan_rate()) {
            last_scan_update = get_matrix_scan_rate();
            snprintf(buf, sizeof(buf), "SCANS: %-5lu", last_scan_update);
            qp_drawtext_recolor(ili9341_display, xpos, ypos, font_oled, buf, curr_hsv.h, curr_hsv.s, curr_hsv.v, 0, 0,
                                0);
        }
        ypos += font_oled->line_height + 4;

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //  WPM

#ifdef WPM_ENABLE
        bool            wpm_redraw      = false;
        static uint32_t last_wpm_update = 0;
        if (timer_elapsed32(last_wpm_update) > 250) {
            last_wpm_update = timer_read32();
            wpm_redraw      = true;
        }
        if (hue_redraw || wpm_redraw) {
            snprintf(buf, sizeof(buf), "WPM: %3u", get_current_wpm());
            xpos += qp_drawtext_recolor(ili9341_display, xpos, ypos, font_oled, buf, curr_hsv.h, curr_hsv.s, curr_hsv.v,
                                        0, 0, 0);
        }
#endif // WPM_ENABLE

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Keymap config (nkro, autocorrect, oneshots)

        ypos                                        = 54 + 4;
        static keymap_config_t last_keymap_config   = {0};
        bool                   keymap_config_redraw = false;
        if (last_keymap_config.raw != keymap_config.raw) {
            last_keymap_config.raw = keymap_config.raw;
            keymap_config_redraw   = true;
        }
        if (hue_redraw || keymap_config_redraw) {
            xpos = 80 + 4;
            qp_drawimage(ili9341_display, xpos, ypos + 2,
                         last_keymap_config.swap_lctl_lgui ? apple_logo : windows_logo);
            xpos += windows_logo->width + 5;
            xpos += qp_drawtext_recolor(ili9341_display, xpos, ypos, font_oled, (const char *)"NKRO",
                                        last_keymap_config.nkro ? offset_hue : curr_hsv.h, curr_hsv.s,
                                        last_keymap_config.nkro ? curr_hsv.v : disabled_val, 0, 0, 0) +
                    5;
            xpos += qp_drawtext_recolor(ili9341_display, xpos, ypos, font_oled, (const char *)"CRCT",
                                        last_keymap_config.autocorrect_enable ? offset_hue : curr_hsv.h, curr_hsv.s,
                                        last_keymap_config.autocorrect_enable ? curr_hsv.v : disabled_val, 0, 0, 0) +
                    5;
            xpos += qp_drawtext_recolor(ili9341_display, xpos, ypos, font_oled, (const char *)"1SHT",
                                        last_keymap_config.oneshot_enable ? offset_hue : curr_hsv.h, curr_hsv.s,
                                        last_keymap_config.oneshot_enable ? curr_hsv.v : disabled_val, 0, 0, 0);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Device Config (Audio, Audio Clicky, Host Driver lock, Swap Hands)

        ypos += font_oled->line_height + 4;
        static user_runtime_config_t last_user_state = {0};
        if (hue_redraw || last_user_state.raw != user_state.raw) {
            last_user_state.raw = user_state.raw;
            xpos                = 80 + 4 + windows_logo->width + 5;
            xpos += qp_drawtext_recolor(ili9341_display, xpos, ypos, font_oled, (const char *)"AUDIO",
                                        last_user_state.audio_enable ? offset_hue : curr_hsv.h, curr_hsv.s,
                                        last_user_state.audio_enable ? curr_hsv.v : disabled_val, 0, 0, 0) +
                    5;
            xpos += qp_drawtext_recolor(ili9341_display, xpos, ypos, font_oled, (const char *)"CLCK",
                                        last_user_state.audio_clicky_enable ? offset_hue : curr_hsv.h, curr_hsv.s,
                                        last_user_state.audio_clicky_enable ? curr_hsv.v : disabled_val, 0, 0, 0) +
                    5;
            xpos += qp_drawtext_recolor(ili9341_display, xpos, ypos, font_oled, (const char *)"HOST",
                                        last_user_state.host_driver_disabled ? offset_hue : curr_hsv.h, curr_hsv.s,
                                        last_user_state.host_driver_disabled ? curr_hsv.v : disabled_val, 0, 0, 0) +
                    5;
            xpos += qp_drawtext_recolor(ili9341_display, xpos, ypos, font_oled, (const char *)"SWAP",
                                        last_user_state.swap_hands ? offset_hue : curr_hsv.h, curr_hsv.s,
                                        last_user_state.swap_hands ? curr_hsv.v : disabled_val, 0, 0, 0);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // LED Lock indicator (text)

        // ypos = 54+4
        // static led_t last_led_state = {0};
        // if (hue_redraw || last_led_state.raw != host_keyboard_led_state().raw) {
        //     last_led_state.raw           = host_keyboard_led_state().raw;
        //     static uint16_t max_led_xpos = 0;
        //     xpos                         = 5;
        //     xpos += qp_drawtext_recolor(ili9341_display, xpos, ypos, font_oled, (const char *)"Locks:", curr_hsv.h,
        //                                 curr_hsv.s, curr_hsv.v, 0, 0, 0) +
        //             5;
        //     xpos += qp_drawtext_recolor(ili9341_display, xpos, ypos, font_oled, (const char *)"CAPS",
        //                                 last_led_state.caps_lock ? offset_hue : curr_hsv.h, curr_hsv.s,
        //                                 last_led_state.caps_lock ? curr_hsv.v : disabled_val, 0, 0, 0) +
        //             5;
        //     xpos += qp_drawtext_recolor(ili9341_display, xpos, ypos, font_oled, (const char *)"SCRL",
        //                                 last_led_state.scroll_lock ? offset_hue : curr_hsv.h, curr_hsv.s,
        //                                 last_led_state.scroll_lock ? curr_hsv.v : disabled_val, 0, 0, 0) +
        //             5;
        //     xpos += qp_drawtext_recolor(ili9341_display, xpos, ypos, font_oled, (const char *)"NUM",
        //                                 last_led_state.num_lock ? offset_hue : curr_hsv.h, curr_hsv.s,
        //                                 last_led_state.num_lock ? curr_hsv.v : disabled_val, 0, 0, 0);
        //     if (max_led_xpos < xpos) {
        //         max_led_xpos = xpos;
        //     }
        //     qp_rect(ili9341_display, xpos, ypos, max_led_xpos, ypos + font_oled->line_height, 0, 0, 0, true);
        // }

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Pointing Device CPI

        ypos = 54 + 4;
#if defined(POINTING_DEVICE_ENABLE) && \
    (defined(KEYBOARD_bastardkb_charybdis) || defined(KEYBOARD_handwired_tractyl_manuform))
        static uint16_t last_cpi   = {0xFFFF};
        uint16_t        curr_cpi   = charybdis_get_pointer_sniping_enabled() ? charybdis_get_pointer_sniping_dpi()
                                                                             : charybdis_get_pointer_default_dpi();
        bool            cpi_redraw = false;
        if (last_cpi != curr_cpi) {
            last_cpi   = curr_cpi;
            cpi_redraw = true;
        }
        static uint16_t max_cpi_xpos = 0;
        if (hue_redraw || cpi_redraw) {
            xpos = 5;
            snprintf(buf, sizeof(buf), "CPI: %5u", curr_cpi);
            xpos += qp_drawtext_recolor(ili9341_display, xpos, ypos, font_oled, buf, curr_hsv.h, curr_hsv.s, curr_hsv.v,
                                        0, 0, 0);
            if (max_cpi_xpos < xpos) {
                max_cpi_xpos = xpos;
            }
            qp_rect(ili9341_display, xpos, ypos, max_cpi_xpos, ypos + font_oled->line_height, 0, 0, 0, true);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Pointing Device Drag Scroll

        bool            ds_state_redraw = false;
        static uint32_t last_ds_state   = 0xFFFFFFFF;
        if (last_ds_state != charybdis_get_pointer_dragscroll_enabled()) {
            last_ds_state   = charybdis_get_pointer_dragscroll_enabled();
            ds_state_redraw = true;
        }

        ypos += font_oled->line_height + 4;
        static uint16_t max_dss_xpos = 0;
        if (hue_redraw || ds_state_redraw) {
            xpos = 5;
            xpos +=
                qp_drawtext_recolor(ili9341_display, xpos, ypos, font_oled, "Drag-Scroll",
                                    charybdis_get_pointer_dragscroll_enabled() ? offset_hue : curr_hsv.h, curr_hsv.s,
                                    charybdis_get_pointer_dragscroll_enabled() ? curr_hsv.v : disabled_val, 0, 0, 0);
            if (max_dss_xpos < xpos) {
                max_dss_xpos = xpos;
            }
            qp_rect(ili9341_display, xpos, ypos, max_dss_xpos, ypos + font_oled->line_height, 0, 0, 0, true);
        }
        ypos += font_oled->line_height + 4;

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Pointing Device Auto Mouse Layer

#    ifdef POINTING_DEVICE_AUTO_MOUSE_ENABLE
        bool           am_state_redraw = false;
        static uint8_t last_am_state   = 0xFF;
        if (last_am_state != get_auto_mouse_enable()) {
            last_am_state   = get_auto_mouse_enable();
            am_state_redraw = true;
        }

        static uint16_t max_ams_xpos = 0;
        if (hue_redraw || am_state_redraw) {
            xpos = 5;
            xpos += qp_drawtext_recolor(ili9341_display, xpos, ypos, font_oled, "Auto Layer",
                                        get_auto_mouse_enable() ? offset_hue : curr_hsv.h, curr_hsv.s,
                                        get_auto_mouse_enable() ? curr_hsv.v : disabled_val, 0, 0, 0);
            if (max_ams_xpos < xpos) {
                max_ams_xpos = xpos;
            }
            qp_rect(ili9341_display, xpos, ypos, max_ams_xpos, ypos + font_oled->line_height, 0, 0, 0, true);
        }
        ypos += font_oled->line_height + 4;
#    endif

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Pointing Device Sniping mode

        bool           sp_state_redraw = false;
        static uint8_t last_sp_state   = 0xFF;
        if (last_sp_state != charybdis_get_pointer_sniping_enabled()) {
            last_sp_state   = charybdis_get_pointer_sniping_enabled();
            sp_state_redraw = true;
        }
        static uint16_t max_sps_xpos = 0;
        if (hue_redraw || sp_state_redraw) {
            xpos = 5;
            xpos += qp_drawtext_recolor(ili9341_display, xpos, ypos, font_oled, "Sniping",
                                        charybdis_get_pointer_sniping_enabled() ? offset_hue : curr_hsv.h, curr_hsv.s,
                                        charybdis_get_pointer_sniping_enabled() ? curr_hsv.v : disabled_val, 0, 0, 0);
            if (max_sps_xpos < xpos) {
                max_sps_xpos = xpos;
            }
            qp_rect(ili9341_display, xpos, ypos, max_sps_xpos, ypos + font_oled->line_height, 0, 0, 0, true);
        }
#endif // POINTING_DEVICE_ENABLE

#ifdef CUSTOM_UNICODE_ENABLE
        ypos                             = 80 + 4;
        static uint8_t last_unicode_mode = UNICODE_MODE_COUNT;
        if (hue_redraw || last_unicode_mode != get_unicode_input_mode()) {
            last_unicode_mode   = get_unicode_input_mode();
            xpos                = 80 + 4;
            uint8_t xpos_offset = xpos +
                                  qp_drawtext_recolor(ili9341_display, xpos, ypos, font_oled, "Unicode", curr_hsv.h,
                                                      curr_hsv.s, curr_hsv.v, 0, 0, 0) +
                                  4;
            switch (last_unicode_mode) {
                case UNICODE_MODE_WINCOMPOSE:
                case UNICODE_MODE_WINDOWS:
                    qp_drawimage(ili9341_display, xpos_offset, ypos + 2, windows_logo);
                    break;
                case UNICODE_MODE_MACOS:
                    qp_drawimage(ili9341_display, xpos_offset, ypos + 2, apple_logo);
                    break;
                case UNICODE_MODE_LINUX:
                case UNICODE_MODE_BSD:
                case UNICODE_MODE_EMACS:
                    qp_drawimage(ili9341_display, xpos_offset, ypos + 2, linux_logo);
                    break;
            }
            ypos += font_oled->line_height + 4;
            qp_drawtext_recolor(ili9341_display, xpos + 8, ypos, font_oled, "Mode", curr_hsv.h, curr_hsv.s, curr_hsv.v,
                                0, 0, 0);
        }

        ypos                                    = 80 + 4;
        static uint8_t last_unicode_typing_mode = 0;
        if (hue_redraw || last_unicode_typing_mode != unicode_typing_mode) {
            last_unicode_typing_mode = unicode_typing_mode;
            xpos                     = 149 + 4;
            qp_drawtext_recolor(ili9341_display, xpos, ypos, font_oled, "Typing Mode:", curr_hsv.h, curr_hsv.s,
                                curr_hsv.v, 0, 0, 0);
            ypos += font_oled->line_height + 4;
            snprintf(buf, sizeof(buf), "%14s", unicode_mode_str[last_unicode_typing_mode]);
            qp_drawtext_recolor(ili9341_display, xpos, ypos, font_oled, buf, offset_hue, curr_hsv.s, curr_hsv.v, 0, 0,
                                0);
        }
#endif // CUSTOM_UNICODE_ENABLE

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Mods

        ypos                        = 107 + 3;
        static uint8_t last_mods    = {0};
        uint8_t        current_mods = get_mods() | get_weak_mods() | get_oneshot_mods();
        if (hue_redraw || last_mods != current_mods || keymap_config_redraw) {
            last_mods = current_mods;
            xpos      = 5;
            xpos += qp_drawtext_recolor(ili9341_display, xpos, ypos + 1, font_oled, "Modifiers:", curr_hsv.h,
                                        curr_hsv.s, curr_hsv.v, 0, 0, 0) +
                    2;

            if (qp_drawimage_recolor(ili9341_display, xpos, ypos, shift_icon,
                                     last_mods & MOD_BIT_LSHIFT ? offset_hue : curr_hsv.h, curr_hsv.s,
                                     last_mods & MOD_BIT_LSHIFT ? curr_hsv.v : disabled_val, 0, 0, 0)) {
                xpos += shift_icon->width + 2;
            }
            if (qp_drawimage_recolor(ili9341_display, xpos, ypos,
                                     keymap_config.swap_lctl_lgui ? command_icon : windows_icon,
                                     last_mods & MOD_BIT_LGUI ? offset_hue : curr_hsv.h, curr_hsv.s,
                                     last_mods & MOD_BIT_LGUI ? curr_hsv.v : disabled_val, 0, 0, 0)) {
                xpos += windows_icon->width + 2;
            }
            if (qp_drawimage_recolor(ili9341_display, xpos, ypos, alt_icon,
                                     last_mods & MOD_BIT_LALT ? offset_hue : curr_hsv.h, curr_hsv.s,
                                     last_mods & MOD_BIT_LALT ? curr_hsv.v : disabled_val, 0, 0, 0)) {
                xpos += alt_icon->width + 2;
            }
            if (qp_drawimage_recolor(ili9341_display, xpos, ypos, control_icon,
                                     last_mods & MOD_BIT_LCTRL ? offset_hue : curr_hsv.h, curr_hsv.s,
                                     last_mods & MOD_BIT_LCTRL ? curr_hsv.v : disabled_val, 0, 0, 0)) {
                xpos += control_icon->width + 2;
            }
            if (qp_drawimage_recolor(ili9341_display, xpos, ypos, control_icon,
                                     last_mods & MOD_BIT_RCTRL ? offset_hue : curr_hsv.h, curr_hsv.s,
                                     last_mods & MOD_BIT_RCTRL ? curr_hsv.v : disabled_val, 0, 0, 0)) {
                xpos += control_icon->width + 2;
            }
            if (qp_drawimage_recolor(ili9341_display, xpos, ypos, alt_icon,
                                     last_mods & MOD_BIT_RALT ? offset_hue : curr_hsv.h, curr_hsv.s,
                                     last_mods & MOD_BIT_RALT ? curr_hsv.v : disabled_val, 0, 0, 0)) {
                xpos += alt_icon->width + 2;
            }
            if (qp_drawimage_recolor(ili9341_display, xpos, ypos,
                                     keymap_config.swap_rctl_rgui ? command_icon : windows_icon,
                                     last_mods & MOD_BIT_RGUI ? offset_hue : curr_hsv.h, curr_hsv.s,
                                     last_mods & MOD_BIT_RGUI ? curr_hsv.v : disabled_val, 0, 0, 0)) {
                xpos += windows_icon->width + 2;
            }
            if (qp_drawimage_recolor(ili9341_display, xpos, ypos, shift_icon,
                                     last_mods & MOD_BIT_RSHIFT ? offset_hue : curr_hsv.h, curr_hsv.s,
                                     last_mods & MOD_BIT_RSHIFT ? curr_hsv.v : disabled_val, 0, 0, 0)) {
                xpos += shift_icon->width + 2;
            }
        }

#ifdef OS_DETECTION_ENABLE
        ypos                                    = 107 + 4;
        xpos                                    = 159;
        static os_variant_t last_detected_os    = {0};
        os_variant_t        current_detected_os = detected_host_os();
        if (hue_redraw || last_detected_os != current_detected_os) {
            last_detected_os = current_detected_os;

            snprintf(buf, sizeof(buf), "OS: %9s", os_variant_to_string(current_detected_os));
            qp_drawtext_recolor(ili9341_display, xpos, ypos, font_oled, buf, curr_hsv.h, curr_hsv.s, curr_hsv.v, 0, 0,
                                0);
        }
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //  RGB Light Settings
#endif // OS_DETECTION_ENABLE

#if 0
#    if defined(RGBLIGHT_ENABLE)
        ypos += font_oled->line_height + 4;
        if (hue_redraw || rgb_redraw) {
            static uint16_t max_rgb_xpos = 0;
            xpos                         = 5;
            snprintf(buf, sizeof(buf), "RGB Light Mode: %s", rgblight_get_effect_name());
            snprintf(buf, sizeof(buf), "%s", truncate_text(buf, width - 7, font_oled, false, false));

            xpos += qp_drawtext_recolor(ili9341_display, xpos, ypos, font_oled, buf, curr_hsv.h, curr_hsv.s, curr_hsv.v,
                                        0, 0, 0);
            if (max_rgb_xpos < xpos) {
                max_rgb_xpos = xpos;
            }
            qp_rect(ili9341_display, xpos, ypos, max_rgb_xpos, ypos + font_oled->line_height, 0, 0, 0, true);

            ypos += font_oled->line_height + 4;
            static uint16_t max_hsv_xpos = 0;
            xpos                         = 5;
            snprintf(buf, sizeof(buf), "RGB Light HSV: %3d, %3d, %3d", rgblight_get_hue(), rgblight_get_sat(),
                     rgblight_get_val());
            xpos += qp_drawtext_recolor(ili9341_display, xpos, ypos, font_oled, buf, curr_hsv.h, curr_hsv.s, curr_hsv.v,
                                        0, 0, 0);
            if (max_hsv_xpos < xpos) {
                max_hsv_xpos = xpos;
            }
            qp_rect(ili9341_display, xpos, ypos, max_hsv_xpos, ypos + font_oled->line_height, 0, 0, 0, true);
            qp_rect(ili9341_display, max_hsv_xpos + 5, ypos, max_hsv_xpos + 25, ypos + font_oled->line_height - 1,
                    rgblight_get_hue(), rgblight_get_sat(), (uint8_t)(rgblight_get_val() * 0xFF / RGBLIGHT_LIMIT_VAL),
                    true);
        } else {
            // we called ypos inside the function ... to make sure we don't skip a line on future passes ....
            ypos += font_oled->line_height + 4;
        }
#    endif // RGBLIGHT_ENABLE

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // RGB Matrix Settings

#    if defined(RGB_MATRIX_ENABLE)
        ypos += font_oled->line_height + 4;
        if (hue_redraw || rgb_redraw) {
            static uint16_t max_rgb_xpos = 0;
            xpos                         = 5;
            snprintf(buf, sizeof(buf), "RGB Matrix Mode: %s", rgb_matrix_get_effect_name());
            snprintf(buf, sizeof(buf), "%s", truncate_text(buf, width - 7, font_oled, false, false));

            xpos += qp_drawtext_recolor(ili9341_display, xpos, ypos, font_oled, buf, curr_hsv.h, curr_hsv.s, curr_hsv.v,
                                        0, 0, 0);
            if (max_rgb_xpos < xpos) {
                max_rgb_xpos = xpos;
            }
            qp_rect(ili9341_display, xpos, ypos, max_rgb_xpos, ypos + font_oled->line_height, 0, 0, 0, true);

            ypos += font_oled->line_height + 4;
            static uint16_t max_hsv_xpos = 0;
            xpos                         = 5;
            snprintf(buf, sizeof(buf), "RGB Matrix HSV: %3d, %3d, %3d", rgb_matrix_get_hue(), rgb_matrix_get_sat(),
                     rgb_matrix_get_val());
            xpos += qp_drawtext_recolor(ili9341_display, xpos, ypos, font_oled, buf, curr_hsv.h, curr_hsv.s, curr_hsv.v,
                                        0, 0, 0);
            if (max_hsv_xpos < xpos) {
                max_hsv_xpos = xpos;
            }
            qp_rect(ili9341_display, xpos, ypos, max_hsv_xpos, ypos + font_oled->line_height, 0, 0, 0, true);
            qp_rect(ili9341_display, max_hsv_xpos + 5, ypos, max_hsv_xpos + 25, ypos + font_oled->line_height - 1,
                    rgb_matrix_get_hue(), rgb_matrix_get_sat(),
                    (uint8_t)(rgb_matrix_get_val() * 0xFF / RGB_MATRIX_MAXIMUM_BRIGHTNESS), true);
        } else {
            // we called ypos inside the function ... to make sure we don't skip a line on future passes ....
            ypos += font_oled->line_height + 4;
        }
#    endif // RGB_MATRIX_ENABLE
#endif
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //  Default layer state

        ypos                                    = 122 + 4;
        xpos                                    = 125;
        bool                 layer_state_redraw = false, dl_state_redraw = false;
        static layer_state_t last_layer_state = 0, last_dl_state = 0;
        if (last_layer_state != layer_state) {
            last_layer_state   = layer_state;
            layer_state_redraw = true;
        }
        if (last_dl_state != default_layer_state) {
            last_dl_state   = default_layer_state;
            dl_state_redraw = true;
        }

        if (hue_redraw || dl_state_redraw || layer_state_redraw) {
            qp_drawtext_recolor(ili9341_display, xpos, ypos, font_oled, "Layout: ", curr_hsv.h, curr_hsv.s, curr_hsv.v,
                                0, 0, 0);
            ypos += font_oled->line_height + 4;
            snprintf(buf, sizeof(buf), "%10s", get_layer_name_string(default_layer_state, false, true));
            qp_drawtext_recolor(ili9341_display, xpos, ypos, font_oled, buf, offset_hue, curr_hsv.s, curr_hsv.v, 0, 0,
                                0);
        } else {
            ypos += font_oled->line_height + 4;
        }
        ypos += font_oled->line_height + 4;

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Layer State

        if (hue_redraw || layer_state_redraw) {
            qp_drawtext_recolor(ili9341_display, xpos, ypos, font_oled, "Layer: ", curr_hsv.h, curr_hsv.s, curr_hsv.v,
                                0, 0, 0);
            ypos += font_oled->line_height + 4;
            layer_state_t temp = last_layer_state;
            if (is_gaming_layer_active(last_layer_state)) {
                temp = last_layer_state & ~((layer_state_t)1 << _MOUSE);
            }
            snprintf(buf, sizeof(buf), "%10s", get_layer_name_string(temp, false, false));
            qp_drawtext_recolor(ili9341_display, xpos, ypos, font_oled, buf, offset_hue, curr_hsv.s, curr_hsv.v, 0, 0,
                                0);
            ypos = 122 + 4;
            xpos = 190;
            qp_drawimage_recolor(ili9341_display, xpos, ypos, gamepad_icon, curr_hsv.h, curr_hsv.s,
                                 layer_state_cmp(last_layer_state, _GAMEPAD) ? curr_hsv.v : disabled_val, 0, 0, 0);
            qp_drawimage_recolor(ili9341_display, xpos + gamepad_icon->width + 6, ypos + 4, mouse_icon, curr_hsv.h,
                                 curr_hsv.s, layer_state_cmp(layer_state, _MOUSE) ? curr_hsv.v : disabled_val, 0, 0, 0);
            ypos += gamepad_icon->height + 2;
            qp_drawtext_recolor(ili9341_display, xpos, ypos, font_oled, "Diablo",
                                layer_state_cmp(last_layer_state, _DIABLO) ? 0 : curr_hsv.h, curr_hsv.s,
                                layer_state_cmp(last_layer_state, _DIABLO) ? curr_hsv.v : disabled_val, 0, 0, 0);
            ypos += font_oled->line_height + 2;
            qp_drawtext_recolor(ili9341_display, xpos, ypos, font_oled, "Diablo 2",
                                layer_state_cmp(last_layer_state, _DIABLOII) ? 0 : curr_hsv.h, curr_hsv.s,
                                layer_state_cmp(last_layer_state, _DIABLOII) ? curr_hsv.v : disabled_val, 0, 0, 0);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Autocorrection values

#ifdef AUTOCORRECT_ENABLE
        ypos = 122 + 4;
        extern bool     autocorrect_str_has_changed;
        extern char     autocorrected_str_raw[2][21];
        static uint32_t autocorrect_timer = 0;
        if (timer_elapsed(autocorrect_timer) > 125) {
            autocorrect_timer           = timer_read();
            autocorrect_str_has_changed = true;
        }

        if (hue_redraw || autocorrect_str_has_changed) {
            xpos = 5;
            qp_drawtext_recolor(ili9341_display, xpos, ypos, font_oled, "Autocorrected: ", curr_hsv.h, curr_hsv.s,
                                curr_hsv.v, 0, 0, 0);
            ypos += font_oled->line_height + 4;
            snprintf(buf, sizeof(buf), "%19s", autocorrected_str_raw[0]);
            qp_drawtext_recolor(ili9341_display, xpos, ypos, font_oled, buf, offset_hue, curr_hsv.s, curr_hsv.v, 0, 0,
                                0);

            ypos += font_oled->line_height + 4;
            qp_drawtext_recolor(ili9341_display, xpos, ypos, font_oled, "Original Text: ", curr_hsv.h, curr_hsv.s,
                                curr_hsv.v, 0, 0, 0);
            ypos += font_oled->line_height + 4;
            snprintf(buf, sizeof(buf), "%19s", autocorrected_str_raw[1]);

            qp_drawtext_recolor(ili9341_display, xpos, ypos, font_oled, buf, offset_hue, curr_hsv.s, curr_hsv.v, 0, 0,
                                0);
            autocorrect_str_has_changed = false;
        } else {
            // we called ypos inside the function ... to make sure we don't skip a line on future passes ....
            ypos += (font_oled->line_height + 4) * 3;
        }
#endif // AUTOCORRECT_ENABLE

        ypos += font_oled->line_height + 1;

        // Keylogger
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef DISPLAY_KEYLOGGER_ENABLE // keep at very end
        static uint32_t last_klog_update = 0;
        if (timer_elapsed32(last_klog_update) > 125 || keylogger_has_changed) {
            last_klog_update      = timer_read32();
            keylogger_has_changed = true;
        }

        ypos = height - (font_mono->line_height + 2);
        if (keylogger_has_changed) {
            static int max_klog_xpos = 0;
            xpos                     = 27;
            snprintf(buf, sizeof(buf), "Keylogger: %s", display_keylogger_string);

            xpos += qp_drawtext_recolor(ili9341_display, xpos, ypos, font_mono, buf, 0, 255, 0, curr_hsv.h, curr_hsv.s,
                                        curr_hsv.v);

            if (max_klog_xpos < xpos) {
                max_klog_xpos = xpos;
            }
            // qp_rect(ili9341_display, xpos, ypos, max_klog_xpos, ypos + font->line_height, 0, 0, 255, true);
            keylogger_has_changed = false;
        }
#endif // DISPLAY_KEYLOGGER_ENABLE

        // RTC
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef RTC_ENABLE
        ypos -= (font_oled->line_height + 3);
        static uint16_t rtc_timer  = 0;
        bool            rtc_redraw = false;
        if (timer_elapsed(rtc_timer) > 125 && rtc_is_connected()) {
            rtc_timer  = timer_read();
            rtc_redraw = true;
        }
        if (hue_redraw || rtc_redraw) {
            static uint16_t max_rtc_xpos = 0;
            xpos                         = 5;
            if (rtc_is_connected()) {
                snprintf(buf, sizeof(buf), "RTC Date/Time: %s", rtc_read_date_time_str());
            } else {
                snprintf(buf, sizeof(buf), "RTC Device Not Connected");
            }

            uint8_t title_width = qp_textwidth(font_oled, buf);
            if (title_width > (width - 6)) {
                title_width = width - 6;
            }
            uint8_t title_xpos = (width - title_width) / 2;

            xpos += qp_drawtext_recolor(ili9341_display, title_xpos, ypos, font_oled, buf, curr_hsv.h, curr_hsv.s,
                                        curr_hsv.v, 0, 0, 0);
            if (max_rtc_xpos < xpos) {
                max_rtc_xpos = xpos;
            }
            qp_rect(ili9341_display, xpos, ypos, max_rtc_xpos, ypos + font_oled->line_height, 0, 0, 0, true);
        }
#endif // RTC_ENABLE

        static bool force_full_block_redraw = false;
        ypos -= SURFACE_MENU_HEIGHT + 4;
        if (render_menu(menu_surface, 0, 0, SURFACE_MENU_WIDTH, SURFACE_MENU_HEIGHT)) {
            force_full_block_redraw = true;
        } else {
            static uint32_t block_timer  = 0;
            bool            block_redraw = false;
            uint16_t        surface_ypos = 0;
            if (timer_elapsed(block_timer) > 125) {
                block_timer  = timer_read();
                block_redraw = true;
            }

            static uint8_t last_display_mode = 0xFF;
            if (last_display_mode != userspace_config.display_mode) {
                last_display_mode       = userspace_config.display_mode;
                force_full_block_redraw = true;
            }

            if (force_full_block_redraw) {
                qp_rect(menu_surface, 0, 0, SURFACE_MENU_WIDTH - 1, SURFACE_MENU_HEIGHT - 1, 0, 0, 0, true);
                qp_line(menu_surface, 0, 0, SURFACE_MENU_WIDTH - 1, 0, curr_hsv.h, curr_hsv.s, curr_hsv.v);
                qp_line(menu_surface, 0, SURFACE_MENU_HEIGHT - 1, SURFACE_MENU_WIDTH - 1, SURFACE_MENU_HEIGHT - 1,
                        curr_hsv.h, curr_hsv.s, curr_hsv.v);
                force_full_block_redraw = false;
                block_redraw            = true;
            }

            surface_ypos += 3;
            xpos = 3;
            switch (userspace_config.display_mode) {
                case 0:
                    if (hue_redraw || block_redraw || console_log_needs_redraw) {
                        static uint16_t max_line_width = 0;
                        for (uint8_t i = 0; i < DISPLAY_CONSOLE_LOG_LINE_NUM; i++) {
                            xpos = 5;
                            xpos += qp_drawtext_recolor(menu_surface, xpos, surface_ypos, font_oled, logline_ptrs[i],
                                                        curr_hsv.h, curr_hsv.s, curr_hsv.v, 0, 0, 0);
                            if (max_line_width < xpos) {
                                max_line_width = xpos;
                            }
                            qp_rect(menu_surface, xpos, surface_ypos, max_line_width,
                                    surface_ypos + font_oled->line_height, 0, 0, 0, true);
                            surface_ypos += font_oled->line_height + 4;
                        }
                        console_log_needs_redraw = false;
                    }
                    break;
                case 1:
                    //  Layer Map render
                    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef LAYER_MAP_ENABLE
                    if (hue_redraw || block_redraw || layer_map_has_updated) {
                        surface_ypos += font_oled->line_height + 4;
                        uint16_t temp_ypos = surface_ypos;
                        for (uint8_t y = 0; y < LAYER_MAP_ROWS; y++) {
                            xpos = 20;
                            for (uint8_t x = 0; x < LAYER_MAP_COLS; x++) {
                                uint16_t keycode = extract_basic_keycode(layer_map[y][x], NULL, false);
                                wchar_t  code[2] = {0};

                                // if (keycode == UC_IRNY) {
                                //     code[0] = L'⸮';
                                // } else if (keycode == UC_CLUE) {
                                //     code[0] = L'‽'
                                // } else
                                if (keycode > 0xFF) {
                                    keycode = KC_SPC;
                                }
                                if (keycode < ARRAY_SIZE(code_to_name)) {
                                    code[0] = pgm_read_byte(&code_to_name[keycode]);
                                }
                                xpos += qp_drawtext_recolor(menu_surface, xpos, temp_ypos, font_oled, (char *)code,
                                                            curr_hsv.h, curr_hsv.s,
                                                            peek_matrix_layer_map(y, x) ? 0 : curr_hsv.v, offset_hue,
                                                            curr_hsv.s, peek_matrix_layer_map(y, x) ? curr_hsv.v : 0);
                                xpos += qp_drawtext_recolor(menu_surface, xpos, temp_ypos, font_oled, " ", 0, 0, 0, 0,
                                                            0, 0);
                            }
                            temp_ypos += font_oled->line_height + 4;
                        }
                        layer_map_has_updated = false;
                    }
                    break;
#endif
                case 2:
                    if (hue_redraw || block_redraw) {
                        static uint16_t max_font_xpos[3][4] = {0};
                        render_character_set(menu_surface, &xpos, max_font_xpos[0], &surface_ypos, font_thintel,
                                             curr_hsv.h, curr_hsv.s, curr_hsv.v, 0, 0, 0);
                        render_character_set(menu_surface, &xpos, max_font_xpos[1], &surface_ypos, font_mono,
                                             curr_hsv.h, curr_hsv.s, curr_hsv.v, 0, 0, 0);
                        render_character_set(menu_surface, &xpos, max_font_xpos[2], &surface_ypos, font_oled,
                                             curr_hsv.h, curr_hsv.s, curr_hsv.v, 0, 0, 0);
                    }
                    break;
                default:
                    break;
            }
        }
        qp_surface_draw(menu_surface, ili9341_display, 2, ypos, false);

    } else {
#ifdef SPLIT_KEYBOARD
        if (!is_transport_connected()) {
            return;
        }
#endif // SPLIT_KEYBOARD
        static uint8_t display_mode = 0xFF;
        static bool    force_redraw = false;
        if (display_mode != userspace_config.display_logo) {
            display_mode                 = userspace_config.display_logo;
            painter_image_handle_t frame = NULL;

            switch (userspace_config.display_logo) {
                case 0:
                    frame = qp_load_image_mem(gfx_samurai_cyberpunk_minimal_dark_8k_b3_240x320);
                    break;
                case 1:
                    frame = qp_load_image_mem(gfx_anime_girl_jacket_240x320);
                    break;
                case 2:
                    frame = qp_load_image_mem(gfx_asuka_240x320);
                    break;
                case 3:
                    frame = qp_load_image_mem(gfx_neon_genesis_evangelion_initial_machine_00_240x320);
                    break;
                case 4:
                    frame = qp_load_image_mem(gfx_neon_genesis_evangelion_initial_machine_01_240x320);
                    break;
                case 5:
                    frame = qp_load_image_mem(gfx_neon_genesis_evangelion_initial_machine_02_240x320);
                    break;
                case 6:
                    frame = qp_load_image_mem(gfx_neon_genesis_evangelion_initial_machine_03_240x320);
                    break;
                case 7:
                    frame = qp_load_image_mem(gfx_neon_genesis_evangelion_initial_machine_04_240x320);
                    break;
                case 8:
                    frame = qp_load_image_mem(gfx_neon_genesis_evangelion_initial_machine_05_240x320);
                    break;
                case 9:
                    frame = qp_load_image_mem(gfx_neon_genesis_evangelion_initial_machine_06_240x320);
                    break;
                case 10:
                    force_redraw = true;
                    qp_rect(ili9341_display, 0, 0, width - 1, height - 1, 0, 0, 0, true);
                    qp_flush(ili9341_display);
                    return;
            }
            if (frame != NULL) {
                qp_drawimage_recolor(ili9341_display, 0, 0, frame, 0, 0, 255, 0, 0, 0);
                qp_close_image(frame);
            }
        }
        if (userspace_config.display_logo == 10) {
            if (hue_redraw || force_redraw || console_log_needs_redraw) {
                for (uint8_t i = 0; i < DISPLAY_CONSOLE_LOG_LINE_NUM; i++) {
                    static uint16_t max_line_width = 5;
                    uint16_t        xpos = 5, ypos = 3;
                    xpos += qp_drawtext_recolor(ili9341_display, xpos, ypos, font_oled, logline_ptrs[i], curr_hsv.h,
                                                curr_hsv.s, curr_hsv.v, 0, 0, 0);
                    if (max_line_width < xpos) {
                        max_line_width = xpos;
                    }
                    qp_rect(ili9341_display, xpos, ypos, max_line_width, ypos + font_oled->line_height, 0, 0, 0, true);
                    ypos += font_oled->line_height + 4;
                }
                console_log_needs_redraw = force_redraw = false;
            }
        }
    }
    qp_flush(ili9341_display);
}

bool ili9341_display_shutdown(bool jump_to_bootloader) {
    painter_font_handle_t font_proggy = qp_load_font_mem(font_proggy_clean_15);
    uint16_t              width, height, ypos = 240;
    qp_get_geometry(ili9341_display, &width, &height, NULL, NULL, NULL);
    painter_image_handle_t frame = NULL;
    qp_rect(ili9341_display, 0, 0, width - 1, height - 1, 0, 0, 0, true);
    frame = qp_load_image_mem(gfx_qmk_logo_220x220);
    qp_drawimage_recolor(ili9341_display, 10, 10, frame, 0, 0, 255, 0, 0, 0);

    char title[50] = {0};
    snprintf(title, sizeof(title), "%s", "Please Stand By...");
    uint8_t title_width = qp_textwidth(font_proggy, title);
    uint8_t title_xpos  = (width - title_width) / 2;
    qp_drawtext_recolor(ili9341_display, title_xpos, ypos, font_proggy,
                        truncate_text(title, title_width, font_proggy, false, false), 0, 0, 255, 0, 0, 0);
    ypos += font_proggy->line_height + 4;
    snprintf(title, sizeof(title), "%s", jump_to_bootloader ? "Jumping to Bootloader..." : "Shutting Down...");
    title_width = qp_textwidth(font_proggy, title);
    title_xpos  = (width - title_width) / 2;
    qp_drawtext_recolor(ili9341_display, title_xpos, ypos, font_proggy,
                        truncate_text(title, title_width, font_proggy, false, false), 0, 0, 255, 0, 0, 0);
    ypos += font_proggy->line_height + 4;
    if (!eeconfig_is_enabled()) {
        snprintf(title, sizeof(title), "%s", "Reinitialiing EEPROM...");
        title_width = qp_textwidth(font_proggy, title);
        title_xpos  = (width - title_width) / 2;
        qp_drawtext_recolor(ili9341_display, title_xpos, ypos, font_proggy,
                            truncate_text(title, title_width, font_proggy, false, false), 0, 0, 255, 0, 0, 0);
        ypos += font_proggy->line_height + 4;
    }
    qp_flush(ili9341_display);
    return false;
}
