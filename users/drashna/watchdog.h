// Copyright 2023 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

void watchdog_init(void);
void watchdog_task(void);
void watchdog_shutdown(void);
void suspend_power_down_watchdog(void);
void suspend_wakeup_init_watchdog(void);
