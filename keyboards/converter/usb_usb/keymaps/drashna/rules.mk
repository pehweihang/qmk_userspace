ifeq ($(strip $(KEYBOARD)), converter/usb_usb/pro_micro)
    F_CPU = 16000000
    BLUETOOTH_ENABLE = yes
    LTO_ENABLE = yes
endif
BLUETOOTH_DRIVER = bluefruit_le
