################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc23xx/crp.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc23xx/diskio.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc23xx/disp.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc23xx/ff.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc23xx/filer.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc23xx/interrupt.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc23xx/main.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc23xx/mci.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc23xx/nand.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc23xx/rtc23xx.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc23xx/sound.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc23xx/tjpgd.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc23xx/uart23xx.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc23xx/xprintf.c 

S_UPPER_SRCS += \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc23xx/asmfunc.S 

OBJS += \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc23xx/asmfunc.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc23xx/crp.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc23xx/diskio.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc23xx/disp.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc23xx/ff.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc23xx/filer.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc23xx/interrupt.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc23xx/main.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc23xx/mci.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc23xx/nand.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc23xx/rtc23xx.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc23xx/sound.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc23xx/tjpgd.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc23xx/uart23xx.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc23xx/xprintf.o 

S_UPPER_DEPS += \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc23xx/asmfunc.d 

C_DEPS += \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc23xx/crp.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc23xx/diskio.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc23xx/disp.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc23xx/ff.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc23xx/filer.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc23xx/interrupt.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc23xx/main.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc23xx/mci.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc23xx/nand.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc23xx/rtc23xx.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc23xx/sound.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc23xx/tjpgd.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc23xx/uart23xx.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc23xx/xprintf.d 


# Each subdirectory must supply rules for building sources it contributes
AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc23xx/%.o: ../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc23xx/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Assembler'
	avr-gcc -x assembler-with-cpp -mmcu=atmega8 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc23xx/%.o: ../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/lpc23xx/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega8 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


