################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ADC.c \
../HD44780.c \
../crc8.c \
../dallas_one_wire.c \
../main.c \
../twi.c \
../uart.c 

OBJS += \
./ADC.o \
./HD44780.o \
./crc8.o \
./dallas_one_wire.o \
./main.o \
./twi.o \
./uart.o 

C_DEPS += \
./ADC.d \
./HD44780.d \
./crc8.d \
./dallas_one_wire.d \
./main.d \
./twi.d \
./uart.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


