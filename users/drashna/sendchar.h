// Copyright 2023 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <stdint.h>
#ifdef __AVR__
#    include_next "sendchar.h"
#endif

extern uint32_t sendchar_timer;
int8_t          drashna_sendchar(uint8_t c);
