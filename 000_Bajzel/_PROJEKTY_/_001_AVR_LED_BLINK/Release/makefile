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
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
_001_AVR_LED_BLINK.lss \

FLASH_IMAGE += \
_001_AVR_LED_BLINK.hex \

EEPROM_IMAGE += \
_001_AVR_LED_BLINK.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: _001_AVR_LED_BLINK.elf secondary-outputs

# Tool invocations
_001_AVR_LED_BLINK.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,_001_AVR_LED_BLINK.map -mmcu=atmega8 -o "_001_AVR_LED_BLINK.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

_001_AVR_LED_BLINK.lss: _001_AVR_LED_BLINK.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S _001_AVR_LED_BLINK.elf  >"_001_AVR_LED_BLINK.lss"
	@echo 'Finished building: $@'
	@echo ' '

_001_AVR_LED_BLINK.hex: _001_AVR_LED_BLINK.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex _001_AVR_LED_BLINK.elf  "_001_AVR_LED_BLINK.hex"
	@echo 'Finished building: $@'
	@echo ' '

_001_AVR_LED_BLINK.eep: _001_AVR_LED_BLINK.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex _001_AVR_LED_BLINK.elf  "_001_AVR_LED_BLINK.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: _001_AVR_LED_BLINK.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega8 _001_AVR_LED_BLINK.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) _001_AVR_LED_BLINK.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents

-include ../makefile.targets
