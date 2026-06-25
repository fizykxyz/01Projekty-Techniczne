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
LCD_Dynamic1.lss \

FLASH_IMAGE += \
LCD_Dynamic1.hex \

EEPROM_IMAGE += \
LCD_Dynamic1.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: LCD_Dynamic1.elf secondary-outputs

# Tool invocations
LCD_Dynamic1.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,LCD_Dynamic1.map -mmcu=atmega162 -o "LCD_Dynamic1.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

LCD_Dynamic1.lss: LCD_Dynamic1.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S LCD_Dynamic1.elf  >"LCD_Dynamic1.lss"
	@echo 'Finished building: $@'
	@echo ' '

LCD_Dynamic1.hex: LCD_Dynamic1.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex LCD_Dynamic1.elf  "LCD_Dynamic1.hex"
	@echo 'Finished building: $@'
	@echo ' '

LCD_Dynamic1.eep: LCD_Dynamic1.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex LCD_Dynamic1.elf  "LCD_Dynamic1.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: LCD_Dynamic1.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega162 LCD_Dynamic1.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) LCD_Dynamic1.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents

-include ../makefile.targets
