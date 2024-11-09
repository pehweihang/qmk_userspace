#ifdef AUDIO_ENABLE
#    include "audio.h"

#    ifndef AG_NORM_SONG
#        define AG_NORM_SONG SONG(AG_NORM_SOUND)
#    endif
#    ifndef AG_SWAP_SONG
#        define AG_SWAP_SONG SONG(AG_SWAP_SOUND)
#    endif
#    ifndef CG_NORM_SONG
#        define CG_NORM_SONG SONG(AG_NORM_SOUND)
#    endif
#    ifndef CG_SWAP_SONG
#        define CG_SWAP_SONG SONG(AG_SWAP_SOUND)
#    endif
static float ag_norm_song[][2] = AG_NORM_SONG;
static float ag_swap_song[][2] = AG_SWAP_SONG;
static float cg_norm_song[][2] = CG_NORM_SONG;
static float cg_swap_song[][2] = CG_SWAP_SONG;
#endif
#ifndef DISPLAY_MENU_TIMEOUT
#    define DISPLAY_MENU_TIMEOUT 30000
#endif // !DISPLAY_MENU_TIMEOUT
deferred_token menu_deferred_token = INVALID_DEFERRED_TOKEN;

#define MENU_ENTRY_CHILD(display_text, name)             \
    {                                                    \
        .flags                 = menu_flag_is_value,     \
        .text                  = display_text,           \
        .child.menu_handler    = menu_handler_##name,    \
        .child.display_handler = display_handler_##name, \
    }

#define MENU_ENTRY_PARENT(display_text, child)     \
    {                                              \
        .flags              = menu_flag_is_parent, \
        .text               = display_text,        \
        .parent.children    = child,               \
        .parent.child_count = ARRAY_SIZE(child),   \
    }

#define MENU_ENTRY_MULTI(display_text, child_item, name)                   \
    {                                                                      \
        .flags                 = menu_flag_is_parent | menu_flag_is_value, \
        .text                  = display_text,                             \
        .child.menu_handler    = menu_handler_##name,                      \
        .child.display_handler = display_handler_##name,                   \
        .parent.children       = child_item,                               \
        .parent.child_count    = ARRAY_SIZE(child_item),                   \
    }

#include "submenus/display_options.h"
#include "submenus/unicode_options.h"
#include "submenus/rgb_options.h"
#include "submenus/backlight_options.h"
#include "submenus/audio_options.h"
#include "submenus/haptic_options.h"
#include "submenus/pointing_device_options.h"
#include "submenus/keymap_config_options.h"
#include "submenus/rtc_options.h"
#include "submenus/user_settings_options.h"
#include "submenus/debugging_options.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Root menu

menu_entry_t root_entries[] = {
    MENU_ENTRY_MULTI("Display Option", display_option_entries, display),
#ifdef UNICODE_COMMON_ENABLE
    MENU_ENTRY_MULTI("Unicode Settings", unicode_entries, unicode_typing),
#endif // UNICODE_COMMON_ENABLE
#ifdef RGB_MATRIX_ENABLE
    MENU_ENTRY_MULTI("RGB Matrix Settings", rgb_matrix_entries, rm_enabled),
#endif // RGB_MATRIX_ENABLE
#ifdef RGBLIGHT_ENABLE
    MENU_ENTRY_MULTI("RGB Light Settings", rgb_light_entries, rgbenabled),
#endif // RGBLIGHT_ENABLE
#ifdef BACKLIGHT_ENABLE
    MENU_ENTRY_MULTI("Backlight Settings", backlight_entries, bl_level),
#endif // BACKLIGHT_ENABLE
#ifdef AUDIO_ENABLE
    MENU_ENTRY_MULTI("Audio Settings", audio_entries, audio_enabled),
#endif // AUDIO_ENABLE
#ifdef POINTING_DEVICE_ENABLE
    MENU_ENTRY_PARENT("Pointing Device Settings", pointing_entries),
#endif // POINTING_DEVICE_ENABLE
#if defined(HAPTIC_ENABLE)
    MENU_ENTRY_MULTI("Haptic Settings", haptic_entries, haptic_enabled),
#endif // HAPTIC_ENABLE
    MENU_ENTRY_PARENT("Keymap Settings", keymap_config_entries),
#if defined(RTC_ENABLE)
    MENU_ENTRY_PARENT("RTC Settings", rtc_config_entries),
#endif // RTC_ENABLE
    MENU_ENTRY_PARENT("User Settings", user_settings_option_entries),
    MENU_ENTRY_PARENT("Debug Settings", debug_entries),
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Root Title

menu_entry_t root = {
    .flags              = menu_flag_is_parent,
    .text               = "Configuration",
    .parent.children    = root_entries,
    .parent.child_count = ARRAY_SIZE(root_entries),
};
