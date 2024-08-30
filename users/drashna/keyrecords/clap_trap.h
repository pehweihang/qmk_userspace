// Copyright 2024 Harrison Chan (@xelus22)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "action.h"

typedef struct clap_trap_t {
    uint16_t press, unpress;
} clap_trap_t;

bool process_clap_trap(uint16_t keycode, keyrecord_t *record);
bool process_clap_trap_user(uint16_t keycode, keyrecord_t *record);

bool clap_trap_recovery_is_enabled(void);
void clap_trap_recovery_enable(void);
void clap_trap_recovery_disable(void);
void clap_trap_recovery_toggle(void);

bool clap_trap_is_enabled(void);
void clap_trap_enable(void);
void clap_trap_disable(void);
void clap_trap_toggle(void);
