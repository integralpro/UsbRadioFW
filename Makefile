DEBUG = 1

BIN_DIR=bin
OBJ_OBJ=obj

include common.mk

INCLUDES = ./usb/include ./usb_lib/include ./include ./CMSIS
CFLAGS += -DSTM32F10X_MD \
	-Wall -std=c11 \
	-ffunction-sections -fdata-sections \
	$(INCLUDES:%=-I%)

LDFLAGS += -nostartfiles -nodefaultlibs -nostdlib -static \
	-T./stm32f103c8.ld \
	-Xlinker "-Map=$(BIN_DIR)/stm32f103c8.map" \
	-Xlinker -gc-sections

#ASFLAGS += 

CMSIS_SOURCES := CMSIS/system_stm32f10x.c
USB_SOURCES := usb/src/usb_desc.c usb/src/usb_endp.c usb/src/usb_istr.c usb/src/usb_prop.c usb/src/usb_pwr.c
USB_LIB_SOURCES := usb_lib/src/usb_core.c usb_lib/src/usb_init.c usb_lib/src/usb_int.c usb_lib/src/usb_mem.c usb_lib/src/usb_regs.c usb_lib/src/usb_sil.c

C_SOURCES := $(CMSIS_SOURCES) $(USB_SOURCES) $(USB_LIB_SOURCES) \
	crt1.c delay.c leds_driver.c main.c si4705_control.c si4705.c \
	sin_table.c stm32f10x_it.c

S_SOURCES := crt0.S

OBJECTS := $(C_SOURCES:%.c=%.o) $(S_SOURCES:%.S=%.o)
OBJECTS := $(OBJECTS:%=$(OBJ_OBJ)/%)
C_DEPS := $(OBJECTS:%.o=%.d)

.PHONY: all
all: bin/RadioFW.hex

.PHONY: clean
clean:
	$(RM) $(OBJECTS) $(C_DEPS)
	$(RM) -r ./obj
	$(RM) -r ./bin

$(BIN_DIR)/RadioFW.elf: $(OBJECTS)
	mkdir -p $(dir $@)
	$(LD) $(LDFLAGS) $^ -o $@

-include $(C_DEPS)
