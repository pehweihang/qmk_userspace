CUSTOM_RGBLIGHT ?= yes
ifeq ($(strip $(RGBLIGHT_ENABLE)), yes)
    ifeq ($(strip $(CUSTOM_RGBLIGHT)), yes)
        SRC += $(USER_PATH)/rgb/rgb_stuff.c
        OPT_DEFS += -DCUSTOM_RGBLIGHT
        CONFIG_H += $(USER_PATH)/rgb/rgblight_config.h
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
        CONFIG_H += $(USER_PATH)/rgb/rgb_matrix_config.h
        POST_CONFIG_H += $(USER_PATH)/rgb/post_rgb_matrix.h

        OPT_DEFS += -DCUSTOM_RGB_MATRIX
        RGB_MATRIX_CUSTOM_USER = yes
    endif
endif
