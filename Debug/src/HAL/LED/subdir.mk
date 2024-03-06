################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/HAL/LED/HAL_LED.c \
../src/HAL/LED/HAL_LED_CONFIG.c 

C_DEPS += \
./src/HAL/LED/HAL_LED.d \
./src/HAL/LED/HAL_LED_CONFIG.d 

OBJS += \
./src/HAL/LED/HAL_LED.o \
./src/HAL/LED/HAL_LED_CONFIG.o 


# Each subdirectory must supply rules for building sources it contributes
src/HAL/LED/%.o: ../src/HAL/LED/%.c src/HAL/LED/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F401xC -DUSE_HAL_DRIVER -DHSE_VALUE=25000000 -I"D:\ITI_9M_ES_Intake44\ARM_Workspace\STM32F401_Drivers\LIB" -I"D:\ITI_9M_ES_Intake44\ARM_Workspace\STM32F401_Drivers\include\MCAL\GPIO" -I"../include" -I"D:\ITI_9M_ES_Intake44\ARM_Workspace\STM32F401_Drivers\include\MCAL\RCC" -I"D:\ITI_9M_ES_Intake44\ARM_Workspace\STM32F401_Drivers\src" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


