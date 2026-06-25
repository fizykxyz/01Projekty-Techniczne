################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/fm3/cc932.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/fm3/ff.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/fm3/iic.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/fm3/main.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/fm3/mmc_mfs.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/fm3/sound.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/fm3/startup_fm3_2.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/fm3/uart_mfs.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/fm3/xprintf.c 

OBJS += \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/fm3/cc932.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/fm3/ff.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/fm3/iic.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/fm3/main.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/fm3/mmc_mfs.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/fm3/sound.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/fm3/startup_fm3_2.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/fm3/uart_mfs.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/fm3/xprintf.o 

C_DEPS += \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/fm3/cc932.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/fm3/ff.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/fm3/iic.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/fm3/main.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/fm3/mmc_mfs.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/fm3/sound.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/fm3/startup_fm3_2.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/fm3/uart_mfs.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/fm3/xprintf.d 


# Each subdirectory must supply rules for building sources it contributes
AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/fm3/%.o: ../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/fm3/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega8 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


