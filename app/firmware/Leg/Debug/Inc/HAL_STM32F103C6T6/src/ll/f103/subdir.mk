################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Inc/HAL_STM32F103C6T6/src/ll/f103/adc_f103.c \
../Inc/HAL_STM32F103C6T6/src/ll/f103/dma_f103.c \
../Inc/HAL_STM32F103C6T6/src/ll/f103/exti_f103.c \
../Inc/HAL_STM32F103C6T6/src/ll/f103/flash_ll_f103.c \
../Inc/HAL_STM32F103C6T6/src/ll/f103/gpio_f103.c \
../Inc/HAL_STM32F103C6T6/src/ll/f103/i2c_f103.c \
../Inc/HAL_STM32F103C6T6/src/ll/f103/nvstore_f103.c \
../Inc/HAL_STM32F103C6T6/src/ll/f103/pwm_f103.c \
../Inc/HAL_STM32F103C6T6/src/ll/f103/spi_f103.c \
../Inc/HAL_STM32F103C6T6/src/ll/f103/tim_f103.c \
../Inc/HAL_STM32F103C6T6/src/ll/f103/usart_f103.c 

OBJS += \
./Inc/HAL_STM32F103C6T6/src/ll/f103/adc_f103.o \
./Inc/HAL_STM32F103C6T6/src/ll/f103/dma_f103.o \
./Inc/HAL_STM32F103C6T6/src/ll/f103/exti_f103.o \
./Inc/HAL_STM32F103C6T6/src/ll/f103/flash_ll_f103.o \
./Inc/HAL_STM32F103C6T6/src/ll/f103/gpio_f103.o \
./Inc/HAL_STM32F103C6T6/src/ll/f103/i2c_f103.o \
./Inc/HAL_STM32F103C6T6/src/ll/f103/nvstore_f103.o \
./Inc/HAL_STM32F103C6T6/src/ll/f103/pwm_f103.o \
./Inc/HAL_STM32F103C6T6/src/ll/f103/spi_f103.o \
./Inc/HAL_STM32F103C6T6/src/ll/f103/tim_f103.o \
./Inc/HAL_STM32F103C6T6/src/ll/f103/usart_f103.o 

C_DEPS += \
./Inc/HAL_STM32F103C6T6/src/ll/f103/adc_f103.d \
./Inc/HAL_STM32F103C6T6/src/ll/f103/dma_f103.d \
./Inc/HAL_STM32F103C6T6/src/ll/f103/exti_f103.d \
./Inc/HAL_STM32F103C6T6/src/ll/f103/flash_ll_f103.d \
./Inc/HAL_STM32F103C6T6/src/ll/f103/gpio_f103.d \
./Inc/HAL_STM32F103C6T6/src/ll/f103/i2c_f103.d \
./Inc/HAL_STM32F103C6T6/src/ll/f103/nvstore_f103.d \
./Inc/HAL_STM32F103C6T6/src/ll/f103/pwm_f103.d \
./Inc/HAL_STM32F103C6T6/src/ll/f103/spi_f103.d \
./Inc/HAL_STM32F103C6T6/src/ll/f103/tim_f103.d \
./Inc/HAL_STM32F103C6T6/src/ll/f103/usart_f103.d 


# Each subdirectory must supply rules for building sources it contributes
Inc/HAL_STM32F103C6T6/src/ll/f103/%.o Inc/HAL_STM32F103C6T6/src/ll/f103/%.su Inc/HAL_STM32F103C6T6/src/ll/f103/%.cyclo: ../Inc/HAL_STM32F103C6T6/src/ll/f103/%.c Inc/HAL_STM32F103C6T6/src/ll/f103/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32G0 -DSTM32G0B1CBTx -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Inc-2f-HAL_STM32F103C6T6-2f-src-2f-ll-2f-f103

