################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../AND_TECH_C/M32/AND_LCD_ADC/AND_LCD_ADC/ADC.c \
../AND_TECH_C/M32/AND_LCD_ADC/AND_LCD_ADC/HD44780.c \
../AND_TECH_C/M32/AND_LCD_ADC/AND_LCD_ADC/main.c 

OBJS += \
./AND_TECH_C/M32/AND_LCD_ADC/AND_LCD_ADC/ADC.o \
./AND_TECH_C/M32/AND_LCD_ADC/AND_LCD_ADC/HD44780.o \
./AND_TECH_C/M32/AND_LCD_ADC/AND_LCD_ADC/main.o 

C_DEPS += \
./AND_TECH_C/M32/AND_LCD_ADC/AND_LCD_ADC/ADC.d \
./AND_TECH_C/M32/AND_LCD_ADC/AND_LCD_ADC/HD44780.d \
./AND_TECH_C/M32/AND_LCD_ADC/AND_LCD_ADC/main.d 


# Each subdirectory must supply rules for building sources it contributes
AND_TECH_C/M32/AND_LCD_ADC/AND_LCD_ADC/%.o: ../AND_TECH_C/M32/AND_LCD_ADC/AND_LCD_ADC/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega8 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


