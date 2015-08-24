################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/Eclipse_workspace/stm32f4_mylib/FatFS/diskio.c \
D:/Eclipse_workspace/stm32f4_mylib/FatFS/ff.c 

OBJS += \
./stm32f4_mylib/FatFS/diskio.o \
./stm32f4_mylib/FatFS/ff.o 

C_DEPS += \
./stm32f4_mylib/FatFS/diskio.d \
./stm32f4_mylib/FatFS/ff.d 


# Each subdirectory must supply rules for building sources it contributes
stm32f4_mylib/FatFS/diskio.o: D:/Eclipse_workspace/stm32f4_mylib/FatFS/diskio.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F407xx -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -I"D:\Eclipse_workspace\stm32f4_mylib\inc" -I"D:\Eclipse_workspace\stm32f4_mylib\FatFS" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32f4_mylib/FatFS/ff.o: D:/Eclipse_workspace/stm32f4_mylib/FatFS/ff.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F407xx -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -I"D:\Eclipse_workspace\stm32f4_mylib\inc" -I"D:\Eclipse_workspace\stm32f4_mylib\FatFS" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


