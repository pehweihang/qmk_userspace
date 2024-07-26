ifeq ($(strip $(BOOTLOADER)), tinyuf2)
    MCU_LDSCRIPT = STM32F303xC_tinyuf2
endif
