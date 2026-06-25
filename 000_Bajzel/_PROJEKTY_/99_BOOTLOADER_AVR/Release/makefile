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
99_BOOTLOADER_AVR.lss \

FLASH_IMAGE += \
99_BOOTLOADER_AVR.hex \

EEPROM_IMAGE += \
99_BOOTLOADER_AVR.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: 99_BOOTLOADER_AVR.elf secondary-outputs

# Tool invocations
99_BOOTLOADER_AVR.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,99_BOOTLOADER_AVR.map -mmcu=atmega32 -o "99_BOOTLOADER_AVR.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

99_BOOTLOADER_AVR.lss: 99_BOOTLOADER_AVR.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S 99_BOOTLOADER_AVR.elf  >"99_BOOTLOADER_AVR.lss"
	@echo 'Finished building: $@'
	@echo ' '

99_BOOTLOADER_AVR.hex: 99_BOOTLOADER_AVR.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex 99_BOOTLOADER_AVR.elf  "99_BOOTLOADER_AVR.hex"
	@echo 'Finished building: $@'
	@echo ' '

99_BOOTLOADER_AVR.eep: 99_BOOTLOADER_AVR.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex 99_BOOTLOADER_AVR.elf  "99_BOOTLOADER_AVR.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: 99_BOOTLOADER_AVR.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega32 99_BOOTLOADER_AVR.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) 99_BOOTLOADER_AVR.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents

-include ../makefile.targets
