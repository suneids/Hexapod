################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Inc/HAL_STM32F103C6T6/ST/g474/system_stm32g4xx.c 

OBJS += \
./Inc/HAL_STM32F103C6T6/ST/g474/system_stm32g4xx.o 

C_DEPS += \
./Inc/HAL_STM32F103C6T6/ST/g474/system_stm32g4xx.d 


# Each subdirectory must supply rules for building sources it contributes
Inc/HAL_STM32F103C6T6/ST/g474/%.o Inc/HAL_STM32F103C6T6/ST/g474/%.su Inc/HAL_STM32F103C6T6/ST/g474/%.cyclo: ../Inc/HAL_STM32F103C6T6/ST/g474/%.c Inc/HAL_STM32F103C6T6/ST/g474/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32G474CEUx -DSTM32G4 -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Inc-2f-HAL_STM32F103C6T6-2f-ST-2f-g474

clean-Inc-2f-HAL_STM32F103C6T6-2f-ST-2f-g474:
	-$(RM) ./Inc/HAL_STM32F103C6T6/ST/g474/system_stm32g4xx.cyclo ./Inc/HAL_STM32F103C6T6/ST/g474/system_stm32g4xx.d ./Inc/HAL_STM32F103C6T6/ST/g474/system_stm32g4xx.o ./Inc/HAL_STM32F103C6T6/ST/g474/system_stm32g4xx.su

.PHONY: clean-Inc-2f-HAL_STM32F103C6T6-2f-ST-2f-g474

