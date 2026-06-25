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
AVT009.lss \

FLASH_IMAGE += \
AVT009.hex \

EEPROM_IMAGE += \
AVT009.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: AVT009.elf secondary-outputs

# Tool invocations
AVT009.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,AVT009.map -mmcu=atmega32 -o "AVT009.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

AVT009.lss: AVT009.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S AVT009.elf  >"AVT009.lss"
	@echo 'Finished building: $@'
	@echo ' '

AVT009.hex: AVT009.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex AVT009.elf  "AVT009.hex"
	@echo 'Finished building: $@'
	@echo ' '

AVT009.eep: AVT009.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex AVT009.elf  "AVT009.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: AVT009.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega32 AVT009.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) AVT009.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents

-include ../makefile.targets
