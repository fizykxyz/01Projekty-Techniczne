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
14_M32_NOKIA_FBUS.lss \

FLASH_IMAGE += \
14_M32_NOKIA_FBUS.hex \

EEPROM_IMAGE += \
14_M32_NOKIA_FBUS.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: 14_M32_NOKIA_FBUS.elf secondary-outputs

# Tool invocations
14_M32_NOKIA_FBUS.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,14_M32_NOKIA_FBUS.map -mmcu=atmega32 -o "14_M32_NOKIA_FBUS.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

14_M32_NOKIA_FBUS.lss: 14_M32_NOKIA_FBUS.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S 14_M32_NOKIA_FBUS.elf  >"14_M32_NOKIA_FBUS.lss"
	@echo 'Finished building: $@'
	@echo ' '

14_M32_NOKIA_FBUS.hex: 14_M32_NOKIA_FBUS.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex 14_M32_NOKIA_FBUS.elf  "14_M32_NOKIA_FBUS.hex"
	@echo 'Finished building: $@'
	@echo ' '

14_M32_NOKIA_FBUS.eep: 14_M32_NOKIA_FBUS.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex 14_M32_NOKIA_FBUS.elf  "14_M32_NOKIA_FBUS.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: 14_M32_NOKIA_FBUS.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega32 14_M32_NOKIA_FBUS.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) 14_M32_NOKIA_FBUS.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents

-include ../makefile.targets
