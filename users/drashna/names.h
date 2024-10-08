// Copyright 2020 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
// Copyright 2018-2024 Nick Brassel (@tzarc)
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <stdint.h>
#ifdef OS_DETECTION_ENABLE
#    include "os_detection.h"
#endif // OS_DETECTION_ENABLE
#if defined(HAPTIC_ENABLE) && defined(HAPTIC_DRV2605L)
#    include "drv2605l.h"
#endif // HAPTIC_ENABLE && HAPTIC_DRV2605L

const char *keycode_name(uint16_t keycode, bool shifted);
const char *layer_name(uint8_t layer);
const char *mod_name(uint16_t mod);
#if defined(HAPTIC_ENABLE) && defined(HAPTIC_DRV2605L)
const char *get_haptic_drv2605l_effect_name(drv2605l_effect_t effect);
#endif // HAPTIC_ENABLE && HAPTIC_DRV2605L
#ifdef OS_DETECTION_ENABLE
const char *os_variant_to_string(os_variant_t os);
#endif // OS_DETECTION_ENABLE
#ifdef CUSTOM_UNICODE_ENABLE
const char *unicode_typing_mode(uint8_t mode);
#endif // CUSTOM_UNICODE_ENABLE
