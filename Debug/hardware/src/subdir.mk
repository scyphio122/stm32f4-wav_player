################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../hardware/src/EXTI.c \
../hardware/src/GPIO.c \
../hardware/src/HD44780.c \
../hardware/src/I2C.c \
../hardware/src/NEC_remote_controller.c \
../hardware/src/RCC.c \
../hardware/src/SysTick.c \
../hardware/src/TIM.c \
../hardware/src/USART.c \
../hardware/src/dac.c \
../hardware/src/fifo.c \
../hardware/src/misc.c \
../hardware/src/sd_card_reader.c \
../hardware/src/spi.c 

OBJS += \
./hardware/src/EXTI.o \
./hardware/src/GPIO.o \
./hardware/src/HD44780.o \
./hardware/src/I2C.o \
./hardware/src/NEC_remote_controller.o \
./hardware/src/RCC.o \
./hardware/src/SysTick.o \
./hardware/src/TIM.o \
./hardware/src/USART.o \
./hardware/src/dac.o \
./hardware/src/fifo.o \
./hardware/src/misc.o \
./hardware/src/sd_card_reader.o \
./hardware/src/spi.o 

C_DEPS += \
./hardware/src/EXTI.d \
./hardware/src/GPIO.d \
./hardware/src/HD44780.d \
./hardware/src/I2C.d \
./hardware/src/NEC_remote_controller.d \
./hardware/src/RCC.d \
./hardware/src/SysTick.d \
./hardware/src/TIM.d \
./hardware/src/USART.d \
./hardware/src/dac.d \
./hardware/src/fifo.d \
./hardware/src/misc.d \
./hardware/src/sd_card_reader.d \
./hardware/src/spi.d 


# Each subdirectory must supply rules for building sources it contributes
hardware/src/%.o: ../hardware/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F407xx -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -I"D:\Eclipse_workspace\Eclipse_Wav\FatFS" -I"D:\Eclipse_workspace\Eclipse_Wav\hardware\inc" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


