################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc176x/crp.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc176x/ff.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc176x/main.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc176x/mmc_ssp.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc176x/rtc.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc176x/sound.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc176x/startup176x.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc176x/startup178x.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc176x/uart176x.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc176x/uart178x.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc176x/xprintf.c 

OBJS += \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc176x/crp.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc176x/ff.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc176x/main.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc176x/mmc_ssp.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc176x/rtc.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc176x/sound.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc176x/startup176x.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc176x/startup178x.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc176x/uart176x.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc176x/uart178x.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc176x/xprintf.o 

C_DEPS += \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc176x/crp.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc176x/ff.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc176x/main.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc176x/mmc_ssp.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc176x/rtc.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc176x/sound.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc176x/startup176x.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc176x/startup178x.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc176x/uart176x.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc176x/uart178x.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc176x/xprintf.d 


# Each subdirectory must supply rules for building sources it contributes
AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc176x/%.o: ../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc176x/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega8 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


