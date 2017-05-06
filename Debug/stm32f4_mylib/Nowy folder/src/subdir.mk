################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/Eclipse_workspace/stm32f4_mylib/Nowy\ folder/src/EXTI.c \
D:/Eclipse_workspace/stm32f4_mylib/Nowy\ folder/src/GPIO.c \
D:/Eclipse_workspace/stm32f4_mylib/Nowy\ folder/src/HD44780.c \
D:/Eclipse_workspace/stm32f4_mylib/Nowy\ folder/src/I2C.c \
D:/Eclipse_workspace/stm32f4_mylib/Nowy\ folder/src/NEC_remote_controller.c \
D:/Eclipse_workspace/stm32f4_mylib/Nowy\ folder/src/RCC.c \
D:/Eclipse_workspace/stm32f4_mylib/Nowy\ folder/src/TIM.c \
D:/Eclipse_workspace/stm32f4_mylib/Nowy\ folder/src/USART.c \
D:/Eclipse_workspace/stm32f4_mylib/Nowy\ folder/src/dac.c \
D:/Eclipse_workspace/stm32f4_mylib/Nowy\ folder/src/fifo.c \
D:/Eclipse_workspace/stm32f4_mylib/Nowy\ folder/src/misc.c \
D:/Eclipse_workspace/stm32f4_mylib/Nowy\ folder/src/sd_card_reader.c \
D:/Eclipse_workspace/stm32f4_mylib/Nowy\ folder/src/spi.c 

OBJS += \
./stm32f4_mylib/Nowy\ folder/src/EXTI.o \
./stm32f4_mylib/Nowy\ folder/src/GPIO.o \
./stm32f4_mylib/Nowy\ folder/src/HD44780.o \
./stm32f4_mylib/Nowy\ folder/src/I2C.o \
./stm32f4_mylib/Nowy\ folder/src/NEC_remote_controller.o \
./stm32f4_mylib/Nowy\ folder/src/RCC.o \
./stm32f4_mylib/Nowy\ folder/src/TIM.o \
./stm32f4_mylib/Nowy\ folder/src/USART.o \
./stm32f4_mylib/Nowy\ folder/src/dac.o \
./stm32f4_mylib/Nowy\ folder/src/fifo.o \
./stm32f4_mylib/Nowy\ folder/src/misc.o \
./stm32f4_mylib/Nowy\ folder/src/sd_card_reader.o \
./stm32f4_mylib/Nowy\ folder/src/spi.o 

C_DEPS += \
./stm32f4_mylib/Nowy\ folder/src/EXTI.d \
./stm32f4_mylib/Nowy\ folder/src/GPIO.d \
./stm32f4_mylib/Nowy\ folder/src/HD44780.d \
./stm32f4_mylib/Nowy\ folder/src/I2C.d \
./stm32f4_mylib/Nowy\ folder/src/NEC_remote_controller.d \
./stm32f4_mylib/Nowy\ folder/src/RCC.d \
./stm32f4_mylib/Nowy\ folder/src/TIM.d \
./stm32f4_mylib/Nowy\ folder/src/USART.d \
./stm32f4_mylib/Nowy\ folder/src/dac.d \
./stm32f4_mylib/Nowy\ folder/src/fifo.d \
./stm32f4_mylib/Nowy\ folder/src/misc.d \
./stm32f4_mylib/Nowy\ folder/src/sd_card_reader.d \
./stm32f4_mylib/Nowy\ folder/src/spi.d 


