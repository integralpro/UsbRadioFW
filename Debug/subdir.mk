################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../crt1.c \
../delay.c \
../leds_driver.c \
../main.c \
../si4705.c \
../sin_table.c \
../stm32f10x_it.c 

S_UPPER_SRCS += \
../crt0.S 

OBJS += \
./crt0.o \
./crt1.o \
./delay.o \
./leds_driver.o \
./main.o \
./si4705.o \
./sin_table.o \
./stm32f10x_it.o 

C_DEPS += \
./crt1.d \
./delay.d \
./leds_driver.d \
./main.d \
./si4705.d \
./sin_table.d \
./stm32f10x_it.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.S
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Assembler'
	arm-none-eabi-as -mcpu=cortex-m3 -mthumb -g -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DSTM32F10X_MD -I../usb/include -I../usb_lib/include -I../include -I../CMSIS -I../std_drivers/include -O0 -g3 -Wall -c -fmessage-length=0 -mcpu=cortex-m3 -mthumb -ffunction-sections -fdata-sections -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


