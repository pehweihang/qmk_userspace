#pragma once

#include_next <mcuconf.h>

#undef STM32_WDG_USE_IWDG
#define STM32_WDG_USE_IWDG TRUE

#undef STM32_I2C_BUSY_TIMEOUT
#define STM32_I2C_BUSY_TIMEOUT 10