# Each subdirectory must supply rules for building sources it contributes
stm32f4_mylib/Nowy\ folder/src/EXTI.o: D:/Eclipse_workspace/stm32f4_mylib/Nowy\ folder/src/EXTI.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F407xx -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -I"D:\Eclipse_workspace\stm32f4_mylib\inc" -I"D:\Eclipse_workspace\stm32f4_mylib\FatFS" -std=gnu11 -MMD -MP -MF"stm32f4_mylib/Nowy folder/src/EXTI.d" -MT"stm32f4_mylib/Nowy\ folder/src/EXTI.d" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32f4_mylib/Nowy\ folder/src/GPIO.o: D:/Eclipse_workspace/stm32f4_mylib/Nowy\ folder/src/GPIO.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F407xx -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -I"D:\Eclipse_workspace\stm32f4_mylib\inc" -I"D:\Eclipse_workspace\stm32f4_mylib\FatFS" -std=gnu11 -MMD -MP -MF"stm32f4_mylib/Nowy folder/src/GPIO.d" -MT"stm32f4_mylib/Nowy\ folder/src/GPIO.d" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32f4_mylib/Nowy\ folder/src/HD44780.o: D:/Eclipse_workspace/stm32f4_mylib/Nowy\ folder/src/HD44780.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F407xx -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -I"D:\Eclipse_workspace\stm32f4_mylib\inc" -I"D:\Eclipse_workspace\stm32f4_mylib\FatFS" -std=gnu11 -MMD -MP -MF"stm32f4_mylib/Nowy folder/src/HD44780.d" -MT"stm32f4_mylib/Nowy\ folder/src/HD44780.d" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32f4_mylib/Nowy\ folder/src/I2C.o: D:/Eclipse_workspace/stm32f4_mylib/Nowy\ folder/src/I2C.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F407xx -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -I"D:\Eclipse_workspace\stm32f4_mylib\inc" -I"D:\Eclipse_workspace\stm32f4_mylib\FatFS" -std=gnu11 -MMD -MP -MF"stm32f4_mylib/Nowy folder/src/I2C.d" -MT"stm32f4_mylib/Nowy\ folder/src/I2C.d" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32f4_mylib/Nowy\ folder/src/NEC_remote_controller.o: D:/Eclipse_workspace/stm32f4_mylib/Nowy\ folder/src/NEC_remote_controller.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F407xx -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -I"D:\Eclipse_workspace\stm32f4_mylib\inc" -I"D:\Eclipse_workspace\stm32f4_mylib\FatFS" -std=gnu11 -MMD -MP -MF"stm32f4_mylib/Nowy folder/src/NEC_remote_controller.d" -MT"stm32f4_mylib/Nowy\ folder/src/NEC_remote_controller.d" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32f4_mylib/Nowy\ folder/src/RCC.o: D:/Eclipse_workspace/stm32f4_mylib/Nowy\ folder/src/RCC.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F407xx -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -I"D:\Eclipse_workspace\stm32f4_mylib\inc" -I"D:\Eclipse_workspace\stm32f4_mylib\FatFS" -std=gnu11 -MMD -MP -MF"stm32f4_mylib/Nowy folder/src/RCC.d" -MT"stm32f4_mylib/Nowy\ folder/src/RCC.d" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32f4_mylib/Nowy\ folder/src/TIM.o: D:/Eclipse_workspace/stm32f4_mylib/Nowy\ folder/src/TIM.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F407xx -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -I"D:\Eclipse_workspace\stm32f4_mylib\inc" -I"D:\Eclipse_workspace\stm32f4_mylib\FatFS" -std=gnu11 -MMD -MP -MF"stm32f4_mylib/Nowy folder/src/TIM.d" -MT"stm32f4_mylib/Nowy\ folder/src/TIM.d" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32f4_mylib/Nowy\ folder/src/USART.o: D:/Eclipse_workspace/stm32f4_mylib/Nowy\ folder/src/USART.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F407xx -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -I"D:\Eclipse_workspace\stm32f4_mylib\inc" -I"D:\Eclipse_workspace\stm32f4_mylib\FatFS" -std=gnu11 -MMD -MP -MF"stm32f4_mylib/Nowy folder/src/USART.d" -MT"stm32f4_mylib/Nowy\ folder/src/USART.d" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32f4_mylib/Nowy\ folder/src/dac.o: D:/Eclipse_workspace/stm32f4_mylib/Nowy\ folder/src/dac.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F407xx -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -I"D:\Eclipse_workspace\stm32f4_mylib\inc" -I"D:\Eclipse_workspace\stm32f4_mylib\FatFS" -std=gnu11 -MMD -MP -MF"stm32f4_mylib/Nowy folder/src/dac.d" -MT"stm32f4_mylib/Nowy\ folder/src/dac.d" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32f4_mylib/Nowy\ folder/src/fifo.o: D:/Eclipse_workspace/stm32f4_mylib/Nowy\ folder/src/fifo.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F407xx -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -I"D:\Eclipse_workspace\stm32f4_mylib\inc" -I"D:\Eclipse_workspace\stm32f4_mylib\FatFS" -std=gnu11 -MMD -MP -MF"stm32f4_mylib/Nowy folder/src/fifo.d" -MT"stm32f4_mylib/Nowy\ folder/src/fifo.d" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32f4_mylib/Nowy\ folder/src/misc.o: D:/Eclipse_workspace/stm32f4_mylib/Nowy\ folder/src/misc.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F407xx -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -I"D:\Eclipse_workspace\stm32f4_mylib\inc" -I"D:\Eclipse_workspace\stm32f4_mylib\FatFS" -std=gnu11 -MMD -MP -MF"stm32f4_mylib/Nowy folder/src/misc.d" -MT"stm32f4_mylib/Nowy\ folder/src/misc.d" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32f4_mylib/Nowy\ folder/src/sd_card_reader.o: D:/Eclipse_workspace/stm32f4_mylib/Nowy\ folder/src/sd_card_reader.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F407xx -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -I"D:\Eclipse_workspace\stm32f4_mylib\inc" -I"D:\Eclipse_workspace\stm32f4_mylib\FatFS" -std=gnu11 -MMD -MP -MF"stm32f4_mylib/Nowy folder/src/sd_card_reader.d" -MT"stm32f4_mylib/Nowy\ folder/src/sd_card_reader.d" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32f4_mylib/Nowy\ folder/src/spi.o: D:/Eclipse_workspace/stm32f4_mylib/Nowy\ folder/src/spi.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F407xx -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -I"D:\Eclipse_workspace\stm32f4_mylib\inc" -I"D:\Eclipse_workspace\stm32f4_mylib\FatFS" -std=gnu11 -MMD -MP -MF"stm32f4_mylib/Nowy folder/src/spi.d" -MT"stm32f4_mylib/Nowy\ folder/src/spi.d" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


