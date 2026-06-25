################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../PCD8544/src/disp/PCD8544.c 

OBJS += \
./PCD8544/src/disp/PCD8544.o 

C_DEPS += \
./PCD8544/src/disp/PCD8544.d 


# Each subdirectory must supply rules for building sources it contributes
PCD8544/src/disp/%.o: ../PCD8544/src/disp/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


