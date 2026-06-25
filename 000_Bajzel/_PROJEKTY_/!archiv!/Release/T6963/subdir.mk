################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../T6963/graphic.c \
../T6963/t6963.c \
../T6963/t6963c.c 

OBJS += \
./T6963/graphic.o \
./T6963/t6963.o \
./T6963/t6963c.o 

C_DEPS += \
./T6963/graphic.d \
./T6963/t6963.d \
./T6963/t6963c.d 


# Each subdirectory must supply rules for building sources it contributes
T6963/%.o: ../T6963/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -DMCU=$(AVRTARGETMCU) -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


