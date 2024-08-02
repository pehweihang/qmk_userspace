#include "drashna.h"
#include "qp.h"
#include "display/painter/fonts.qff.h"
#include "display/painter/graphics.qgf.h"
#include "display/painter/graphics/assets.h"
#include "qp_comms.h"
#include "display/display.h"
#include "display/painter/ili9341_display.h"
#ifdef CUSTOM_SPLIT_TRANSPORT_SYNC
#    include "split/transport_sync.h"
#endif

#include <math.h>
#include <stdio.h>
#include <ctype.h>

painter_device_t      ili9341_display;
painter_font_handle_t font;

static painter_image_handle_t frame;
static painter_image_handle_t lock_caps_on;
static painter_image_handle_t lock_caps_off;
static painter_image_handle_t lock_num_on;
static painter_image_handle_t lock_num_off;
static painter_image_handle_t lock_scrl_on;
static painter_image_handle_t lock_scrl_off;
static painter_image_handle_t cg_on;
static painter_image_handle_t cg_off;
static painter_image_handle_t mouse_icon;

void init_and_clear(painter_device_t device, painter_rotation_t rotation) {
    uint16_t width;
    uint16_t height;

    qp_init(device, rotation);
    // if needs inversion
#if 0
    qp_comms_start(device);
    qp_comms_command(device, ILI9XXX_CMD_INVERT_OFF);
    qp_comms_stop(device);
#endif
    qp_get_geometry(device, &width, &height, NULL, NULL, NULL);
    qp_clear(device);
    qp_rect(device, 0, 0, width - 1, height - 1, 0, 0, 0, true);
    qp_drawimage_recolor(device, 0, 0, frame, 0, 0, 255, 0, 0, 0);

    char title[50] = {0};
    snprintf(title, sizeof(title), "%s", PRODUCT);
    uint8_t title_width = qp_textwidth(font, title);
    if (title_width > (width - 50)) {
        title_width = width - 50;
    }
    uint8_t title_xpos = (width - title_width) / 2;
    qp_drawtext_recolor(device, title_xpos, 1, font, title, 0, 0, 0, 0, 0, 255);
    qp_close_image(frame);
}

