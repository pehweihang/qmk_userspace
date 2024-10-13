include $(USER_PATH)/features/rtc/rtc.mk
VPATH += $(USER_PATH)/features/

KEYBOARD_LOCK_ENABLE ?= yes

USERSPACE_FEATURES = \
    KEYBOARD_LOCK \
    LAYER_LOCK \
    LAYER_MAP \
    ORBITAL_MOUSE \
    WATCHDOG

define HANDLE_MY_FEATURE
    # $$(info "Processing: $1_ENABLE $$(USER_PATH)/features/$2.c")
    SRC += $$(wildcard $$(USER_PATH)/features/$2.c)
    OPT_DEFS += -D$1_ENABLE
endef

$(foreach F,$(USERSPACE_FEATURES),\
    $(if $(filter yes, $(strip $($(F)_ENABLE))),\
        $(eval $(call HANDLE_MY_FEATURE,$(F),$(shell echo $(F) | tr '[:upper:]' '[:lower:]'))) \
    ) \
)

ifeq ($(strip $(WATCHDOG_ENABLE)), yes)
    OPT_DEFS += -DHAL_USE_WDG=TRUE
endif
