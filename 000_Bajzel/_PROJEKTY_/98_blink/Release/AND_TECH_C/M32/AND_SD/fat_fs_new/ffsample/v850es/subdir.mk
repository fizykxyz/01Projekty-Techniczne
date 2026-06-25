################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/v850es/startup.s 

C_SRCS += \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/v850es/cc932.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/v850es/ff.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/v850es/main.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/v850es/mmc.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/v850es/uart.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/v850es/xprintf.c 

OBJS += \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/v850es/cc932.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/v850es/ff.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/v850es/main.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/v850es/mmc.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/v850es/startup.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/v850es/uart.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/v850es/xprintf.o 

S_DEPS += \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/v850es/startup.d 

C_DEPS += \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/v850es/cc932.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/v850es/ff.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/v850es/main.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/v850es/mmc.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/v850es/uart.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/v850es/xprintf.d 


# Each subdirectory must supply rules for building sources it contributes
AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/v850es/%.o: ../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/v850es/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega8 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/v850es/%.o: ../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/v850es/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Assembler'
	avr-gcc -x assembler-with-cpp -mmcu=atmega8 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


