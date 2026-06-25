################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ENC28J60/enc28j60.c \
../ENC28J60/network.c 

OBJS += \
./ENC28J60/enc28j60.o \
./ENC28J60/network.o 

C_DEPS += \
./ENC28J60/enc28j60.d \
./ENC28J60/network.d 


# Each subdirectory must supply rules for building sources it contributes
ENC28J60/%.o: ../ENC28J60/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


