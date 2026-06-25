################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include vOLED/subdir.mk
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
99_vOLED.lss \

FLASH_IMAGE += \
99_vOLED.hex \

EEPROM_IMAGE += \
99_vOLED.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: 99_vOLED.elf secondary-outputs

# Tool invocations
99_vOLED.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,99_vOLED.map -mmcu=atmega16 -o "99_vOLED.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

99_vOLED.lss: 99_vOLED.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S 99_vOLED.elf  >"99_vOLED.lss"
	@echo 'Finished building: $@'
	@echo ' '

99_vOLED.hex: 99_vOLED.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex 99_vOLED.elf  "99_vOLED.hex"
	@echo 'Finished building: $@'
	@echo ' '

99_vOLED.eep: 99_vOLED.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex 99_vOLED.elf  "99_vOLED.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: 99_vOLED.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega16 99_vOLED.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) 99_vOLED.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents

-include ../makefile.targets