void draw_ui_user(void) {
    bool            hue_redraw = false;
    static uint16_t last_hue   = {0xFFFF};
#if defined(RGBLIGHT_ENABLE) || defined(RGB_MATRIX_ENABLE)
    uint8_t curr_hue = rgblight_get_hue();
#endif

#ifdef POINTING_DEVICE_ENABLE
    static uint16_t last_cpi   = {0xFFFF};
    uint16_t        curr_cpi   = charybdis_get_pointer_sniping_enabled() ? charybdis_get_pointer_sniping_dpi()
                                                                         : charybdis_get_pointer_default_dpi();
    bool            cpi_redraw = false;
#endif

    uint16_t width;
    uint16_t height;
    qp_get_geometry(ili9341_display, &width, &height, NULL, NULL, NULL);

    if (last_hue != curr_hue) {
        last_hue   = curr_hue;
        hue_redraw = true;
    }
#ifdef POINTING_DEVICE_ENABLE
    if (last_cpi != curr_cpi) {
        last_cpi   = curr_cpi;
        cpi_redraw = true;
    }
#endif

    bool            layer_state_redraw = false;
    static uint32_t last_layer_state   = 0;
    if (last_layer_state != layer_state) {
        last_layer_state   = layer_state;
        layer_state_redraw = true;
    }

    bool            dl_state_redraw = false;
    static uint32_t last_dl_state   = 0;
    if (last_dl_state != default_layer_state) {
        last_dl_state   = default_layer_state;
        dl_state_redraw = true;
    }

#ifdef POINTING_DEVICE_ENABLE
    bool            ds_state_redraw = false;
    static uint32_t last_ds_state   = 0xFFFFFFFF;
    if (last_ds_state != charybdis_get_pointer_dragscroll_enabled()) {
        last_ds_state   = charybdis_get_pointer_dragscroll_enabled();
        ds_state_redraw = true;
    }

    bool            sp_state_redraw = false;
    static uint32_t last_sp_state   = 0xFFFFFFFF;
    if (last_sp_state != charybdis_get_pointer_sniping_enabled()) {
        last_sp_state   = charybdis_get_pointer_sniping_enabled();
        sp_state_redraw = true;
    }

    __attribute__((unused)) bool            am_state_redraw = false;
    __attribute__((unused)) static uint32_t last_am_state   = 0xFFFFFFFF;
    if (last_am_state != get_auto_mouse_enable()) {
        last_am_state   = get_auto_mouse_enable();
        am_state_redraw = true;
    }
#endif

#ifdef WPM_ENABLE
    bool            wpm_redraw      = false;
    static uint32_t last_wpm_update = 0;
    if (timer_elapsed32(last_wpm_update) > 125) {
        last_wpm_update = timer_read32();
        wpm_redraw      = true;
    }
#endif // WPM_ENABLE

#ifdef KEYLOGGER_ENABLE
    static uint32_t last_klog_update = 0;
    if (timer_elapsed32(last_klog_update) > 125 || keylogger_has_changed) {
        last_klog_update = timer_read32();
    }
#endif

    bool            scan_redraw      = false;
    static uint32_t last_scan_update = 0;
    if (timer_elapsed32(last_scan_update) > 125) {
        last_scan_update = timer_read32();
        scan_redraw      = true;
    }

#if defined(RGB_MATRIX_ENABLE) || defined(RGBLIGHT_ENABLE)
    bool            rgb_effect_redraw = false;
    static uint16_t last_effect       = 0xFFFF;
#    if defined(RGB_MATRIX_ENABLE)
    uint8_t curr_effect = rgb_matrix_get_mode();
#    else
    uint8_t curr_effect = rgblight_get_mode();
#    endif
    if (last_effect != curr_effect) {
        last_effect       = curr_effect;
        rgb_effect_redraw = true;
    }
#endif
    if (is_keyboard_left()) {
        char     buf[50] = {0};
        uint16_t ypos    = 15;
        uint16_t xpos    = 5;

#ifdef QP_LOCK_LOGO_ENABLE
        static led_t last_led_state = {0};
        if (hue_redraw || last_led_state.raw != host_keyboard_led_state().raw) {
            last_led_state.raw = host_keyboard_led_state().raw;
            qp_drawimage_recolor(ili9341_display, xpos, ypos, last_led_state.caps_lock ? lock_caps_on : lock_caps_off,
                                 curr_hue, 255, last_led_state.caps_lock ? 255 : 32, curr_hue, 255, 0);
            xpos += lock_caps_on->width + 4;
            qp_drawimage_recolor(ili9341_display, xpos, ypos, last_led_state.num_lock ? lock_num_on : lock_num_off,
                                 curr_hue, 255, last_led_state.num_lock ? 255 : 32, curr_hue, 255, 0);
            xpos += lock_num_on->width + 4;
            qp_drawimage_recolor(ili9341_display, xpos, ypos, last_led_state.scroll_lock ? lock_scrl_on : lock_scrl_off,
                                 curr_hue, 255, last_led_state.scroll_lock ? 255 : 32, curr_hue, 255, 0);
        }

        ypos += lock_caps_on->height + 4;
#endif // QP_LOCK_LOGO_ENABLE

#ifdef WPM_ENABLE
        if (hue_redraw || wpm_redraw) {
            static int max_wpm_xpos = 0;
            xpos                    = 5;
            snprintf(buf, sizeof(buf), "WPM: %d", (int)get_current_wpm());
            xpos += qp_drawtext_recolor(ili9341_display, xpos, ypos, font, buf, curr_hue, 255, 255, curr_hue, 255, 0);
            if (max_wpm_xpos < xpos) {
                max_wpm_xpos = xpos;
            }
            qp_rect(ili9341_display, xpos, ypos, max_wpm_xpos, ypos + font->line_height, 0, 0, 0, true);
        }
#endif // WPM_ENABLE

        if (hue_redraw || scan_redraw) {
            static int max_scans_xpos = 0;
            xpos                      = 50;
            snprintf(buf, sizeof(buf), "SCANS: %d", (int)get_matrix_scan_rate());
            xpos += qp_drawtext_recolor(ili9341_display, xpos, ypos, font, buf, curr_hue, 255, 255, curr_hue, 255, 0);
            if (max_scans_xpos < xpos) {
                max_scans_xpos = xpos;
            }
            qp_rect(ili9341_display, xpos, ypos, max_scans_xpos, ypos + font->line_height, 0, 0, 0, true);
        }

#ifdef POINTING_DEVICE_ENABLE
        if (hue_redraw || cpi_redraw) {
            static int max_cpi_xpos = 0;
            xpos                    = 110;
            snprintf(buf, sizeof(buf), "CPI: %d",
                     (int)charybdis_get_pointer_sniping_enabled() ? charybdis_get_pointer_sniping_dpi()
                                                                  : charybdis_get_pointer_default_dpi());
            xpos += qp_drawtext_recolor(ili9341_display, xpos, ypos, font, buf, curr_hue, 255, 255, curr_hue, 255, 0);
            if (max_cpi_xpos < xpos) {
                max_cpi_xpos = xpos;
            }
            qp_rect(ili9341_display, xpos, ypos, max_cpi_xpos, ypos + font->line_height, 0, 0, 0, true);
        }
        // if (hue_redraw) {
        //     xpos = 160;
        //     qp_rect(ili9341_display, xpos, ypos, xpos + 10, ypos + 10, 0, 0, 200, true);
        //     qp_drawimage(ili9341_display, xpos, ypos, mouse_icon);
        // }

        ypos += font->line_height + 4;
        if (ds_state_redraw) {
            static int max_dss_xpos = 0;
            xpos                    = 5;
            const char* buf         = "DRAG SCROLL";
            xpos += qp_drawtext_recolor(ili9341_display, xpos, ypos, font, buf,
                                        charybdis_get_pointer_dragscroll_enabled() ? 153 : 255, 255, 255,
                                        charybdis_get_pointer_dragscroll_enabled() ? 153 : 255, 255, 0);
            if (max_dss_xpos < xpos) {
                max_dss_xpos = xpos;
            }
            qp_rect(ili9341_display, xpos, ypos, max_dss_xpos, ypos + font->line_height, 0, 0, 0, true);
        }

        if (am_state_redraw) {
            static int max_ams_xpos = 0;
            xpos += 10;
            const char* buf = "Auto Layer";
            xpos += qp_drawtext_recolor(ili9341_display, xpos, ypos, font, buf, get_auto_mouse_enable() ? 34 : 255, 255,
                                        255, charybdis_get_pointer_sniping_enabled() ? 34 : 255, 255, 0);
            if (max_ams_xpos < xpos) {
                max_ams_xpos = xpos;
            }
            qp_rect(ili9341_display, xpos, ypos, max_ams_xpos, ypos + font->line_height, 0, 0, 0, true);
        }

        if (sp_state_redraw) {
            static int max_sps_xpos = 0;
            xpos += 10;
            const char* buf = "Sniping";
            xpos += qp_drawtext_recolor(ili9341_display, xpos, ypos, font, buf,
                                        charybdis_get_pointer_sniping_enabled() ? 153 : 255, 255, 255,
                                        charybdis_get_pointer_sniping_enabled() ? 153 : 255, 255, 0);
            if (max_sps_xpos < xpos) {
                max_sps_xpos = xpos;
            }
            qp_rect(ili9341_display, xpos, ypos, max_sps_xpos, ypos + font->line_height, 0, 0, 0, true);
        }
#endif // POINTING_DEVICE_ENABLE

        ypos += font->line_height + 4;
        static keymap_config_t last_keymap_config = {0};
        if (hue_redraw || last_keymap_config.raw != keymap_config.raw) {
            last_keymap_config.raw  = keymap_config.raw;
            uint8_t    temp_pos     = 0;
            static int max_bpm_xpos = 0;
            xpos                    = 5;
            const char* buf0        = "NKRO ";
            const char* buf1        = "CRCT ";
            const char* buf2        = "1SHOT";
            qp_drawimage(ili9341_display, xpos, ypos, last_keymap_config.swap_lctl_lgui ? cg_on : cg_off);
            temp_pos = xpos += cg_off->width + 5;
            temp_pos = qp_drawtext_recolor(ili9341_display, xpos, ypos, font, buf0, last_keymap_config.nkro ? 153 : 255,
                                           255, 255, last_keymap_config.nkro ? 153 : 255, 255, 0);
            xpos += MAX(temp_pos, 30);
            temp_pos = qp_drawtext_recolor(ili9341_display, xpos, ypos, font, buf1,
                                           last_keymap_config.autocorrect_enable ? 153 : 255, 255, 255,
                                           last_keymap_config.autocorrect_enable ? 153 : 255, 255, 0);
            xpos += MAX(temp_pos, 30);
            temp_pos += qp_drawtext_recolor(ili9341_display, xpos, ypos, font, buf2,
                                            last_keymap_config.oneshot_enable ? 153 : 255, 255, 255,
                                            last_keymap_config.oneshot_enable ? 153 : 255, 255, 0);
            xpos += MAX(temp_pos, 30);
            if (max_bpm_xpos < xpos) {
                max_bpm_xpos = xpos;
            }
            qp_rect(ili9341_display, xpos, ypos, max_bpm_xpos, ypos + font->line_height, 0, 0, 0, true);
        }

        ypos += font->line_height + 2;
        static user_runtime_config_t last_user_state = {0};
        if (hue_redraw || last_user_state.raw != user_state.raw) {
            last_user_state.raw     = user_state.raw;
            uint8_t    temp_pos     = 0;
            static int max_upm_xpos = 0;
            xpos                    = cg_off->width + 10;
            const char* buf0        = "AUDIO";
            const char* buf1        = "CLCK ";
            const char* buf2        = "HOST ";
            const char* buf3        = "SWAP ";
            temp_pos =
                qp_drawtext_recolor(ili9341_display, xpos, ypos, font, buf0, last_user_state.audio_enable ? 153 : 255,
                                    255, 255, last_user_state.audio_enable ? 153 : 255, 255, 0);
            xpos += MAX(temp_pos, 30);
            temp_pos = qp_drawtext_recolor(ili9341_display, xpos, ypos, font, buf1,
                                           last_user_state.audio_clicky_enable ? 153 : 255, 255, 255,
                                           last_user_state.audio_clicky_enable ? 153 : 255, 255, 0);
            xpos += MAX(temp_pos, 30);
            temp_pos = qp_drawtext_recolor(ili9341_display, xpos, ypos, font, buf2,
                                           last_user_state.host_driver_disabled ? 153 : 255, 255, 255,
                                           last_user_state.host_driver_disabled ? 153 : 255, 255, 0);
            xpos += MAX(temp_pos, 30);
            temp_pos =
                qp_drawtext_recolor(ili9341_display, xpos, ypos, font, buf3, last_user_state.swap_hands ? 153 : 255,
                                    255, 255, last_user_state.swap_hands ? 153 : 255, 255, 0);
            xpos += MAX(temp_pos, 30);
            if (max_upm_xpos < xpos) {
                max_upm_xpos = xpos;
            }
            qp_rect(ili9341_display, xpos, ypos, max_upm_xpos, ypos + font->line_height, 0, 0, 0, true);
        }

#if defined(RGBLIGHT_ENABLE)
        ypos += font->line_height + 4;
        if (hue_redraw || rgb_effect_redraw) {
            static int max_rgb_xpos = 0;
            xpos                    = 5;
            snprintf(buf, sizeof(buf), "RGB Light: %s", rgblight_name(curr_effect));
            for (int i = 5; i < sizeof(buf); ++i) {
                if (buf[i] == 0)
                    break;
                else if (buf[i] == '_')
                    buf[i] = ' ';
                else if (buf[i - 1] == ' ')
                    buf[i] = toupper(buf[i]);
                else if (buf[i - 1] != ' ')
                    buf[i] = tolower(buf[i]);
            }

            xpos += qp_drawtext_recolor(ili9341_display, xpos, ypos, font, buf, curr_hue, 255, 255, curr_hue, 255, 0);
            if (max_rgb_xpos < xpos) {
                max_rgb_xpos = xpos;
            }
            qp_rect(ili9341_display, xpos, ypos, max_rgb_xpos, ypos + font->line_height, 0, 0, 0, true);
        }
#endif // RGBLIGHT_ENABLE

#if defined(RGB_MATRIX_ENABLE)
        ypos += font->line_height + 4;
        if (hue_redraw || rgb_effect_redraw) {
            static int max_rgb_xpos = 0;
            xpos                    = 5;
            snprintf(buf, sizeof(buf), "RGB Matrix: %s", rgb_matrix_name(curr_effect));
            for (int i = 5; i < sizeof(buf); ++i) {
                if (buf[i] == 0)
                    break;
                else if (buf[i] == '_')
                    buf[i] = ' ';
                else if (buf[i - 1] == ' ')
                    buf[i] = toupper(buf[i]);
                else if (buf[i - 1] != ' ')
                    buf[i] = tolower(buf[i]);
            }

            xpos += qp_drawtext_recolor(ili9341_display, xpos, ypos, font, buf, curr_hue, 255, 255, curr_hue, 255, 0);
            if (max_rgb_xpos < xpos) {
                max_rgb_xpos = xpos;
            }
            qp_rect(ili9341_display, xpos, ypos, max_rgb_xpos, ypos + font->line_height, 0, 0, 0, true);
        }
#endif // RGB_MATRIX_ENABLE

        ypos += font->line_height + 4;
        if (hue_redraw || dl_state_redraw || layer_state_redraw) {
            const char* dl_name = "unknown";
            switch (get_highest_layer(default_layer_state)) {
                case _COLEMAK_DH:
                    dl_name = "COLEMAK_DH";
                    break;
                case _COLEMAK:
                    dl_name = "COLEMAK";
                    break;
                case _QWERTY:
                    dl_name = "QWERTY";
                    break;
                case _DVORAK:
                    dl_name = "DVORAK";
                    break;
            }

            static int max_layer_xpos = 0;
            xpos                      = 5;
            snprintf(buf, sizeof(buf), "LAYOUT: %s", dl_name);
            xpos += qp_drawtext_recolor(ili9341_display, xpos, ypos, font, buf, curr_hue, 255, 255, curr_hue, 255, 0);
            if (max_layer_xpos < xpos) {
                max_layer_xpos = xpos;
            }
            qp_rect(ili9341_display, xpos, ypos, max_layer_xpos, ypos + font->line_height, 0, 0, 0, true);
        }

        if (hue_redraw || layer_state_redraw) {
            const char* layer_name = "default";
            switch (get_highest_layer(layer_state)) {
                case _MOUSE:
                    layer_name = "mouse";
                    break;
                case _LOWER:
                    layer_name = "lower";
                    break;
                case _RAISE:
                    layer_name = "raise";
                    break;
                case _ADJUST:
                    layer_name = "adjust";
                    break;
                case _MEDIA:
                    layer_name = "keypad";
                    break;
            }

            static int max_layer_xpos = 0;
            xpos                      = 5 + (qp_textwidth(font, "LAYOUT: COLEMAK_DH"));
            snprintf(buf, sizeof(buf), "LAYER: %s", layer_name);
            xpos += qp_drawtext_recolor(ili9341_display, xpos, ypos, font, buf, curr_hue, 255, 255, curr_hue, 255, 0);
            if (max_layer_xpos < xpos) {
                max_layer_xpos = xpos;
            }
            qp_rect(ili9341_display, xpos, ypos, max_layer_xpos, ypos + font->line_height, 0, 0, 0, true);
        }

#ifdef AUTOCORRECT_ENABLE
        ypos += font->line_height + 4;
        extern bool autocorrect_str_has_changed;
        extern char autocorrected_str[2][21];
        if (hue_redraw || autocorrect_str_has_changed) {
            static int max_klog_xpos = 0;
            xpos                     = 5;
            snprintf(buf, sizeof(buf), "Autocorrected: %s", autocorrected_str[0]);
            xpos += qp_drawtext_recolor(ili9341_display, xpos, ypos, font, buf, curr_hue, 255, 255, curr_hue, 255, 0);

            if (max_klog_xpos < xpos) {
                max_klog_xpos = xpos;
            }
            qp_rect(ili9341_display, xpos, ypos, max_klog_xpos, ypos + font->line_height, 0, 0, 0, true);

            ypos += font->line_height + 4;
            max_klog_xpos = 0;
            xpos          = 5;
            snprintf(buf, sizeof(buf), "Original Text: %s", autocorrected_str[1]);
            xpos += qp_drawtext_recolor(ili9341_display, xpos, ypos, font, buf, curr_hue, 255, 255, curr_hue, 255, 0);
            if (max_klog_xpos < xpos) {
                max_klog_xpos = xpos;
            }
            qp_rect(ili9341_display, xpos, ypos, max_klog_xpos, ypos + font->line_height, 0, 0, 0, true);

            autocorrect_str_has_changed = false;
        }
#endif // AUTOCORRECT_ENABLE

#ifdef KEYLOGGER_ENABLE // keep at very end
        ypos = height - (font->line_height + 2);
        if (keylogger_has_changed) {
            static int max_klog_xpos = 0;
            xpos                     = 25;
            snprintf(buf, sizeof(buf), "Keylogger: %s", qp_keylog_str);

            xpos += qp_drawtext_recolor(ili9341_display, xpos, ypos, font, buf, 0, 255, 0, 0, 0, 255);

            if (max_klog_xpos < xpos) {
                max_klog_xpos = xpos;
            }
            // qp_rect(ili9341_display, xpos, ypos, max_klog_xpos, ypos + font->line_height, 0, 0, 255, true);
            keylogger_has_changed = false;
        }
#endif
    }
    qp_flush(ili9341_display);
}

