################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../T6963C/src/test/test_AVR.c \
../T6963C/src/test/test_LPC21.c \
../T6963C/src/test/test_STR91.c 

OBJS += \
./T6963C/src/test/test_AVR.o \
./T6963C/src/test/test_LPC21.o \
./T6963C/src/test/test_STR91.o 

C_DEPS += \
./T6963C/src/test/test_AVR.d \
./T6963C/src/test/test_LPC21.d \
./T6963C/src/test/test_STR91.d 


# Each subdirectory must supply rules for building sources it contributes
T6963C/src/test/%.o: ../T6963C/src/test/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


