// Copyright 2024 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <stdio.h>
#include "action.h"
#include "progmem.h"

extern bool               keylogger_has_changed;
extern const char PROGMEM code_to_name[256];

bool process_record_display_driver(uint16_t keycode, keyrecord_t* record);
