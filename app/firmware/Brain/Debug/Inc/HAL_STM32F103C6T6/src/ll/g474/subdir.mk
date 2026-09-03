################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Inc/HAL_STM32F103C6T6/src/ll/g474/adc_g474.c \
../Inc/HAL_STM32F103C6T6/src/ll/g474/dac_g474.c \
../Inc/HAL_STM32F103C6T6/src/ll/g474/dma_g474.c \
../Inc/HAL_STM32F103C6T6/src/ll/g474/fdcan.c \
../Inc/HAL_STM32F103C6T6/src/ll/g474/flash_ll_g474.c \
../Inc/HAL_STM32F103C6T6/src/ll/g474/gpio_g474.c \
../Inc/HAL_STM32F103C6T6/src/ll/g474/i2c_g474.c \
../Inc/HAL_STM32F103C6T6/src/ll/g474/nvstore_g474.c \
../Inc/HAL_STM32F103C6T6/src/ll/g474/pwm_g474.c \
../Inc/HAL_STM32F103C6T6/src/ll/g474/spi_g474.c \
../Inc/HAL_STM32F103C6T6/src/ll/g474/tim_g474.c \
../Inc/HAL_STM32F103C6T6/src/ll/g474/usart_g474.c 

OBJS += \
./Inc/HAL_STM32F103C6T6/src/ll/g474/adc_g474.o \
./Inc/HAL_STM32F103C6T6/src/ll/g474/dac_g474.o \
./Inc/HAL_STM32F103C6T6/src/ll/g474/dma_g474.o \
./Inc/HAL_STM32F103C6T6/src/ll/g474/fdcan.o \
./Inc/HAL_STM32F103C6T6/src/ll/g474/flash_ll_g474.o \
./Inc/HAL_STM32F103C6T6/src/ll/g474/gpio_g474.o \
./Inc/HAL_STM32F103C6T6/src/ll/g474/i2c_g474.o \
./Inc/HAL_STM32F103C6T6/src/ll/g474/nvstore_g474.o \
./Inc/HAL_STM32F103C6T6/src/ll/g474/pwm_g474.o \
./Inc/HAL_STM32F103C6T6/src/ll/g474/spi_g474.o \
./Inc/HAL_STM32F103C6T6/src/ll/g474/tim_g474.o \
./Inc/HAL_STM32F103C6T6/src/ll/g474/usart_g474.o 

C_DEPS += \
./Inc/HAL_STM32F103C6T6/src/ll/g474/adc_g474.d \
./Inc/HAL_STM32F103C6T6/src/ll/g474/dac_g474.d \
./Inc/HAL_STM32F103C6T6/src/ll/g474/dma_g474.d \
./Inc/HAL_STM32F103C6T6/src/ll/g474/fdcan.d \
./Inc/HAL_STM32F103C6T6/src/ll/g474/flash_ll_g474.d \
./Inc/HAL_STM32F103C6T6/src/ll/g474/gpio_g474.d \
./Inc/HAL_STM32F103C6T6/src/ll/g474/i2c_g474.d \
./Inc/HAL_STM32F103C6T6/src/ll/g474/nvstore_g474.d \
./Inc/HAL_STM32F103C6T6/src/ll/g474/pwm_g474.d \
./Inc/HAL_STM32F103C6T6/src/ll/g474/spi_g474.d \
./Inc/HAL_STM32F103C6T6/src/ll/g474/tim_g474.d \
./Inc/HAL_STM32F103C6T6/src/ll/g474/usart_g474.d 


# Each subdirectory must supply rules for building sources it contributes
Inc/HAL_STM32F103C6T6/src/ll/g474/%.o Inc/HAL_STM32F103C6T6/src/ll/g474/%.su Inc/HAL_STM32F103C6T6/src/ll/g474/%.cyclo: ../Inc/HAL_STM32F103C6T6/src/ll/g474/%.c Inc/HAL_STM32F103C6T6/src/ll/g474/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32G474CEUx -DSTM32G4 -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Inc-2f-HAL_STM32F103C6T6-2f-src-2f-ll-2f-g474

