################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../DS18B20.c \
../graphic.c \
../hd44780.c \
../main.c \
../t6963c.c 

OBJS += \
./DS18B20.o \
./graphic.o \
./hd44780.o \
./main.o \
./t6963c.o 

C_DEPS += \
./DS18B20.d \
./graphic.d \
./hd44780.d \
./main.d \
./t6963c.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"E:\AVR-Programowanie\C\_PROJEKTY_\00_XYZ_AVRLIBS" -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


