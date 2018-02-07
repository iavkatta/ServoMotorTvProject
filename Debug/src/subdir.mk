################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/IRDrv.c \
../src/MotorControl.c \
../src/MotorDriver.c \
../src/MotorDriverMX1508.c \
../src/exti.c \
../src/gpio.c \
../src/main.c \
../src/syscalls.c \
../src/system_stm32f10x.c \
../src/timers.c 

OBJS += \
./src/IRDrv.o \
./src/MotorControl.o \
./src/MotorDriver.o \
./src/MotorDriverMX1508.o \
./src/exti.o \
./src/gpio.o \
./src/main.o \
./src/syscalls.o \
./src/system_stm32f10x.o \
./src/timers.o 

C_DEPS += \
./src/IRDrv.d \
./src/MotorControl.d \
./src/MotorDriver.d \
./src/MotorDriverMX1508.d \
./src/exti.d \
./src/gpio.d \
./src/main.d \
./src/syscalls.d \
./src/system_stm32f10x.d \
./src/timers.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -DDEBUG -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -I"C:/Users/dzhen001/Documents/STWorkPlace/PlayGround/RL_KOSTAL_2017/StdPeriph_Driver/inc" -I"C:/Users/dzhen001/Documents/STWorkPlace/PlayGround/RL_KOSTAL_2017/inc" -I"C:/Users/dzhen001/Documents/STWorkPlace/PlayGround/RL_KOSTAL_2017/CMSIS/device" -I"C:/Users/dzhen001/Documents/STWorkPlace/PlayGround/RL_KOSTAL_2017/CMSIS/core" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


