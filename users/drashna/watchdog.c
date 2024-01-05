// Copyright 2023 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
// SPDX-License-Identifier: GPL-3.0-or-later

/**
 * @brief Because this is a stupid idea, why not.   Adds support for watchdog monitoring of keyboard.
 *
 */

#include "watchdog.h"
#ifdef __arm__
#    include <hal.h>
#    include <math.h>
#    include <stddef.h>

// STM32-specific watchdog config calculations
// timeout = 31.25us * PR * (RL + 1)
#    define _STM32_IWDG_LSI(us)    ((us) / 31.25)
#    define STM32_IWDG_PR_US(us)   (uint8_t)(log(_STM32_IWDG_LSI(us)) / log(2) - 11)
#    define STM32_IWDG_PR_MS(s)    STM32_IWDG_PR_US(s * 1000.0)
#    define STM32_IWDG_PR_S(s)     STM32_IWDG_PR_US(s * 1000000.0)
#    define _STM32_IWDG_SCALAR(us) (2 << ((uint8_t)STM32_IWDG_PR_US(us) + 1))
#    define STM32_IWDG_RL_US(us)   (uint64_t)(_STM32_IWDG_LSI(us)) / _STM32_IWDG_SCALAR(us)
#    define STM32_IWDG_RL_MS(s)    STM32_IWDG_RL_US(s * 1000.0)
#    define STM32_IWDG_RL_S(s)     STM32_IWDG_RL_US(s * 1000000.0)

#    if !defined(WATCHDOG_TIMEOUT)
#        define WATCHDOG_TIMEOUT 2.0
#    endif

void watchdog_init(void) {
    wdgInit();

    static WDGConfig wdgcfg;
#if STM32_HAS_IWDG == TRUE
    wdgcfg.pr   = STM32_IWDG_PR_S(WATCHDOG_TIMEOUT);
#endif
    wdgcfg.rlr  = STM32_IWDG_RL_S(WATCHDOG_TIMEOUT);
#if STM32_IWDG_IS_WINDOWED == TRUE
    wdgcfg.winr = STM32_IWDG_WIN_DISABLED;
#endif
    wdgStart(&WDGD1, &wdgcfg);
}

void watchdog_task(void) {
    wdgReset(&WDGD1);
}

void watchdog_shutdown(void) {
    wdgStop(&WDGD1);
}
#elif defined(__AVR__)
#    include <avr/wdt.h>

void watchdog_init(void) {
    wdt_enable(WDTO_2S);
}

void watchdog_task(void) {
    wdt_reset();
}

void watchdog_shutdown(void) {
    wdt_disable();
}
#endif
