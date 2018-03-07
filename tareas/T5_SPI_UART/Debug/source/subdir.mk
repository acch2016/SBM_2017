################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/LCDNokia5110.c \
../source/LCDNokia5110Images.c \
../source/SPI.c \
../source/UART.c \
../source/main.c \
../source/user.c 

OBJS += \
./source/LCDNokia5110.o \
./source/LCDNokia5110Images.o \
./source/SPI.o \
./source/UART.o \
./source/main.o \
./source/user.o 

C_DEPS += \
./source/LCDNokia5110.d \
./source/LCDNokia5110Images.d \
./source/SPI.d \
./source/UART.d \
./source/main.d \
./source/user.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -flto  -g3 -D"CPU_MK64FN1M0VDC12" -I../freertos/Source/include -I../startup -I../board -I../utilities -I../freertos/Source/portable/GCC/ARM_CM4F -I../CMSIS -I../source -I../drivers -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


