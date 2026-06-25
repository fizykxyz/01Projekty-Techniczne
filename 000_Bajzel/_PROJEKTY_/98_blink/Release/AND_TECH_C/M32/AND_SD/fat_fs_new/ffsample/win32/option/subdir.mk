################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/win32/option/cc932.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/win32/option/cc936.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/win32/option/cc949.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/win32/option/cc950.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/win32/option/ccsbcs.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/win32/option/syscall.c \
../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/win32/option/unicode.c 

OBJS += \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/win32/option/cc932.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/win32/option/cc936.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/win32/option/cc949.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/win32/option/cc950.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/win32/option/ccsbcs.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/win32/option/syscall.o \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/win32/option/unicode.o 

C_DEPS += \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/win32/option/cc932.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/win32/option/cc936.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/win32/option/cc949.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/win32/option/cc950.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/win32/option/ccsbcs.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/win32/option/syscall.d \
./AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/win32/option/unicode.d 


# Each subdirectory must supply rules for building sources it contributes
AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/win32/option/%.o: ../AND_TECH_C/M32/AND_SD/fat_fs_new/ffsample/win32/option/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega8 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


