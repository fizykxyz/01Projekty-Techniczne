################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include LCD/subdir.mk
-include 1Wire/subdir.mk
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
05_WIRE1MIREK.lss \

FLASH_IMAGE += \
05_WIRE1MIREK.hex \

EEPROM_IMAGE += \
05_WIRE1MIREK.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: 05_WIRE1MIREK.elf secondary-outputs

# Tool invocations
05_WIRE1MIREK.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,05_WIRE1MIREK.map -Wl,-u,vfprintf -mmcu=atmega32 -o "05_WIRE1MIREK.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

05_WIRE1MIREK.lss: 05_WIRE1MIREK.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S 05_WIRE1MIREK.elf  >"05_WIRE1MIREK.lss"
	@echo 'Finished building: $@'
	@echo ' '

05_WIRE1MIREK.hex: 05_WIRE1MIREK.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex 05_WIRE1MIREK.elf  "05_WIRE1MIREK.hex"
	@echo 'Finished building: $@'
	@echo ' '

05_WIRE1MIREK.eep: 05_WIRE1MIREK.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex 05_WIRE1MIREK.elf  "05_WIRE1MIREK.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: 05_WIRE1MIREK.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega32 05_WIRE1MIREK.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) 05_WIRE1MIREK.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents

-include ../makefile.targets
