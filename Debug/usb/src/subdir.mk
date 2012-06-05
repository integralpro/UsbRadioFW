################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../usb/src/usb_desc.c \
../usb/src/usb_endp.c \
../usb/src/usb_istr.c \
../usb/src/usb_prop.c \
../usb/src/usb_pwr.c 

OBJS += \
./usb/src/usb_desc.o \
./usb/src/usb_endp.o \
./usb/src/usb_istr.o \
./usb/src/usb_prop.o \
./usb/src/usb_pwr.o 

C_DEPS += \
./usb/src/usb_desc.d \
./usb/src/usb_endp.d \
./usb/src/usb_istr.d \
./usb/src/usb_prop.d \
./usb/src/usb_pwr.d 


# Each subdirectory must supply rules for building sources it contributes
usb/src/%.o: ../usb/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DSTM32F10X_MD -I../usb/include -I../usb_lib/include -I../include -I../CMSIS -O0 -g3 -Wall -c -fmessage-length=0 -mcpu=cortex-m3 -mthumb -ffunction-sections -fdata-sections -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


