################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../LaserArray/Src/laser_array.c 

OBJS += \
./LaserArray/Src/laser_array.o 

C_DEPS += \
./LaserArray/Src/laser_array.d 


# Each subdirectory must supply rules for building sources it contributes
LaserArray/Src/%.o LaserArray/Src/%.su: ../LaserArray/Src/%.c LaserArray/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../LaserArray/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-LaserArray-2f-Src

clean-LaserArray-2f-Src:
	-$(RM) ./LaserArray/Src/laser_array.d ./LaserArray/Src/laser_array.o ./LaserArray/Src/laser_array.su

.PHONY: clean-LaserArray-2f-Src

