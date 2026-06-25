################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ff9b/src/diskio.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ff9b/src/ff.c 

OBJS += \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ff9b/src/diskio.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ff9b/src/ff.o 

C_DEPS += \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ff9b/src/diskio.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ff9b/src/ff.d 


# Each subdirectory must supply rules for building sources it contributes
AND_TECH_C/M32/AND_SD/fat_fs_new/ff9b/src/%.o: ../AND_TECH_C/M32/AND_SD/fat_fs_new/ff9b/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega8 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


