#pragma once

#undef OLED_DISPLAY_64X128
#define OLED_DISPLAY_128X128
#undef OLED_UPDATE_INTERVAL
#define OLED_UPDATE_INTERVAL 100

#define DRIVER_ADDR_1        0x30
#define DRIVER_COUNT         1
#define DRIVER_1_LED_TOTAL   117
#define RGB_MATRIX_LED_COUNT (117)
#define RGB_MATRIX_CENTER \
    { 48, 32 }
#define ISSI_GLOBALCURRENT 0x01
#define RGB_MATRIX_TIMEOUT 60000

#define DRV2605L_FB_ERM_LRA   0
#define DRV2605L_GREETING     DRV2605L_EFFECT_750_MS_ALERT_100
#define DRV2605L_DEFAULT_MODE DRV2605L_EFFECT_BUZZ_1_100
