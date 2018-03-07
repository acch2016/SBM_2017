################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/FSMLCD.c \
../Sources/GPIO.c \
../Sources/GlobalFunctions.c \
../Sources/I2C.c \
../Sources/LCDNokia5110.c \
../Sources/LCDNokia5110Images.c \
../Sources/MCG.c \
../Sources/Memory.c \
../Sources/NVIC.c \
../Sources/RTC.c \
../Sources/SPI.c \
../Sources/UART.c \
../Sources/main.c \
../Sources/menu.c 

OBJS += \
./Sources/FSMLCD.o \
./Sources/GPIO.o \
./Sources/GlobalFunctions.o \
./Sources/I2C.o \
./Sources/LCDNokia5110.o \
./Sources/LCDNokia5110Images.o \
./Sources/MCG.o \
./Sources/Memory.o \
./Sources/NVIC.o \
./Sources/RTC.o \
./Sources/SPI.o \
./Sources/UART.o \
./Sources/main.o \
./Sources/menu.o 

C_DEPS += \
./Sources/FSMLCD.d \
./Sources/GPIO.d \
./Sources/GlobalFunctions.d \
./Sources/I2C.d \
./Sources/LCDNokia5110.d \
./Sources/LCDNokia5110Images.d \
./Sources/MCG.d \
./Sources/Memory.d \
./Sources/NVIC.d \
./Sources/RTC.d \
./Sources/SPI.d \
./Sources/UART.d \
./Sources/main.d \
./Sources/menu.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/%.o: ../Sources/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -I"../Sources" -I"../Includes" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