clean-Inc-2f-HAL_STM32F103C6T6-2f-src-2f-ll-2f-f103:
	-$(RM) ./Inc/HAL_STM32F103C6T6/src/ll/f103/adc_f103.cyclo ./Inc/HAL_STM32F103C6T6/src/ll/f103/adc_f103.d ./Inc/HAL_STM32F103C6T6/src/ll/f103/adc_f103.o ./Inc/HAL_STM32F103C6T6/src/ll/f103/adc_f103.su ./Inc/HAL_STM32F103C6T6/src/ll/f103/dma_f103.cyclo ./Inc/HAL_STM32F103C6T6/src/ll/f103/dma_f103.d ./Inc/HAL_STM32F103C6T6/src/ll/f103/dma_f103.o ./Inc/HAL_STM32F103C6T6/src/ll/f103/dma_f103.su ./Inc/HAL_STM32F103C6T6/src/ll/f103/exti_f103.cyclo ./Inc/HAL_STM32F103C6T6/src/ll/f103/exti_f103.d ./Inc/HAL_STM32F103C6T6/src/ll/f103/exti_f103.o ./Inc/HAL_STM32F103C6T6/src/ll/f103/exti_f103.su ./Inc/HAL_STM32F103C6T6/src/ll/f103/flash_ll_f103.cyclo ./Inc/HAL_STM32F103C6T6/src/ll/f103/flash_ll_f103.d ./Inc/HAL_STM32F103C6T6/src/ll/f103/flash_ll_f103.o ./Inc/HAL_STM32F103C6T6/src/ll/f103/flash_ll_f103.su ./Inc/HAL_STM32F103C6T6/src/ll/f103/gpio_f103.cyclo ./Inc/HAL_STM32F103C6T6/src/ll/f103/gpio_f103.d ./Inc/HAL_STM32F103C6T6/src/ll/f103/gpio_f103.o ./Inc/HAL_STM32F103C6T6/src/ll/f103/gpio_f103.su ./Inc/HAL_STM32F103C6T6/src/ll/f103/i2c_f103.cyclo ./Inc/HAL_STM32F103C6T6/src/ll/f103/i2c_f103.d ./Inc/HAL_STM32F103C6T6/src/ll/f103/i2c_f103.o ./Inc/HAL_STM32F103C6T6/src/ll/f103/i2c_f103.su ./Inc/HAL_STM32F103C6T6/src/ll/f103/nvstore_f103.cyclo ./Inc/HAL_STM32F103C6T6/src/ll/f103/nvstore_f103.d ./Inc/HAL_STM32F103C6T6/src/ll/f103/nvstore_f103.o ./Inc/HAL_STM32F103C6T6/src/ll/f103/nvstore_f103.su ./Inc/HAL_STM32F103C6T6/src/ll/f103/pwm_f103.cyclo ./Inc/HAL_STM32F103C6T6/src/ll/f103/pwm_f103.d ./Inc/HAL_STM32F103C6T6/src/ll/f103/pwm_f103.o ./Inc/HAL_STM32F103C6T6/src/ll/f103/pwm_f103.su ./Inc/HAL_STM32F103C6T6/src/ll/f103/spi_f103.cyclo ./Inc/HAL_STM32F103C6T6/src/ll/f103/spi_f103.d ./Inc/HAL_STM32F103C6T6/src/ll/f103/spi_f103.o ./Inc/HAL_STM32F103C6T6/src/ll/f103/spi_f103.su ./Inc/HAL_STM32F103C6T6/src/ll/f103/tim_f103.cyclo ./Inc/HAL_STM32F103C6T6/src/ll/f103/tim_f103.d ./Inc/HAL_STM32F103C6T6/src/ll/f103/tim_f103.o ./Inc/HAL_STM32F103C6T6/src/ll/f103/tim_f103.su ./Inc/HAL_STM32F103C6T6/src/ll/f103/usart_f103.cyclo ./Inc/HAL_STM32F103C6T6/src/ll/f103/usart_f103.d ./Inc/HAL_STM32F103C6T6/src/ll/f103/usart_f103.o ./Inc/HAL_STM32F103C6T6/src/ll/f103/usart_f103.su

.PHONY: clean-Inc-2f-HAL_STM32F103C6T6-2f-src-2f-ll-2f-f103

