// Copyright 2023 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "print.h"
#include "timer.h"

#ifdef SEGGER_RTT_ENABLE
#    include "SEGGER_RTT.h"
#endif // SEGGER_RTT_ENABLE
#ifdef VIRTSER_ENABLE
#    include "virtser.h"
#endif // VIRTSER_ENABLE

uint32_t sendchar_timer = 0;

int8_t drashna_sendchar(uint8_t c) {
    uint8_t ret    = 0;
    sendchar_timer = timer_read32();

    extern int8_t sendchar(uint8_t c);
    ret = sendchar(c);
#ifdef SEGGER_RTT_ENABLE
    ret = SEGGER_RTT_PutChar(0, (char)c);
#endif // SEGGER_RTT_ENABLE
#ifdef VIRTSER_ENABLE
    virtser_send(c);
#endif // VIRTSER_ENABLE
#if defined(DISPLAY_DRIVER_ENABLE)
    void display_sendchar_hook(uint8_t c);
    display_sendchar_hook(c);
#endif // DISPLAY_DRIVER_ENABLE
    return ret;
}
