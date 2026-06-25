################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../KS07XX/src/disp/KS07XX.c 

OBJS += \
./KS07XX/src/disp/KS07XX.o 

C_DEPS += \
./KS07XX/src/disp/KS07XX.d 


# Each subdirectory must supply rules for building sources it contributes
KS07XX/src/disp/%.o: ../KS07XX/src/disp/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


