################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../KS0108B/src/disp/KS0108B.c 

OBJS += \
./KS0108B/src/disp/KS0108B.o 

C_DEPS += \
./KS0108B/src/disp/KS0108B.d 


# Each subdirectory must supply rules for building sources it contributes
KS0108B/src/disp/%.o: ../KS0108B/src/disp/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


