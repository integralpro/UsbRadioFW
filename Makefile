DEBUG = 1

ifneq ($(DEBUG),)
CFLAGS += -O0 -g
ASFLAGS += -g
endif

INCLUDES = ./usb/include ./usb_lib/include ./include ./CMSIS
CFLAGS += -DSTM32F10X_MD \
	-Wall -c -mcpu=cortex-m3 -mthumb \
	-ffunction-sections -fdata-sections \
	$(INCLUDES:%=-I%)

LDFLAGS += -nostartfiles -nodefaultlibs -nostdlib -static \
	-mcpu=cortex-m3 -mthumb -T./stm32f103c8.ld \
	-Xlinker "-Map=stm32f103c8.map" \
	-Xlinker -gc-sections

ASFLAGS += -mcpu=cortex-m3 -mthumb

include common.mk

CMSIS_SOURCES := CMSIS/system_stm32f10x.c
#CMSIS/core_cm3.c 
USB_SOURCES := usb/src/usb_desc.c usb/src/usb_endp.c usb/src/usb_istr.c usb/src/usb_prop.c usb/src/usb_pwr.c
USB_LIB_SOURCES := usb_lib/src/usb_core.c usb_lib/src/usb_init.c usb_lib/src/usb_int.c usb_lib/src/usb_mem.c usb_lib/src/usb_regs.c usb_lib/src/usb_sil.c

C_SOURCES := $(CMSIS_SOURCES) $(USB_SOURCES) $(USB_LIB_SOURCES) \
	crt1.c delay.c leds_driver.c main.c si4705_control.c si4705.c \
	sin_table.c stm32f10x_it.c

S_SOURCES := crt0.S delay_low.S

OBJECTS := $(C_SOURCES:%.c=%.o) $(S_SOURCES:%.S=%.o)

RadioFW.elf: $(OBJECTS)
	$(LD) $(LDFLAGS) $^ -o $@

.PHONY: all
all: RadioFW.hex	

.PHONY: clean
clean:
	$(RM) RadioFW.elf $(OBJECTS)
