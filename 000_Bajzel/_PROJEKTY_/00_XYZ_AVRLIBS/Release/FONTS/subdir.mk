################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FONTS/font_5x8\ -\ Kopia.c 

OBJS += \
./FONTS/font_5x8\ -\ Kopia.o 

C_DEPS += \
./FONTS/font_5x8\ -\ Kopia.d 


# Each subdirectory must supply rules for building sources it contributes
FONTS/font_5x8\ -\ Kopia.o: ../FONTS/font_5x8\ -\ Kopia.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -DMCU=$(AVRTARGETMCU) -mmcu=atmega32 -DF_CPU=16000000UL -MMD -MP -MF"FONTS/font_5x8 - Kopia.d" -MT"FONTS/font_5x8\ -\ Kopia.d" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


