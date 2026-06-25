################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
10_ATMEGA328P_BLINK.lss \

FLASH_IMAGE += \
10_ATMEGA328P_BLINK.hex \

EEPROM_IMAGE += \
10_ATMEGA328P_BLINK.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: 10_ATMEGA328P_BLINK.elf secondary-outputs

# Tool invocations
10_ATMEGA328P_BLINK.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,10_ATMEGA328P_BLINK.map -mmcu=atmega328p -o "10_ATMEGA328P_BLINK.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

10_ATMEGA328P_BLINK.lss: 10_ATMEGA328P_BLINK.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S 10_ATMEGA328P_BLINK.elf  >"10_ATMEGA328P_BLINK.lss"
	@echo 'Finished building: $@'
	@echo ' '

10_ATMEGA328P_BLINK.hex: 10_ATMEGA328P_BLINK.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex 10_ATMEGA328P_BLINK.elf  "10_ATMEGA328P_BLINK.hex"
	@echo 'Finished building: $@'
	@echo ' '

10_ATMEGA328P_BLINK.eep: 10_ATMEGA328P_BLINK.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex 10_ATMEGA328P_BLINK.elf  "10_ATMEGA328P_BLINK.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: 10_ATMEGA328P_BLINK.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega328p 10_ATMEGA328P_BLINK.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(ASM_DEPS)$(EEPROM_IMAGE)$(FLASH_IMAGE)$(ELFS)$(LSS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS) 10_ATMEGA328P_BLINK.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
