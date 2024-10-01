// Copyright 2020 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "progmem.h"

enum unicode_typing_modes {
    UCTM_NO_MODE,
    UCTM_WIDE,
    UCTM_SCRIPT,
    UCTM_BLOCKS,
    UCTM_REGIONAL,
    UCTM_AUSSIE,
    UCTM_ZALGO,
    UCTM_SUPER,
    UCTM_COMIC,
    UCTM_FRAKTUR,
    UCTM_DOUBLE_STRUCK,
    UNCODES_MODE_END,
};

extern const PROGMEM char unicode_mode_str[UNCODES_MODE_END][13];
void                      set_unicode_input_mode_soft(uint8_t input_mode);
