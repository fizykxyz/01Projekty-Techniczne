################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ADDS/memcheck.c \
../ADDS/timers.c \
../ADDS/tools.c 

OBJS += \
./ADDS/memcheck.o \
./ADDS/timers.o \
./ADDS/tools.o 

C_DEPS += \
./ADDS/memcheck.d \
./ADDS/timers.d \
./ADDS/tools.d 


# Each subdirectory must supply rules for building sources it contributes
ADDS/%.o: ../ADDS/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -DMCU=$(AVRTARGETMCU) -mmcu=atmega16 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


