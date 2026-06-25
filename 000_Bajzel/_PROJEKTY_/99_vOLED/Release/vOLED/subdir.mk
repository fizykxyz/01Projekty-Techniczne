################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../vOLED/virtualOLED.c \
../vOLED/virtualOLED_UART.c 

OBJS += \
./vOLED/virtualOLED.o \
./vOLED/virtualOLED_UART.o 

C_DEPS += \
./vOLED/virtualOLED.d \
./vOLED/virtualOLED_UART.d 


# Each subdirectory must supply rules for building sources it contributes
vOLED/%.o: ../vOLED/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=1000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


