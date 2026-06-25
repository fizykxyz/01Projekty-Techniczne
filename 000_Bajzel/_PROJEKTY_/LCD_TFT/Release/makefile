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
LCD_TFT.lss \

FLASH_IMAGE += \
LCD_TFT.hex \

EEPROM_IMAGE += \
LCD_TFT.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: LCD_TFT.elf secondary-outputs

# Tool invocations
LCD_TFT.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,LCD_TFT.map -Wl,--gc-sections -mmcu=atmega328p -o "LCD_TFT.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

LCD_TFT.lss: LCD_TFT.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S LCD_TFT.elf  >"LCD_TFT.lss"
	@echo 'Finished building: $@'
	@echo ' '

LCD_TFT.hex: LCD_TFT.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex LCD_TFT.elf  "LCD_TFT.hex"
	@echo 'Finished building: $@'
	@echo ' '

LCD_TFT.eep: LCD_TFT.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex LCD_TFT.elf  "LCD_TFT.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: LCD_TFT.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega328p LCD_TFT.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) LCD_TFT.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents

-include ../makefile.targets
