################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/h8/h8_mmc/dbsct.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/h8/h8_mmc/ff.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/h8/h8_mmc/intprg.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/h8/h8_mmc/main.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/h8/h8_mmc/mmc.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/h8/h8_mmc/resetprg.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/h8/h8_mmc/sci.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/h8/h8_mmc/xprintf.c 

OBJS += \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/h8/h8_mmc/dbsct.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/h8/h8_mmc/ff.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/h8/h8_mmc/intprg.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/h8/h8_mmc/main.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/h8/h8_mmc/mmc.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/h8/h8_mmc/resetprg.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/h8/h8_mmc/sci.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/h8/h8_mmc/xprintf.o 

C_DEPS += \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/h8/h8_mmc/dbsct.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/h8/h8_mmc/ff.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/h8/h8_mmc/intprg.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/h8/h8_mmc/main.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/h8/h8_mmc/mmc.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/h8/h8_mmc/resetprg.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/h8/h8_mmc/sci.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/h8/h8_mmc/xprintf.d 


# Each subdirectory must supply rules for building sources it contributes
AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/h8/h8_mmc/%.o: ../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/h8/h8_mmc/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega8 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


