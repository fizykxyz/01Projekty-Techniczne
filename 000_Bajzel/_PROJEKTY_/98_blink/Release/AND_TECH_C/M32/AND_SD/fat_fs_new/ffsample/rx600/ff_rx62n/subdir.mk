################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/rx600/ff_rx62n/cc932.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/rx600/ff_rx62n/dbsct.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/rx600/ff_rx62n/ff.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/rx600/ff_rx62n/intprg.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/rx600/ff_rx62n/main.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/rx600/ff_rx62n/mmc_rspi.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/rx600/ff_rx62n/resetprg.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/rx600/ff_rx62n/scif.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/rx600/ff_rx62n/sound.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/rx600/ff_rx62n/vecttbl.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/rx600/ff_rx62n/xprintf.c 

OBJS += \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/rx600/ff_rx62n/cc932.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/rx600/ff_rx62n/dbsct.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/rx600/ff_rx62n/ff.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/rx600/ff_rx62n/intprg.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/rx600/ff_rx62n/main.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/rx600/ff_rx62n/mmc_rspi.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/rx600/ff_rx62n/resetprg.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/rx600/ff_rx62n/scif.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/rx600/ff_rx62n/sound.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/rx600/ff_rx62n/vecttbl.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/rx600/ff_rx62n/xprintf.o 

C_DEPS += \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/rx600/ff_rx62n/cc932.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/rx600/ff_rx62n/dbsct.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/rx600/ff_rx62n/ff.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/rx600/ff_rx62n/intprg.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/rx600/ff_rx62n/main.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/rx600/ff_rx62n/mmc_rspi.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/rx600/ff_rx62n/resetprg.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/rx600/ff_rx62n/scif.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/rx600/ff_rx62n/sound.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/rx600/ff_rx62n/vecttbl.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/rx600/ff_rx62n/xprintf.d 


# Each subdirectory must supply rules for building sources it contributes
AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/rx600/ff_rx62n/%.o: ../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/rx600/ff_rx62n/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega8 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


