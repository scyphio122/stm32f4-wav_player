################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/Eclipse_workspace/stm32f4_mylib/EXTI.c \
D:/Eclipse_workspace/stm32f4_mylib/GPIO.c \
D:/Eclipse_workspace/stm32f4_mylib/HD44780.c \
D:/Eclipse_workspace/stm32f4_mylib/I2C.c \
D:/Eclipse_workspace/stm32f4_mylib/NEC_remote_controller.c \
D:/Eclipse_workspace/stm32f4_mylib/RCC.c \
D:/Eclipse_workspace/stm32f4_mylib/TIM.c \
D:/Eclipse_workspace/stm32f4_mylib/USART.c \
D:/Eclipse_workspace/stm32f4_mylib/fifo.c \
D:/Eclipse_workspace/stm32f4_mylib/misc.c \
D:/Eclipse_workspace/stm32f4_mylib/sd_card_reader.c \
D:/Eclipse_workspace/stm32f4_mylib/spi.c 

OBJS += \
./stm32f4_mylib/EXTI.o \
./stm32f4_mylib/GPIO.o \
./stm32f4_mylib/HD44780.o \
./stm32f4_mylib/I2C.o \
./stm32f4_mylib/NEC_remote_controller.o \
./stm32f4_mylib/RCC.o \
./stm32f4_mylib/TIM.o \
./stm32f4_mylib/USART.o \
./stm32f4_mylib/fifo.o \
./stm32f4_mylib/misc.o \
./stm32f4_mylib/sd_card_reader.o \
./stm32f4_mylib/spi.o 

C_DEPS += \
./stm32f4_mylib/EXTI.d \
./stm32f4_mylib/GPIO.d \
./stm32f4_mylib/HD44780.d \
./stm32f4_mylib/I2C.d \
./stm32f4_mylib/NEC_remote_controller.d \
./stm32f4_mylib/RCC.d \
./stm32f4_mylib/TIM.d \
./stm32f4_mylib/USART.d \
./stm32f4_mylib/fifo.d \
./stm32f4_mylib/misc.d \
./stm32f4_mylib/sd_card_reader.d \
./stm32f4_mylib/spi.d 


# Each subdirectory must supply rules for building sources it contributes
stm32f4_mylib/EXTI.o: D:/Eclipse_workspace/stm32f4_mylib/EXTI.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F407xx -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -I"D:\Eclipse_workspace\stm32f4_template\stm32f4_mylib" -I"D:\Eclipse_workspace\stm32f4_template\stm32f4_mylib\FatFS" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32f4_mylib/GPIO.o: D:/Eclipse_workspace/stm32f4_mylib/GPIO.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F407xx -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -I"D:\Eclipse_workspace\stm32f4_template\stm32f4_mylib" -I"D:\Eclipse_workspace\stm32f4_template\stm32f4_mylib\FatFS" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32f4_mylib/HD44780.o: D:/Eclipse_workspace/stm32f4_mylib/HD44780.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F407xx -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -I"D:\Eclipse_workspace\stm32f4_template\stm32f4_mylib" -I"D:\Eclipse_workspace\stm32f4_template\stm32f4_mylib\FatFS" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32f4_mylib/I2C.o: D:/Eclipse_workspace/stm32f4_mylib/I2C.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F407xx -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -I"D:\Eclipse_workspace\stm32f4_template\stm32f4_mylib" -I"D:\Eclipse_workspace\stm32f4_template\stm32f4_mylib\FatFS" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32f4_mylib/NEC_remote_controller.o: D:/Eclipse_workspace/stm32f4_mylib/NEC_remote_controller.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F407xx -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -I"D:\Eclipse_workspace\stm32f4_template\stm32f4_mylib" -I"D:\Eclipse_workspace\stm32f4_template\stm32f4_mylib\FatFS" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32f4_mylib/RCC.o: D:/Eclipse_workspace/stm32f4_mylib/RCC.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F407xx -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -I"D:\Eclipse_workspace\stm32f4_template\stm32f4_mylib" -I"D:\Eclipse_workspace\stm32f4_template\stm32f4_mylib\FatFS" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32f4_mylib/TIM.o: D:/Eclipse_workspace/stm32f4_mylib/TIM.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F407xx -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -I"D:\Eclipse_workspace\stm32f4_template\stm32f4_mylib" -I"D:\Eclipse_workspace\stm32f4_template\stm32f4_mylib\FatFS" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32f4_mylib/USART.o: D:/Eclipse_workspace/stm32f4_mylib/USART.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F407xx -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -I"D:\Eclipse_workspace\stm32f4_template\stm32f4_mylib" -I"D:\Eclipse_workspace\stm32f4_template\stm32f4_mylib\FatFS" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32f4_mylib/fifo.o: D:/Eclipse_workspace/stm32f4_mylib/fifo.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F407xx -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -I"D:\Eclipse_workspace\stm32f4_template\stm32f4_mylib" -I"D:\Eclipse_workspace\stm32f4_template\stm32f4_mylib\FatFS" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32f4_mylib/misc.o: D:/Eclipse_workspace/stm32f4_mylib/misc.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F407xx -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -I"D:\Eclipse_workspace\stm32f4_template\stm32f4_mylib" -I"D:\Eclipse_workspace\stm32f4_template\stm32f4_mylib\FatFS" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32f4_mylib/sd_card_reader.o: D:/Eclipse_workspace/stm32f4_mylib/sd_card_reader.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F407xx -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -I"D:\Eclipse_workspace\stm32f4_template\stm32f4_mylib" -I"D:\Eclipse_workspace\stm32f4_template\stm32f4_mylib\FatFS" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32f4_mylib/spi.o: D:/Eclipse_workspace/stm32f4_mylib/spi.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F407xx -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -I"D:\Eclipse_workspace\stm32f4_template\stm32f4_mylib" -I"D:\Eclipse_workspace\stm32f4_template\stm32f4_mylib\FatFS" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


