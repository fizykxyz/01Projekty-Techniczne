################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/sh7262/fftest/cc932.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/sh7262/fftest/ff.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/sh7262/fftest/main.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/sh7262/fftest/mmc.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/sh7262/fftest/resetprg.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/sh7262/fftest/scif.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/sh7262/fftest/vecttbl.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/sh7262/fftest/xprintf.c 

OBJS += \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/sh7262/fftest/cc932.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/sh7262/fftest/ff.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/sh7262/fftest/main.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/sh7262/fftest/mmc.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/sh7262/fftest/resetprg.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/sh7262/fftest/scif.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/sh7262/fftest/vecttbl.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/sh7262/fftest/xprintf.o 

C_DEPS += \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/sh7262/fftest/cc932.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/sh7262/fftest/ff.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/sh7262/fftest/main.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/sh7262/fftest/mmc.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/sh7262/fftest/resetprg.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/sh7262/fftest/scif.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/sh7262/fftest/vecttbl.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/sh7262/fftest/xprintf.d 


# Each subdirectory must supply rules for building sources it contributes
AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/sh7262/fftest/%.o: ../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/sh7262/fftest/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega8 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


