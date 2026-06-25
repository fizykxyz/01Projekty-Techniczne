################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../KS0108B/src/test/test_AVR.c \
../KS0108B/src/test/test_LPC21.c \
../KS0108B/src/test/test_STR91.c 

OBJS += \
./KS0108B/src/test/test_AVR.o \
./KS0108B/src/test/test_LPC21.o \
./KS0108B/src/test/test_STR91.o 

C_DEPS += \
./KS0108B/src/test/test_AVR.d \
./KS0108B/src/test/test_LPC21.d \
./KS0108B/src/test/test_STR91.d 


# Each subdirectory must supply rules for building sources it contributes
KS0108B/src/test/%.o: ../KS0108B/src/test/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


