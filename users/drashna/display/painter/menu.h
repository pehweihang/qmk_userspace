// Copyright 2018-2024 Nick Brassel (@tzarc)
// Copyright 2024 Drashna (@drashna)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct _menu_state_t {
    bool    dirty;
    bool    is_in_menu;
    uint8_t selected_child;
    uint8_t menu_stack[8];
} menu_state_t;

extern menu_state_t display_menu_state;
