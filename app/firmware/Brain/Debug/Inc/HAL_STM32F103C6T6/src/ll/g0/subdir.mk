################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Inc/HAL_STM32F103C6T6/src/ll/g0/adc_g0.c \
../Inc/HAL_STM32F103C6T6/src/ll/g0/dac_g0.c \
../Inc/HAL_STM32F103C6T6/src/ll/g0/dma_g0.c \
../Inc/HAL_STM32F103C6T6/src/ll/g0/fdcan.c \
../Inc/HAL_STM32F103C6T6/src/ll/g0/flash_ll_g0.c \
../Inc/HAL_STM32F103C6T6/src/ll/g0/gpio_g0.c \
../Inc/HAL_STM32F103C6T6/src/ll/g0/i2c_g0.c \
../Inc/HAL_STM32F103C6T6/src/ll/g0/nvstore_g0.c \
../Inc/HAL_STM32F103C6T6/src/ll/g0/pwm_g0.c \
../Inc/HAL_STM32F103C6T6/src/ll/g0/spi_g0.c \
../Inc/HAL_STM32F103C6T6/src/ll/g0/tim_g0.c \
../Inc/HAL_STM32F103C6T6/src/ll/g0/usart_g0.c 

OBJS += \
./Inc/HAL_STM32F103C6T6/src/ll/g0/adc_g0.o \
./Inc/HAL_STM32F103C6T6/src/ll/g0/dac_g0.o \
./Inc/HAL_STM32F103C6T6/src/ll/g0/dma_g0.o \
./Inc/HAL_STM32F103C6T6/src/ll/g0/fdcan.o \
./Inc/HAL_STM32F103C6T6/src/ll/g0/flash_ll_g0.o \
./Inc/HAL_STM32F103C6T6/src/ll/g0/gpio_g0.o \
./Inc/HAL_STM32F103C6T6/src/ll/g0/i2c_g0.o \
./Inc/HAL_STM32F103C6T6/src/ll/g0/nvstore_g0.o \
./Inc/HAL_STM32F103C6T6/src/ll/g0/pwm_g0.o \
./Inc/HAL_STM32F103C6T6/src/ll/g0/spi_g0.o \
./Inc/HAL_STM32F103C6T6/src/ll/g0/tim_g0.o \
./Inc/HAL_STM32F103C6T6/src/ll/g0/usart_g0.o 

C_DEPS += \
./Inc/HAL_STM32F103C6T6/src/ll/g0/adc_g0.d \
./Inc/HAL_STM32F103C6T6/src/ll/g0/dac_g0.d \
./Inc/HAL_STM32F103C6T6/src/ll/g0/dma_g0.d \
./Inc/HAL_STM32F103C6T6/src/ll/g0/fdcan.d \
./Inc/HAL_STM32F103C6T6/src/ll/g0/flash_ll_g0.d \
./Inc/HAL_STM32F103C6T6/src/ll/g0/gpio_g0.d \
./Inc/HAL_STM32F103C6T6/src/ll/g0/i2c_g0.d \
./Inc/HAL_STM32F103C6T6/src/ll/g0/nvstore_g0.d \
./Inc/HAL_STM32F103C6T6/src/ll/g0/pwm_g0.d \
./Inc/HAL_STM32F103C6T6/src/ll/g0/spi_g0.d \
./Inc/HAL_STM32F103C6T6/src/ll/g0/tim_g0.d \
./Inc/HAL_STM32F103C6T6/src/ll/g0/usart_g0.d 


# Each subdirectory must supply rules for building sources it contributes
Inc/HAL_STM32F103C6T6/src/ll/g0/%.o Inc/HAL_STM32F103C6T6/src/ll/g0/%.su Inc/HAL_STM32F103C6T6/src/ll/g0/%.cyclo: ../Inc/HAL_STM32F103C6T6/src/ll/g0/%.c Inc/HAL_STM32F103C6T6/src/ll/g0/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32G474CEUx -DSTM32G4 -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Inc-2f-HAL_STM32F103C6T6-2f-src-2f-ll-2f-g0

