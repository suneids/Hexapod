################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Inc/HAL_STM32F103C6T6/src/modbus.c \
../Inc/HAL_STM32F103C6T6/src/soft_i2c.c \
../Inc/HAL_STM32F103C6T6/src/soft_uart.c \
../Inc/HAL_STM32F103C6T6/src/utilities.c 

OBJS += \
./Inc/HAL_STM32F103C6T6/src/modbus.o \
./Inc/HAL_STM32F103C6T6/src/soft_i2c.o \
./Inc/HAL_STM32F103C6T6/src/soft_uart.o \
./Inc/HAL_STM32F103C6T6/src/utilities.o 

C_DEPS += \
./Inc/HAL_STM32F103C6T6/src/modbus.d \
./Inc/HAL_STM32F103C6T6/src/soft_i2c.d \
./Inc/HAL_STM32F103C6T6/src/soft_uart.d \
./Inc/HAL_STM32F103C6T6/src/utilities.d 


# Each subdirectory must supply rules for building sources it contributes
Inc/HAL_STM32F103C6T6/src/%.o Inc/HAL_STM32F103C6T6/src/%.su Inc/HAL_STM32F103C6T6/src/%.cyclo: ../Inc/HAL_STM32F103C6T6/src/%.c Inc/HAL_STM32F103C6T6/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32G0 -DSTM32G0B1CBTx -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Inc-2f-HAL_STM32F103C6T6-2f-src

clean-Inc-2f-HAL_STM32F103C6T6-2f-src:
	-$(RM) ./Inc/HAL_STM32F103C6T6/src/modbus.cyclo ./Inc/HAL_STM32F103C6T6/src/modbus.d ./Inc/HAL_STM32F103C6T6/src/modbus.o ./Inc/HAL_STM32F103C6T6/src/modbus.su ./Inc/HAL_STM32F103C6T6/src/soft_i2c.cyclo ./Inc/HAL_STM32F103C6T6/src/soft_i2c.d ./Inc/HAL_STM32F103C6T6/src/soft_i2c.o ./Inc/HAL_STM32F103C6T6/src/soft_i2c.su ./Inc/HAL_STM32F103C6T6/src/soft_uart.cyclo ./Inc/HAL_STM32F103C6T6/src/soft_uart.d ./Inc/HAL_STM32F103C6T6/src/soft_uart.o ./Inc/HAL_STM32F103C6T6/src/soft_uart.su ./Inc/HAL_STM32F103C6T6/src/utilities.cyclo ./Inc/HAL_STM32F103C6T6/src/utilities.d ./Inc/HAL_STM32F103C6T6/src/utilities.o ./Inc/HAL_STM32F103C6T6/src/utilities.su

.PHONY: clean-Inc-2f-HAL_STM32F103C6T6-2f-src