clean-Inc-2f-HAL_STM32F103C6T6-2f-src-2f-ll-2f-g474:
	-$(RM) ./Inc/HAL_STM32F103C6T6/src/ll/g474/adc_g474.cyclo ./Inc/HAL_STM32F103C6T6/src/ll/g474/adc_g474.d ./Inc/HAL_STM32F103C6T6/src/ll/g474/adc_g474.o ./Inc/HAL_STM32F103C6T6/src/ll/g474/adc_g474.su ./Inc/HAL_STM32F103C6T6/src/ll/g474/dac_g474.cyclo ./Inc/HAL_STM32F103C6T6/src/ll/g474/dac_g474.d ./Inc/HAL_STM32F103C6T6/src/ll/g474/dac_g474.o ./Inc/HAL_STM32F103C6T6/src/ll/g474/dac_g474.su ./Inc/HAL_STM32F103C6T6/src/ll/g474/dma_g474.cyclo ./Inc/HAL_STM32F103C6T6/src/ll/g474/dma_g474.d ./Inc/HAL_STM32F103C6T6/src/ll/g474/dma_g474.o ./Inc/HAL_STM32F103C6T6/src/ll/g474/dma_g474.su ./Inc/HAL_STM32F103C6T6/src/ll/g474/fdcan.cyclo ./Inc/HAL_STM32F103C6T6/src/ll/g474/fdcan.d ./Inc/HAL_STM32F103C6T6/src/ll/g474/fdcan.o ./Inc/HAL_STM32F103C6T6/src/ll/g474/fdcan.su ./Inc/HAL_STM32F103C6T6/src/ll/g474/flash_ll_g474.cyclo ./Inc/HAL_STM32F103C6T6/src/ll/g474/flash_ll_g474.d ./Inc/HAL_STM32F103C6T6/src/ll/g474/flash_ll_g474.o ./Inc/HAL_STM32F103C6T6/src/ll/g474/flash_ll_g474.su ./Inc/HAL_STM32F103C6T6/src/ll/g474/gpio_g474.cyclo ./Inc/HAL_STM32F103C6T6/src/ll/g474/gpio_g474.d ./Inc/HAL_STM32F103C6T6/src/ll/g474/gpio_g474.o ./Inc/HAL_STM32F103C6T6/src/ll/g474/gpio_g474.su ./Inc/HAL_STM32F103C6T6/src/ll/g474/i2c_g474.cyclo ./Inc/HAL_STM32F103C6T6/src/ll/g474/i2c_g474.d ./Inc/HAL_STM32F103C6T6/src/ll/g474/i2c_g474.o ./Inc/HAL_STM32F103C6T6/src/ll/g474/i2c_g474.su ./Inc/HAL_STM32F103C6T6/src/ll/g474/nvstore_g474.cyclo ./Inc/HAL_STM32F103C6T6/src/ll/g474/nvstore_g474.d ./Inc/HAL_STM32F103C6T6/src/ll/g474/nvstore_g474.o ./Inc/HAL_STM32F103C6T6/src/ll/g474/nvstore_g474.su ./Inc/HAL_STM32F103C6T6/src/ll/g474/pwm_g474.cyclo ./Inc/HAL_STM32F103C6T6/src/ll/g474/pwm_g474.d ./Inc/HAL_STM32F103C6T6/src/ll/g474/pwm_g474.o ./Inc/HAL_STM32F103C6T6/src/ll/g474/pwm_g474.su ./Inc/HAL_STM32F103C6T6/src/ll/g474/spi_g474.cyclo ./Inc/HAL_STM32F103C6T6/src/ll/g474/spi_g474.d ./Inc/HAL_STM32F103C6T6/src/ll/g474/spi_g474.o ./Inc/HAL_STM32F103C6T6/src/ll/g474/spi_g474.su ./Inc/HAL_STM32F103C6T6/src/ll/g474/tim_g474.cyclo ./Inc/HAL_STM32F103C6T6/src/ll/g474/tim_g474.d ./Inc/HAL_STM32F103C6T6/src/ll/g474/tim_g474.o ./Inc/HAL_STM32F103C6T6/src/ll/g474/tim_g474.su ./Inc/HAL_STM32F103C6T6/src/ll/g474/usart_g474.cyclo ./Inc/HAL_STM32F103C6T6/src/ll/g474/usart_g474.d ./Inc/HAL_STM32F103C6T6/src/ll/g474/usart_g474.o ./Inc/HAL_STM32F103C6T6/src/ll/g474/usart_g474.su

.PHONY: clean-Inc-2f-HAL_STM32F103C6T6-2f-src-2f-ll-2f-g474

