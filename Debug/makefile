################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include usb_lib/src/subdir.mk
-include usb/src/subdir.mk
-include CMSIS/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: RadioFW.elf

# Tool invocations
RadioFW.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross GCC Linker'
	arm-none-eabi-gcc -nostartfiles -nodefaultlibs -nostdlib -static -mcpu=cortex-m3 -mthumb -T../stm32f103c8.ld -Xlinker "-Map=stm32f103c8.map" -Xlinker -gc-sections -o "RadioFW.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) RadioFW.elf
	-@echo ' '

post-build:
	-@echo 'Extracting elf container to hex format'
	-arm-none-eabi-objcopy -O ihex RadioFW.elf RadioFW.hex; arm-none-eabi-size --format=berkeley RadioFW.elf
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
