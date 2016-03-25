################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
LD_SRCS += \
../src/lscript.ld 

C_SRCS += \
../src/adau1761.c \
../src/audioManager.c \
../src/audioRx.c \
../src/bufferPool_d.c \
../src/chunk_d.c \
../src/main.c 

OBJS += \
./src/adau1761.o \
./src/audioManager.o \
./src/audioRx.o \
./src/bufferPool_d.o \
./src/chunk_d.o \
./src/main.o 

C_DEPS += \
./src/adau1761.d \
./src/audioManager.d \
./src/audioRx.d \
./src/bufferPool_d.d \
./src/chunk_d.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM gcc compiler'
	arm-xilinx-eabi-gcc -Wall -O0 -g3 -c -fmessage-length=0 -MT"$@" -I../../freertos_bsp_3_1_1/ps7_cortexa9_0/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


