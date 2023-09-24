// Copyright 2021 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "drashna.h"

void           pointing_device_init_keymap(void);
report_mouse_t pointing_device_task_keymap(report_mouse_t mouse_report);
void           matrix_scan_pointing(void);
bool           process_record_pointing(uint16_t keycode, keyrecord_t* record);
layer_state_t  layer_state_set_pointing(layer_state_t state);
void           pointing_device_mouse_jiggler_toggle(void);

#ifdef POINTING_MODE_MAP_ENABLE
extern const uint16_t PROGMEM pointing_mode_maps[POINTING_MODE_MAP_COUNT][POINTING_NUM_DIRECTIONS];
#endif
