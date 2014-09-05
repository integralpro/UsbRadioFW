CROSS = arm-none-eabi-
AS = $(CROSS)as
CC = $(CROSS)gcc
LD = $(CROSS)gcc
OBJCOPY = $(CROSS)objcopy
SIZE = $(CROSS)size
STRIP = $(CROSS)strip
RM = rm -f

CFLAGS += -MMD -MP -mcpu=cortex-m3 -mthumb
LDFLAGS += -mcpu=cortex-m3 -mthumb
ASFLAGS += -mcpu=cortex-m3 -mthumb

ifneq ($(DEBUG),)
CFLAGS += -O0 -g
ASFLAGS += -g
endif

$(OBJ_OBJ)/%.o: %.c
	mkdir -p $(dir $@);
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJ_OBJ)/%.o: %.S
	mkdir -p $(dir $@);
	$(AS) $(ASFLAGS) $< -o $@

%.hex: %.elf
	$(OBJCOPY) -O ihex $< $@
	$(SIZE) --format=berkeley $<
