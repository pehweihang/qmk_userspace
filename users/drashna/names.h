// Copyright 2020 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
// Copyright 2018-2024 Nick Brassel (@tzarc)
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <ctype.h>

const char *keycode_name(uint16_t keycode, bool shifted);
const char *layer_name(uint8_t layer);
const char *mod_name(uint16_t mod);
