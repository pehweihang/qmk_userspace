// Copyright 2024 QMK -- generated source code only, image retains original copyright
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "qp.h"
#include "drashna_runtime.h"
#include "display/painter/fonts.qff.h"
#include "display/painter/graphics.qgf.h"
#include "display/painter/graphics/assets.h"
#include "display/display.h"

void housekeeping_task_quantum_painter(void);
void keyboard_post_init_quantum_painter(void);
void suspend_power_down_quantum_painter(void);
void suspend_wakeup_init_quantum_painter(void);
void shutdown_quantum_painter(bool jump_to_bootloader);

char* truncate_text(const char* text, uint16_t max_width, painter_font_handle_t font, bool from_start,
                    bool add_ellipses);
void  render_character_set(painter_device_t display, uint16_t* x_offset, uint16_t* max_pos, uint16_t* ypos,
                           painter_font_handle_t font, uint8_t hue_fg, uint8_t sat_fg, uint8_t val_fg, uint8_t hue_bg,
                           uint8_t sat_bg, uint8_t val_bg);

dual_hsv_t painter_get_dual_hsv(void);
void       painter_sethsv(uint8_t hue, uint8_t sat, uint8_t val, bool primary);
void       painter_sethsv_noeeprom(uint8_t hue, uint8_t sat, uint8_t val, bool primary);
HSV        painter_get_hsv(bool primary);
uint8_t    painter_get_hue(bool primary);
uint8_t    painter_get_sat(bool primary);
uint8_t    painter_get_val(bool primary);
void       painter_increase_hue(bool primary);
void       painter_increase_hue_noeeprom(bool primary);
void       painter_decrease_hue(bool primary);
void       painter_decrease_hue_noeeprom(bool primary);
void       painter_increase_sat(bool primary);
void       painter_increase_sat_noeeprom(bool primary);
void       painter_decrease_sat(bool primary);
void       painter_decrease_sat_noeeprom(bool primary);
void       painter_increase_val(bool primary);
void       painter_increase_val_noeeprom(bool primary);
void       painter_decrease_val(bool primary);
void       painter_decrease_val_noeeprom(bool primary);
