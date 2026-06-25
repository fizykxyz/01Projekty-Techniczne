################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../demo/AVR_platf_spec.c \
../demo/T6963C.c \
../demo/demo_AVR.c 

OBJS += \
./demo/AVR_platf_spec.o \
./demo/T6963C.o \
./demo/demo_AVR.o 

C_DEPS += \
./demo/AVR_platf_spec.d \
./demo/T6963C.d \
./demo/demo_AVR.d 


# Each subdirectory must supply rules for building sources it contributes
demo/%.o: ../demo/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


