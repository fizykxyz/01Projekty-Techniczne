################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../AND_TECH_C/M32/AND_SD/ff.c \
../AND_TECH_C/M32/AND_SD/main.c \
../AND_TECH_C/M32/AND_SD/mmc.c \
../AND_TECH_C/M32/AND_SD/rtc.c \
../AND_TECH_C/M32/AND_SD/sound.c \
../AND_TECH_C/M32/AND_SD/uart.c 

S_UPPER_SRCS += \
../AND_TECH_C/M32/AND_SD/xitoa.S 

OBJS += \
./AND_TECH_C/M32/AND_SD/ff.o \
./AND_TECH_C/M32/AND_SD/main.o \
./AND_TECH_C/M32/AND_SD/mmc.o \
./AND_TECH_C/M32/AND_SD/rtc.o \
./AND_TECH_C/M32/AND_SD/sound.o \
./AND_TECH_C/M32/AND_SD/uart.o \
./AND_TECH_C/M32/AND_SD/xitoa.o 

S_UPPER_DEPS += \
./AND_TECH_C/M32/AND_SD/xitoa.d 

C_DEPS += \
./AND_TECH_C/M32/AND_SD/ff.d \
./AND_TECH_C/M32/AND_SD/main.d \
./AND_TECH_C/M32/AND_SD/mmc.d \
./AND_TECH_C/M32/AND_SD/rtc.d \
./AND_TECH_C/M32/AND_SD/sound.d \
./AND_TECH_C/M32/AND_SD/uart.d 


# Each subdirectory must supply rules for building sources it contributes
AND_TECH_C/M32/AND_SD/%.o: ../AND_TECH_C/M32/AND_SD/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega8 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

AND_TECH_C/M32/AND_SD/%.o: ../AND_TECH_C/M32/AND_SD/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Assembler'
	avr-gcc -x assembler-with-cpp -mmcu=atmega8 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


