################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SED1520/src/demo/demo_AVR.c \
../SED1520/src/demo/demo_LPC21.c \
../SED1520/src/demo/demo_STR91.c 

OBJS += \
./SED1520/src/demo/demo_AVR.o \
./SED1520/src/demo/demo_LPC21.o \
./SED1520/src/demo/demo_STR91.o 

C_DEPS += \
./SED1520/src/demo/demo_AVR.d \
./SED1520/src/demo/demo_LPC21.d \
./SED1520/src/demo/demo_STR91.d 


# Each subdirectory must supply rules for building sources it contributes
SED1520/src/demo/%.o: ../SED1520/src/demo/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


