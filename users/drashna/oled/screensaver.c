
#include "oled_driver.h"
#include "timer.h"
#include "util.h"

#if defined(MCU_RP) || defined(QMK_MCU_SERIES_STM32F4XX)
#    include "screensavers/matrix_scroll.h"
#endif

#define FRAME_DURATION 30 // How long each frame lasts in milliseconds

void render_oled_screensaver(void) {
#if defined(MCU_RP) || defined(QMK_MCU_SERIES_STM32F4XX)
    static uint32_t timer         = 0;
    static uint8_t  current_frame = 0;

    // Run animation
    if (timer_elapsed(timer) > FRAME_DURATION) {
        // Set timer to updated time
        timer = timer_read();

        // Increment frame
        current_frame = (current_frame + 1) % ARRAY_SIZE(screensaver);
    }

    // Draw frame to OLED
    oled_write_raw_P(screensaver[current_frame], ARRAY_SIZE(screensaver[current_frame]));

#else
    oled_off();
#endif
}
