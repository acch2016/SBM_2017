################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../utilities/fsl_debug_console.c \
../utilities/fsl_sbrk.c 

OBJS += \
./utilities/fsl_debug_console.o \
./utilities/fsl_sbrk.o 

C_DEPS += \
./utilities/fsl_debug_console.d \
./utilities/fsl_sbrk.d 


# Each subdirectory must supply rules for building sources it contributes
utilities/%.o: ../utilities/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -ffreestanding -fno-builtin -Wall  -g -DDEBUG -DCPU_MK64FN1M0VMD12 -DUSE_RTOS=1 -DPRINTF_ADVANCED_ENABLE=1 -DFSL_RTOS_FREE_RTOS -DFRDM_K64F -DFREEDOM -I../freertos/Source/portable/GCC/ARM_CM4F -I../lwip/src/include -I../CMSIS/Include -I../lwip/port/arch -I../freertos/Source/include -I../sources -I../devices -I../drivers -I../lwip/port -I../utilities -I../devices/gcc -I../freertos/Source -I../lwip/contrib/apps -std=gnu99 -mapcs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


