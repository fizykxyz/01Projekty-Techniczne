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
98_M328_KS0108.lss \

FLASH_IMAGE += \
98_M328_KS0108.hex \

EEPROM_IMAGE += \
98_M328_KS0108.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: 98_M328_KS0108.elf secondary-outputs

# Tool invocations
98_M328_KS0108.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,98_M328_KS0108.map -mmcu=atmega328p -o "98_M328_KS0108.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

98_M328_KS0108.lss: 98_M328_KS0108.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S 98_M328_KS0108.elf  >"98_M328_KS0108.lss"
	@echo 'Finished building: $@'
	@echo ' '

98_M328_KS0108.hex: 98_M328_KS0108.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex 98_M328_KS0108.elf  "98_M328_KS0108.hex"
	@echo 'Finished building: $@'
	@echo ' '

98_M328_KS0108.eep: 98_M328_KS0108.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex 98_M328_KS0108.elf  "98_M328_KS0108.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: 98_M328_KS0108.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega328p 98_M328_KS0108.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) 98_M328_KS0108.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents

-include ../makefile.targets
