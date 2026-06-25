################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/avr/ata.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/avr/cfc.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/avr/cfmm.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/avr/ff.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/avr/main.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/avr/mmc.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/avr/mmcbb.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/avr/rtc.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/avr/sound.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/avr/uart.c 

S_UPPER_SRCS += \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/avr/xitoa.S 

OBJS += \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/avr/ata.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/avr/cfc.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/avr/cfmm.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/avr/ff.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/avr/main.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/avr/mmc.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/avr/mmcbb.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/avr/rtc.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/avr/sound.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/avr/uart.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/avr/xitoa.o 

S_UPPER_DEPS += \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/avr/xitoa.d 

C_DEPS += \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/avr/ata.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/avr/cfc.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/avr/cfmm.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/avr/ff.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/avr/main.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/avr/mmc.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/avr/mmcbb.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/avr/rtc.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/avr/sound.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/avr/uart.d 


# Each subdirectory must supply rules for building sources it contributes
AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/avr/%.o: ../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/avr/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega8 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/avr/%.o: ../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/avr/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Assembler'
	avr-gcc -x assembler-with-cpp -mmcu=atmega8 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


