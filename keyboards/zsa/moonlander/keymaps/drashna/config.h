// Copyright 2023 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#define TAPPING_TERM_PER_KEY
// #define WAIT_FOR_USB
// #define NO_USB_STARTUP_CHECK

#define SECURE_UNLOCK_SEQUENCE    \
    {                             \
        {2, 1}, {2, 2}, {2, 3}, { \
            2, 4                  \
        }                         \
    }
