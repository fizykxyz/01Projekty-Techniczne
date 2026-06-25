################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../DS18xxx/ds18xxx.c 

OBJS += \
./DS18xxx/ds18xxx.o 

C_DEPS += \
./DS18xxx/ds18xxx.d 


# Each subdirectory must supply rules for building sources it contributes
DS18xxx/%.o: ../DS18xxx/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -DMCU=$(AVRTARGETMCU) -mmcu=atmega32 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


