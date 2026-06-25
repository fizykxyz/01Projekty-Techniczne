################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../AND_TECH_C/M32/AND_DS18B20/AND_DS18B20/HD44780.c \
../AND_TECH_C/M32/AND_DS18B20/AND_DS18B20/crc8.c \
../AND_TECH_C/M32/AND_DS18B20/AND_DS18B20/dallas_one_wire.c \
../AND_TECH_C/M32/AND_DS18B20/AND_DS18B20/main.c 

OBJS += \
./AND_TECH_C/M32/AND_DS18B20/AND_DS18B20/HD44780.o \
./AND_TECH_C/M32/AND_DS18B20/AND_DS18B20/crc8.o \
./AND_TECH_C/M32/AND_DS18B20/AND_DS18B20/dallas_one_wire.o \
./AND_TECH_C/M32/AND_DS18B20/AND_DS18B20/main.o 

C_DEPS += \
./AND_TECH_C/M32/AND_DS18B20/AND_DS18B20/HD44780.d \
./AND_TECH_C/M32/AND_DS18B20/AND_DS18B20/crc8.d \
./AND_TECH_C/M32/AND_DS18B20/AND_DS18B20/dallas_one_wire.d \
./AND_TECH_C/M32/AND_DS18B20/AND_DS18B20/main.d 


# Each subdirectory must supply rules for building sources it contributes
AND_TECH_C/M32/AND_DS18B20/AND_DS18B20/%.o: ../AND_TECH_C/M32/AND_DS18B20/AND_DS18B20/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega8 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