void housekeeping_task_quantum_painter(void) {
    draw_ui_user();
}

void keyboard_post_init_quantum_painter(void) {
    font  = qp_load_font_mem(font_thintel15);
    frame = qp_load_image_mem(gfx_frame);

    // ters1 = qp_load_image_mem(gfx_ters1);
    lock_caps_on  = qp_load_image_mem(gfx_lock_caps_ON);
    lock_caps_off = qp_load_image_mem(gfx_lock_caps_OFF);
    lock_num_on   = qp_load_image_mem(gfx_lock_NUM_ON);
    lock_num_off  = qp_load_image_mem(gfx_lock_NUM_OFF);
    lock_scrl_on  = qp_load_image_mem(gfx_lock_scrl_ON);
    lock_scrl_off = qp_load_image_mem(gfx_lock_scrl_OFF);
    // test_anim = qp_load_image_mem(gfx_test_anim);
    // matrix = qp_load_image_mem(gfx_matrix);
    cg_on      = qp_load_image_mem(gfx_cg_on);
    cg_off     = qp_load_image_mem(gfx_cg_off);
    mouse_icon = qp_load_image_mem(gfx_mouse_icon);

#ifdef BACKLIGHT_ENABLE
    backlight_level_noeeprom(BACKLIGHT_LEVELS);
#elif defined(BACKLIGHT_PIN)
    gpio_set_pin_output_push_pull(BACKLIGHT_PIN);
    gpio_write_pin_high(BACKLIGHT_PIN);
#endif
    wait_ms(150);

    ili9341_display =
        qp_ili9341_make_spi_device(240, 320, DISPLAY_CS_PIN, DISPLAY_DC_PIN, DISPLAY_RST_PIN, DISPLAY_SPI_DIVIDER, 0);
    init_and_clear(ili9341_display, QP_ROTATION_180);
    qp_power(ili9341_display, true);

    wait_ms(50);
}

#ifdef BACKLIGHT_ENABLE
static uint8_t last_backlight = 255;
#endif

void suspend_power_down_quantum_painter(void) {
#ifdef BACKLIGHT_ENABLE
    if (last_backlight == 255) {
        last_backlight = get_backlight_level();
    }
    backlight_set(0);
#elif defined(BACKLIGHT_PIN)
    gpio_write_pin_low(BACKLIGHT_PIN);
#endif
    qp_power(ili9341_display, false);
}

void suspend_wakeup_init_quantum_painter(void) {
    qp_power(ili9341_display, true);
#ifdef BACKLIGHT_ENABLE
    if (last_backlight != 255) {
        backlight_set(last_backlight);
    }
    last_backlight = 255;
#elif defined(BACKLIGHT_PIN)
    gpio_write_pin_high(BACKLIGHT_PIN);
#endif
}

void shutdown_quantum_painter(void) {
#ifdef BACKLIGHT_ENABLE
    backlight_set(0);
#elif defined(BACKLIGHT_PIN)
    gpio_write_pin_low(BACKLIGHT_PIN);
#endif
    qp_power(ili9341_display, false);
}
