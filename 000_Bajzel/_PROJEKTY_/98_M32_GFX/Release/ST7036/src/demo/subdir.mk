################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ST7036/src/demo/demo_AVR.c \
../ST7036/src/demo/demo_LPC21.c \
../ST7036/src/demo/demo_STR91.c 

OBJS += \
./ST7036/src/demo/demo_AVR.o \
./ST7036/src/demo/demo_LPC21.o \
./ST7036/src/demo/demo_STR91.o 

C_DEPS += \
./ST7036/src/demo/demo_AVR.d \
./ST7036/src/demo/demo_LPC21.d \
./ST7036/src/demo/demo_STR91.d 


# Each subdirectory must supply rules for building sources it contributes
ST7036/src/demo/%.o: ../ST7036/src/demo/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


