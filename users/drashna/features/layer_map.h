// Copyright 2023 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <stdio.h>

extern volatile uint16_t layer_map[MATRIX_ROWS][MATRIX_COLS];
void                     set_layer_map(void);
