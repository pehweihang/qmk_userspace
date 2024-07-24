#include "drashna.h"

#if defined(OLED_ENABLE) && defined(CUSTOM_OLED_DRIVER)
#    include "display/oled/oled_stuff.h"
#endif
#if defined(QUANTUM_PAINTER_ENABLE) && defined(CUSTOM_QUANTUM_PAINTER_ENABLE)
#    include "qp.h"
#    ifdef CUSTOM_QUANTUM_PAINTER_ILI9341
#        include "display/painter/ili9341_display.h"
#    endif // CUSTOM_QUANTUM_PAINTER_ILI9341
#endif     // QUANTUM_PAINTER_ENABLE && CUSTOM_QUANTUM_PAINTER_ENABLE

bool keylogger_has_changed = false;

// clang-format off
static const char PROGMEM code_to_name[256] = {
//   0    1    2    3    4    5    6    7    8    9    A    B    c    D    E    F
    ' ', ' ', ' ', ' ', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l',  // 0x
    'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '1', '2',  // 1x
    '3', '4', '5', '6', '7', '8', '9', '0',  20,  19,  27,  26,  22, '-', '=', '[',  // 2x
    ']','\\', '#', ';','\'', '`', ',', '.', '/', 128,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,  // 3x
    0xDB,0xDC,0xDD,0xDE,0XDF,0xFB,'P', 'S',  19, ' ',  17,  30,  16,  16,  31,  26,  // 4x
     27,  25,  24, 'N', '/', '*', '-', '+',  23, '1', '2', '3', '4', '5', '6', '7',  // 5x
    '8', '9', '0', '.','\\', 'A',   0, '=', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',  // 6x
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',  // 7x
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',  // 8x
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',  // 9x
    ' ', ' ', ' ', ' ', ' ',   0, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',  // Ax
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',  // Bx
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',  // Cx
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',  // Dx
    'C', 'S', 'A', 'C', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',  24,  26,  24,  // Ex
     25,0x9D,0x9D,0x9D,0x9D,0x9D,0x9D,0x9D,0x9D,  24,  25,  27,  26, ' ', ' ', ' '   // Fx
};
// clang-format on

/**
 * @brief parses pressed keycodes and saves to buffer
 *
 * @param keycode Keycode pressed from switch matrix
 * @param record keyrecord_t data structure
 */
static void add_keylog(uint16_t keycode, keyrecord_t* record, char* str, uint8_t length) {
    keycode = extract_basic_keycode(keycode, record, true);

    if ((keycode == KC_BSPC) && mod_config(get_mods() | get_oneshot_mods()) & MOD_MASK_CTRL) {
        memset(str, ' ', length);
        str[length - 1] = 0x00;
        return;
    }
    if (record->tap.count) {
        keycode &= 0xFF;
    } else if (keycode > 0xFF) {
        return;
    }

    memmove(str, str + 1, length - 2);

    if (keycode < ARRAY_SIZE(code_to_name)) {
        str[(length - 2)]     = pgm_read_byte(&code_to_name[keycode]);
        keylogger_has_changed = false;
    }
}

/**
 * @brief Keycode handler for oled display.
 *
 * This adds pressed keys to buffer, but also resets the oled timer
 *
 * @param keycode Keycode from matrix
 * @param record keyrecord data struture
 * @return true
 * @return false
 */
bool process_record_display_driver(uint16_t keycode, keyrecord_t* record) {
    if (record->event.pressed) {
#if defined(OLED_ENABLE) && defined(CUSTOM_OLED_DRIVER)
        add_keylog(keycode, record, oled_keylog_str, (OLED_KEYLOGGER_LENGTH + 1));
#endif // OLED_ENABLE && CUSTOM_OLED_DRIVER
#if defined(QUANTUM_PAINTER_ENABLE) && defined(CUSTOM_QUANTUM_PAINTER_ENABLE)
        add_keylog(keycode, record, qp_keylog_str, QP_KEYLOGGER_LENGTH);
#endif // QUANTUM_PAINTER_ENABLE && CUSTOM_QUANTUM_PAINTER_ENABLE
    }
    return true;
}
