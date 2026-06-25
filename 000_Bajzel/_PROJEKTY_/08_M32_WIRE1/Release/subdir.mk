################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../1wire.c \
../1wire_basic.c \
../1wirebb.c \
../KS0108lib.c \
../graphics.c 

OBJS += \
./1wire.o \
./1wire_basic.o \
./1wirebb.o \
./KS0108lib.o \
./graphics.o 

C_DEPS += \
./1wire.d \
./1wire_basic.d \
./1wirebb.d \
./KS0108lib.d \
./graphics.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


