SRC += $(USER_PATH)/drashna.c \
        $(USER_PATH)/callbacks.c \
        $(USER_PATH)/keyrecords/process_records.c \
        $(USER_PATH)/keyrecords/tapping.c \
        $(USER_PATH)/eeconfig_users.c

# TOP_SYMBOLS = yes

DEBOUNCE_TYPE        = asym_eager_defer_pk
DEFERRED_EXEC_ENABLE = yes
OS_DETECTION_ENABLE  = yes
DEBUG_MATRIX_SCAN_RATE_ENABLE := no

ifeq ($(PLATFORM_KEY),chibios)
    # cortex-m4 has DSP+FPU support, so use hack to enable it for lib8tion
    ifeq ($(strip $(MCU)), cortex-m4)
        OPT_DEFS += -DFASTLED_TEENSY3
    endif
    CUSTOM_UNICODE_ENABLE ?= yes

else
    ifneq ($(strip $(LTO_SUPPORTED)), no)
        LTO_ENABLE        = yes
    endif
    SPACE_CADET_ENABLE    = no
    GRAVE_ESC_ENABLE      = no
endif

-include $(USER_PATH)/../../../qmk_secrets/rules.mk

ifeq ($(strip $(MAKE_BOOTLOADER)), yes)
    OPT_DEFS += -DMAKE_BOOTLOADER
endif

# At least until build.mk or the like drops, this is here to prevent
# VUSB boards from enabling NKRO, as they do not support it. Ideally
# this should be handled per keyboard, but until that happens ...
ifeq ($(strip $(PROTOCOL)), VUSB)
    NKRO_ENABLE       := no
endif

ifeq ($(strip $(PER_KEY_TAPPING)), yes)
    OPT_DEFS += -DPER_KEY_TAPPING
endif

UNICODE_ENABLE        := no
UNICODEMAP_ENABLE     := no
UCIS_ENABLE           := no
ifeq ($(strip $(CUSTOM_UNICODE_ENABLE)), yes)
    UNICODE_COMMON        := yes
    OPT_DEFS += -DCUSTOM_UNICODE_ENABLE
    SRC += $(USER_PATH)/keyrecords/unicode.c
else
    UNICODE_COMMON        := no
endif

CUSTOM_TAP_DANCE ?= yes
ifeq ($(strip $(CUSTOM_TAP_DANCE)), yes)
    ifeq ($(strip $(TAP_DANCE_ENABLE)), yes)
        SRC += $(USER_PATH)/keyrecords/tap_dances.c
    endif
endif

CUSTOM_RGBLIGHT ?= yes
ifeq ($(strip $(RGBLIGHT_ENABLE)), yes)
    ifeq ($(strip $(CUSTOM_RGBLIGHT)), yes)
        SRC += $(USER_PATH)/rgb/rgb_stuff.c
        OPT_DEFS += -DCUSTOM_RGBLIGHT
        ifeq ($(strip $(RGBLIGHT_NOEEPROM)), yes)
            OPT_DEFS += -DRGBLIGHT_NOEEPROM
        endif
        ifeq ($(strip $(RGBLIGHT_STARTUP_ANIMATION)), yes)
            OPT_DEFS += -DRGBLIGHT_STARTUP_ANIMATION
        endif
    endif
endif

CUSTOM_RGB_MATRIX ?= yes
ifeq ($(strip $(RGB_MATRIX_ENABLE)), yes)
    ifeq ($(strip $(CUSTOM_RGB_MATRIX)), yes)
        SRC += $(USER_PATH)/rgb/rgb_matrix_stuff.c
        OPT_DEFS += -DCUSTOM_RGB_MATRIX
        RGB_MATRIX_CUSTOM_USER = yes
    endif
endif

KEYLOGGER_ENABLE ?= no
ifdef CONSOLE_ENABLE
    ifeq ($(strip $(KEYLOGGER_ENABLE)), yes)
        OPT_DEFS += -DKEYLOGGER_ENABLE
    endif
endif

ifeq ($(strip $(I2C_SCANNER_ENABLE)), yes)
    OPT_DEFS += -DI2C_SCANNER_ENABLE
    CONSOLE_ENABLE := yes
endif

-include $(USER_PATH)/oled/rules.mk
-include $(USER_PATH)/pointing/rules.mk
-include $(USER_PATH)/split/rules.mk
-include $(USER_PATH)/painter/rules.mk

CUSTOM_BOOTMAGIC_ENABLE ?= yes
ifeq ($(strip $(CUSTOM_BOOTMAGIC_ENABLE)), yes)
    ifeq ($(strip $(BOOTMAGIC_ENABLE)), yes)
        SRC += bootmagic_better.c
    endif
endif

CUSTOM_DYNAMIC_MACROS_ENABLE ?= no
ifeq ($(strip $(CUSTOM_DYNAMIC_MACROS_ENABLE)), yes)
    SRC += $(USER_PATH)/keyrecords/dynamic_macros.c
    OPT_DEFS += -DCUSTOM_DYNAMIC_MACROS_ENABLE
endif

ifeq ($(strip $(HARDWARE_DEBUG_ENABLE)), yes)
    LTO_ENABLE := no
    OPT := 0
    OPT_DEFS += -g
endif

-include $(USER_PATH)/rtc/rtc.mk
-include $(KEYMAP_PATH)/post_rules.mk
