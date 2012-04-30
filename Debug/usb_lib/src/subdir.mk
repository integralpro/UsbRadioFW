################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../usb_lib/src/usb_core.c \
../usb_lib/src/usb_init.c \
../usb_lib/src/usb_int.c \
../usb_lib/src/usb_mem.c \
../usb_lib/src/usb_regs.c \
../usb_lib/src/usb_sil.c 

OBJS += \
./usb_lib/src/usb_core.o \
./usb_lib/src/usb_init.o \
./usb_lib/src/usb_int.o \
./usb_lib/src/usb_mem.o \
./usb_lib/src/usb_regs.o \
./usb_lib/src/usb_sil.o 

C_DEPS += \
./usb_lib/src/usb_core.d \
./usb_lib/src/usb_init.d \
./usb_lib/src/usb_int.d \
./usb_lib/src/usb_mem.d \
./usb_lib/src/usb_regs.d \
./usb_lib/src/usb_sil.d 


# Each subdirectory must supply rules for building sources it contributes
usb_lib/src/%.o: ../usb_lib/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DSTM32F10X_MD -I../usb/include -I../usb_lib/include -I../include -I../CMSIS -I../std_drivers/include -O0 -g3 -Wall -c -fmessage-length=0 -mcpu=cortex-m3 -mthumb -ffunction-sections -fdata-sections -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


