// Copyright 2023 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "eeconfig_users.h"
#include "eeprom.h"
#include "eeconfig.h"
#include <string.h>

#if !defined(KEYBOARD_input_club_ergodox_infinity)
_Static_assert(TOTAL_EEPROM_BYTE_COUNT >= EECONFIG_SIZE, "More eeprom configured than is available.");
#endif
#if (EECONFIG_USER_DATA_SIZE) != 0 && ((defined(CUSTOM_QUANTUM_PAINTER_ENABLE) && EECONFIG_USER_DATA_SIZE < 8) || \
                                       (!defined(CUSTOM_QUANTUM_PAINTER_ENABLE) && EECONFIG_USER_DATA_SIZE < 8))
#    error "Not enough EEPROM configured for user config."
#endif

#if (EECONFIG_USER_DATA_SIZE) == 0
#    define EECONFIG_USER_TEMP EECONFIG_USER
#else // EECONFIG_USER_DATA_SIZE
#    define EECONFIG_USER_TEMP (uint64_t *)(EECONFIG_USER_DATABLOCK)
#endif // EECONFIG_USER_DATA_SIZE

void eeconfig_read_user_config(uint64_t *data) {
#if (EECONFIG_USER_DATA_SIZE) > 0
    if (!eeconfig_is_user_datablock_valid()) {
        memset(data, 0, 4);
    } else
#endif // EECONFIG_USER_DATA_SIZE
        eeprom_read_block(data, EECONFIG_USER_TEMP, 8);
}

void eeconfig_update_user_config(const uint64_t *data) {
    eeprom_update_block(data, EECONFIG_USER_TEMP, 8);
#if (EECONFIG_USER_DATA_SIZE) > 0
    eeprom_update_dword(EECONFIG_USER, (EECONFIG_USER_DATA_VERSION));
#endif // EECONFIG_USER_DATA_SIZE
}

void eeconfig_read_user_data(void *data) {
#if (EECONFIG_USER_DATA_SIZE) > 8
    if (eeconfig_is_user_datablock_valid()) {
        eeprom_read_block(data, EECONFIG_USER_DATABLOCK + 8, (EECONFIG_USER_DATA_SIZE)-8);
    } else {
        memset(data, 0, (EECONFIG_USER_DATA_SIZE));
    }
#endif // EECONFIG_USER_DATA_SIZE
}

void eeconfig_update_user_data(const void *data) {
#if (EECONFIG_USER_DATA_SIZE) > 8
    eeprom_update_dword(EECONFIG_USER, (EECONFIG_USER_DATA_VERSION));
    eeprom_update_block(data, EECONFIG_USER_DATABLOCK + 8, (EECONFIG_USER_DATA_SIZE)-8);
#endif // EECONFIG_USER_DATA_SIZE
}
