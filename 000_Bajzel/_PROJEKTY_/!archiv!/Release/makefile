################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
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
00_XYZ_AVRLIBS_2020.lss \

FLASH_IMAGE += \
00_XYZ_AVRLIBS_2020.hex \

EEPROM_IMAGE += \
00_XYZ_AVRLIBS_2020.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: 00_XYZ_AVRLIBS_2020.elf secondary-outputs

# Tool invocations
00_XYZ_AVRLIBS_2020.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,00_XYZ_AVRLIBS_2020.map -mmcu=atmega32 -o "00_XYZ_AVRLIBS_2020.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

00_XYZ_AVRLIBS_2020.lss: 00_XYZ_AVRLIBS_2020.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S 00_XYZ_AVRLIBS_2020.elf  >"00_XYZ_AVRLIBS_2020.lss"
	@echo 'Finished building: $@'
	@echo ' '

00_XYZ_AVRLIBS_2020.hex: 00_XYZ_AVRLIBS_2020.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex 00_XYZ_AVRLIBS_2020.elf  "00_XYZ_AVRLIBS_2020.hex"
	@echo 'Finished building: $@'
	@echo ' '

00_XYZ_AVRLIBS_2020.eep: 00_XYZ_AVRLIBS_2020.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex 00_XYZ_AVRLIBS_2020.elf  "00_XYZ_AVRLIBS_2020.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: 00_XYZ_AVRLIBS_2020.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega32 00_XYZ_AVRLIBS_2020.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) 00_XYZ_AVRLIBS_2020.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents

-include ../makefile.targets
