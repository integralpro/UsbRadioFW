CROSS = arm-none-eabi-
AS = $(CROSS)as
CC = $(CROSS)gcc
LD = $(CROSS)gcc
OBJCOPY = $(CROSS)objcopy
SIZE = $(CROSS)size
STRIP = $(CROSS)strip
RM = rm -f

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

%.o: %.S
	$(AS) $(ASFLAGS) $< -o $@

%.hex: %.elf
	$(OBJCOPY) -O ihex $< $@
	$(SIZE) --format=berkeley $<
