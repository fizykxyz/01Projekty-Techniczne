################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include ONEWIRE/subdir.mk
-include HD44780/subdir.mk
-include ADDS/subdir.mk
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
11_M32_MULTILCD.lss \

FLASH_IMAGE += \
11_M32_MULTILCD.hex \

EEPROM_IMAGE += \
11_M32_MULTILCD.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: 11_M32_MULTILCD.elf secondary-outputs

# Tool invocations
11_M32_MULTILCD.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,11_M32_MULTILCD.map -mmcu=atmega16 -o "11_M32_MULTILCD.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

11_M32_MULTILCD.lss: 11_M32_MULTILCD.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S 11_M32_MULTILCD.elf  >"11_M32_MULTILCD.lss"
	@echo 'Finished building: $@'
	@echo ' '

11_M32_MULTILCD.hex: 11_M32_MULTILCD.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex 11_M32_MULTILCD.elf  "11_M32_MULTILCD.hex"
	@echo 'Finished building: $@'
	@echo ' '

11_M32_MULTILCD.eep: 11_M32_MULTILCD.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex 11_M32_MULTILCD.elf  "11_M32_MULTILCD.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: 11_M32_MULTILCD.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega16 11_M32_MULTILCD.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) 11_M32_MULTILCD.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents

-include ../makefile.targets
