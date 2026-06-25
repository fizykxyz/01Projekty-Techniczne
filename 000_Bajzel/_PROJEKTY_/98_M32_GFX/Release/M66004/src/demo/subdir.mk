################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../M66004/src/demo/demo_AVR.c \
../M66004/src/demo/demo_LPC21.c \
../M66004/src/demo/demo_STR91.c 

OBJS += \
./M66004/src/demo/demo_AVR.o \
./M66004/src/demo/demo_LPC21.o \
./M66004/src/demo/demo_STR91.o 

C_DEPS += \
./M66004/src/demo/demo_AVR.d \
./M66004/src/demo/demo_LPC21.d \
./M66004/src/demo/demo_STR91.d 


# Each subdirectory must supply rules for building sources it contributes
M66004/src/demo/%.o: ../M66004/src/demo/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


