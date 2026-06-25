################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../T6963C/include/demo/T6963C.c \
../T6963C/include/demo/demo_AVR.c 

OBJS += \
./T6963C/include/demo/T6963C.o \
./T6963C/include/demo/demo_AVR.o 

C_DEPS += \
./T6963C/include/demo/T6963C.d \
./T6963C/include/demo/demo_AVR.d 


# Each subdirectory must supply rules for building sources it contributes
T6963C/include/demo/%.o: ../T6963C/include/demo/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


