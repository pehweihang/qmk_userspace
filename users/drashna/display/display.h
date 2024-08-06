// Copyright 2024 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <stdio.h>
#include "action.h"
#include "progmem.h"

extern bool               keylogger_has_changed;
extern bool               layer_map_has_updated;
extern const char PROGMEM code_to_name[256];

bool process_record_display_driver(uint16_t keycode, keyrecord_t* record);
void keyboard_post_init_display_driver(void);

#ifdef KEYLOGGER_ENABLE
#    ifndef QP_KEYLOGGER_LENGTH
#        define QP_KEYLOGGER_LENGTH 20
#    endif
extern char qp_keylog_str[QP_KEYLOGGER_LENGTH];
extern bool keylogger_has_changed;
#endif