clean-Inc-2f-HAL_STM32F103C6T6-2f-src-2f-ll-2f-g0:
	-$(RM) ./Inc/HAL_STM32F103C6T6/src/ll/g0/adc_g0.cyclo ./Inc/HAL_STM32F103C6T6/src/ll/g0/adc_g0.d ./Inc/HAL_STM32F103C6T6/src/ll/g0/adc_g0.o ./Inc/HAL_STM32F103C6T6/src/ll/g0/adc_g0.su ./Inc/HAL_STM32F103C6T6/src/ll/g0/dac_g0.cyclo ./Inc/HAL_STM32F103C6T6/src/ll/g0/dac_g0.d ./Inc/HAL_STM32F103C6T6/src/ll/g0/dac_g0.o ./Inc/HAL_STM32F103C6T6/src/ll/g0/dac_g0.su ./Inc/HAL_STM32F103C6T6/src/ll/g0/dma_g0.cyclo ./Inc/HAL_STM32F103C6T6/src/ll/g0/dma_g0.d ./Inc/HAL_STM32F103C6T6/src/ll/g0/dma_g0.o ./Inc/HAL_STM32F103C6T6/src/ll/g0/dma_g0.su ./Inc/HAL_STM32F103C6T6/src/ll/g0/fdcan.cyclo ./Inc/HAL_STM32F103C6T6/src/ll/g0/fdcan.d ./Inc/HAL_STM32F103C6T6/src/ll/g0/fdcan.o ./Inc/HAL_STM32F103C6T6/src/ll/g0/fdcan.su ./Inc/HAL_STM32F103C6T6/src/ll/g0/flash_ll_g0.cyclo ./Inc/HAL_STM32F103C6T6/src/ll/g0/flash_ll_g0.d ./Inc/HAL_STM32F103C6T6/src/ll/g0/flash_ll_g0.o ./Inc/HAL_STM32F103C6T6/src/ll/g0/flash_ll_g0.su ./Inc/HAL_STM32F103C6T6/src/ll/g0/gpio_g0.cyclo ./Inc/HAL_STM32F103C6T6/src/ll/g0/gpio_g0.d ./Inc/HAL_STM32F103C6T6/src/ll/g0/gpio_g0.o ./Inc/HAL_STM32F103C6T6/src/ll/g0/gpio_g0.su ./Inc/HAL_STM32F103C6T6/src/ll/g0/i2c_g0.cyclo ./Inc/HAL_STM32F103C6T6/src/ll/g0/i2c_g0.d ./Inc/HAL_STM32F103C6T6/src/ll/g0/i2c_g0.o ./Inc/HAL_STM32F103C6T6/src/ll/g0/i2c_g0.su ./Inc/HAL_STM32F103C6T6/src/ll/g0/nvstore_g0.cyclo ./Inc/HAL_STM32F103C6T6/src/ll/g0/nvstore_g0.d ./Inc/HAL_STM32F103C6T6/src/ll/g0/nvstore_g0.o ./Inc/HAL_STM32F103C6T6/src/ll/g0/nvstore_g0.su ./Inc/HAL_STM32F103C6T6/src/ll/g0/pwm_g0.cyclo ./Inc/HAL_STM32F103C6T6/src/ll/g0/pwm_g0.d ./Inc/HAL_STM32F103C6T6/src/ll/g0/pwm_g0.o ./Inc/HAL_STM32F103C6T6/src/ll/g0/pwm_g0.su ./Inc/HAL_STM32F103C6T6/src/ll/g0/spi_g0.cyclo ./Inc/HAL_STM32F103C6T6/src/ll/g0/spi_g0.d ./Inc/HAL_STM32F103C6T6/src/ll/g0/spi_g0.o ./Inc/HAL_STM32F103C6T6/src/ll/g0/spi_g0.su ./Inc/HAL_STM32F103C6T6/src/ll/g0/tim_g0.cyclo ./Inc/HAL_STM32F103C6T6/src/ll/g0/tim_g0.d ./Inc/HAL_STM32F103C6T6/src/ll/g0/tim_g0.o ./Inc/HAL_STM32F103C6T6/src/ll/g0/tim_g0.su ./Inc/HAL_STM32F103C6T6/src/ll/g0/usart_g0.cyclo ./Inc/HAL_STM32F103C6T6/src/ll/g0/usart_g0.d ./Inc/HAL_STM32F103C6T6/src/ll/g0/usart_g0.o ./Inc/HAL_STM32F103C6T6/src/ll/g0/usart_g0.su

.PHONY: clean-Inc-2f-HAL_STM32F103C6T6-2f-src-2f-ll-2f-g0

