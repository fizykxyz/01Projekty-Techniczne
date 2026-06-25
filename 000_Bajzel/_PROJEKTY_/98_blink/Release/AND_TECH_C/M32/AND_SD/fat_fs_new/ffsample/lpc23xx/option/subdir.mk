################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc23xx/option/cc932.c 

O_SRCS += \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc23xx/option/cc932.o 

OBJS += \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc23xx/option/cc932.o 

C_DEPS += \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc23xx/option/cc932.d 


# Each subdirectory must supply rules for building sources it contributes
AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc23xx/option/%.o: ../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc23xx/option/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega8 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


