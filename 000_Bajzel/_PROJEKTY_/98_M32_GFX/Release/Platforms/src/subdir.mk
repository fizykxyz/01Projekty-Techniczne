################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Platforms/src/AVR_platf_spec.c \
../Platforms/src/LPC21_platf_spec.c \
../Platforms/src/STR91_platf_spec.c \
../Platforms/src/TEMPLATE_platf_spec.c 

S_UPPER_SRCS += \
../Platforms/src/LPC21_startup.S \
../Platforms/src/STR91_startup.S 

OBJS += \
./Platforms/src/AVR_platf_spec.o \
./Platforms/src/LPC21_platf_spec.o \
./Platforms/src/LPC21_startup.o \
./Platforms/src/STR91_platf_spec.o \
./Platforms/src/STR91_startup.o \
./Platforms/src/TEMPLATE_platf_spec.o 

S_UPPER_DEPS += \
./Platforms/src/LPC21_startup.d \
./Platforms/src/STR91_startup.d 

C_DEPS += \
./Platforms/src/AVR_platf_spec.d \
./Platforms/src/LPC21_platf_spec.d \
./Platforms/src/STR91_platf_spec.d \
./Platforms/src/TEMPLATE_platf_spec.d 


# Each subdirectory must supply rules for building sources it contributes
Platforms/src/%.o: ../Platforms/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Platforms/src/%.o: ../Platforms/src/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Assembler'
	avr-gcc -x assembler-with-cpp -mmcu=atmega32 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


