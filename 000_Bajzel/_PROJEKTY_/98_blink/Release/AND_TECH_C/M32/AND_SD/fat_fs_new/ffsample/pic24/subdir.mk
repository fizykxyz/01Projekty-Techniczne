################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/pic24/ccsbcs.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/pic24/ff.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/pic24/main.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/pic24/mmc.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/pic24/uart.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/pic24/xprintf.c 

OBJS += \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/pic24/ccsbcs.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/pic24/ff.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/pic24/main.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/pic24/mmc.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/pic24/uart.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/pic24/xprintf.o 

C_DEPS += \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/pic24/ccsbcs.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/pic24/ff.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/pic24/main.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/pic24/mmc.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/pic24/uart.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/pic24/xprintf.d 


# Each subdirectory must supply rules for building sources it contributes
AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/pic24/%.o: ../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/pic24/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega8 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


