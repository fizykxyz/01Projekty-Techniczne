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
98_blink.lss \

FLASH_IMAGE += \
98_blink.hex \

EEPROM_IMAGE += \
98_blink.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: 98_blink.elf secondary-outputs

# Tool invocations
98_blink.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,98_blink.map -mmcu=atmega32 -o "98_blink.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

98_blink.lss: 98_blink.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S 98_blink.elf  >"98_blink.lss"
	@echo 'Finished building: $@'
	@echo ' '

98_blink.hex: 98_blink.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex 98_blink.elf  "98_blink.hex"
	@echo 'Finished building: $@'
	@echo ' '

98_blink.eep: 98_blink.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex 98_blink.elf  "98_blink.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: 98_blink.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega32 98_blink.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) 98_blink.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents

-include ../makefile.targets
