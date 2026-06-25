################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../AND_TECH_C/M32/AND_7SEG/AND_7SEG/7SEG.c \
../AND_TECH_C/M32/AND_7SEG/AND_7SEG/main.c 

OBJS += \
./AND_TECH_C/M32/AND_7SEG/AND_7SEG/7SEG.o \
./AND_TECH_C/M32/AND_7SEG/AND_7SEG/main.o 

C_DEPS += \
./AND_TECH_C/M32/AND_7SEG/AND_7SEG/7SEG.d \
./AND_TECH_C/M32/AND_7SEG/AND_7SEG/main.d 


# Each subdirectory must supply rules for building sources it contributes
AND_TECH_C/M32/AND_7SEG/AND_7SEG/%.o: ../AND_TECH_C/M32/AND_7SEG/AND_7SEG/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega8 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


