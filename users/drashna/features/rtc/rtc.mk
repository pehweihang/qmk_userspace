VALID_RTC_DRIVER_TYPES := vendor custom ds3231 ds1307 pcf8523

ifeq ($(strip $(RTC_ENABLE)), yes)
    ifeq ($(filter $(RTC_DRIVER),$(VALID_RTC_DRIVER_TYPES)),)
        $(call CATASTROPHIC_ERROR,Invalid RTC_DRIVER,RTC_DRIVER="$(RTC_DRIVER)" is not a valid RTC type)
    else
        SRC += $(USER_PATH)/features/rtc/rtc.c
        OPT_DEFS += -DRTC_ENABLE
        OPT_DEFS += -D$(strip $(shell echo $(RTC_DRIVER) | tr '[:lower:]' '[:upper:]'))_RTC_DRIVER_ENABLE
        SRC += $(USER_PATH)/features/rtc/$(strip $(RTC_DRIVER)).c
        POST_CONFIG_H += $(USER_PATH)/features/rtc/config.h
        VPATH += $(USER_PATH)/features/rtc/

        ifeq ($(strip $(RTC_DRIVER)), vendor)
            OPT_DEFS += -DHAL_USE_RTC=TRUE
        else
            I2C_DRIVER_REQUIRED = yes
        endif
    endif
endif
