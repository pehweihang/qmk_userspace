.SILENT:

MAKEFLAGS += --no-print-directory

# Deal with macOS
ifeq ($(shell uname -s),Darwin)
SED = gsed
ECHO = gecho
else
SED = sed
ECHO = echo
endif

QMK_USERSPACE := $(patsubst %/,%,$(dir $(shell realpath "$(lastword $(MAKEFILE_LIST))")))
ifeq ($(QMK_USERSPACE),)
    QMK_USERSPACE := $(shell pwd)
endif

QMK_FIRMWARE_ROOT = $(shell qmk config -ro user.qmk_home | cut -d= -f2 | sed -e 's@^None$$@@g')
ifeq ($(QMK_FIRMWARE_ROOT),)
    $(error Cannot determine qmk_firmware location. `qmk config -ro user.qmk_home` is not set)
endif

%:
	+$(MAKE) -C $(QMK_FIRMWARE_ROOT) $(MAKECMDGOALS) QMK_USERSPACE=$(QMK_USERSPACE)

.PHONY: format

format:
	@git ls-files | grep -E '\.(c|h|cpp|hpp|cxx|hxx|inc|inl)$$' | grep -v autocorrect_data.h | grep -vE '\.q[gf]f\.' | grep -vE '(ch|hal|mcu)conf\.h$$' | grep -vE 'board.[ch]$$' | grep -vE '.inl.h$$' | grep -vE 'mini-rv32ima.h$$' | while read file ; do \
		$(ECHO) -e "\e[38;5;14mFormatting: $$file\e[0m" ; \
		clang-format -i "$$file" ; \
	done
