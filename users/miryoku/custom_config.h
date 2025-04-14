// Copyright 2019 Manna Harbour
// https://github.com/manna-harbour/miryoku

// This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version. This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with this program. If not, see <http://www.gnu.org/licenses/>.

#pragma once

#define BILATERAL_COMBINATIONS
#undef MASTER_LEFT
#undef MASTER_RIGHT
#define EE_HANDS
#define TAPPING_TERM 150

#if defined (KEYBOARD_bastardkb_charybdis_3x5)
    #define MIRYOKU_LAYER_MOUSE \
    TD(U_TD_BOOT),     TD(U_TD_U_TAP),    TD(U_TD_U_EXTRA),  TD(U_TD_U_BASE),   U_NA,              U_RDO,             U_PST,             U_CPY,             U_CUT,             U_UND,             \
    KC_LGUI,           KC_LALT,           KC_LCTL,           KC_LSFT,           U_NA,              U_NA,              KC_BTN1,           KC_BTN2,           KC_BTN3,           DRGSCRL,              \
    U_NA,              KC_ALGR,           TD(U_TD_U_SYM),    TD(U_TD_U_MOUSE),  U_NA,              U_NA,              U_NA,              U_NA,              U_NA,              SNIPING,              \
    U_NP,              U_NP,              U_NA,              U_NA,              U_NA,              DPI_MOD,           DPI_RMOD,          U_NA,              U_NP,              U_NP

    #define CHARYBDIS_DRAGSCROLL_REVERSE_X
    #define CHARYBDIS_DRAGSCROLL_REVERSE_Y
    #define MACCEL_TAKEOFF 2.0      // lower/higher value = curve takes off more smoothly/abruptly
    #define MACCEL_GROWTH_RATE 0.25 // lower/higher value = curve reaches its upper limit slower/faster
    #define MACCEL_OFFSET 2.2       // lower/higher value = acceleration kicks in earlier/later
    #define MACCEL_LIMIT 0.2        // lower limit of accel curve (minimum acceleration factor)
    #define MOUSE_EXTENDED_REPORT
#endif
