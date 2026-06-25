################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../AND_TECH_C/M32/AND_RFM12B/RFM12B.c \
../AND_TECH_C/M32/AND_RFM12B/main.c \
../AND_TECH_C/M32/AND_RFM12B/uart.c 

OBJS += \
./AND_TECH_C/M32/AND_RFM12B/RFM12B.o \
./AND_TECH_C/M32/AND_RFM12B/main.o \
./AND_TECH_C/M32/AND_RFM12B/uart.o 

C_DEPS += \
./AND_TECH_C/M32/AND_RFM12B/RFM12B.d \
./AND_TECH_C/M32/AND_RFM12B/main.d \
./AND_TECH_C/M32/AND_RFM12B/uart.d 


# Each subdirectory must supply rules for building sources it contributes
AND_TECH_C/M32/AND_RFM12B/%.o: ../AND_TECH_C/M32/AND_RFM12B/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega8 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


