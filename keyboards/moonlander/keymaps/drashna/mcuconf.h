#pragma once

#include_next <mcuconf.h>

#undef STM32_WDG_USE_IWDG
#define STM32_WDG_USE_IWDG TRUE
