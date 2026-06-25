################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=
LIB_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS := 


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../ff.c \
../main.c \
../mmc.c \
../uart.c


PREPROCESSING_SRCS +=  \
../xitoa.S


ASM_SRCS += 


OBJS +=  \
ff.o \
main.o \
mmc.o \
uart.o \
xitoa.o

OBJS_AS_ARGS +=  \
ff.o \
main.o \
mmc.o \
uart.o \
xitoa.o

C_DEPS +=  \
ff.d \
main.d \
mmc.d \
uart.d \
xitoa.d

C_DEPS_AS_ARGS +=  \
ff.d \
main.d \
mmc.d \
uart.d \
xitoa.d

OUTPUT_FILE_PATH +=SD_fatfs_new.elf

OUTPUT_FILE_PATH_AS_ARGS +=SD_fatfs_new.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

# AVR32/GNU C Compiler









./%.o: .././%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 20100110.0
	$(QUOTE)C:\WinAVR\bin\avr-gcc.exe$(QUOTE)  -funsigned-char -funsigned-bitfields -DF_CPU=16000000UL -DMEDIA="MMC"  -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -mmcu=atmega32 -c -gdwarf-2 -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<"
	@echo Finished building: $<
	



# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler
./xitoa.o: .././xitoa.S
	@echo Building file: $<
	@echo Invoking: AVR32/GNU Assembler : 20100110.0
	$(QUOTE)C:\WinAVR\bin\avr-gcc.exe$(QUOTE) -Wa,-gdwarf2 -x assembler-with-cpp -c -mmcu=atmega32 -Wall -gdwarf-2 -std=gnu99 -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums  -DF_CPU=16000000UL  -DMEDIA="MMC" -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<"
	@echo Finished building: $<
	


./%.o: .././%.S
	@echo Building file: $<
	@echo Invoking: AVR32/GNU Assembler : 20100110.0
	$(QUOTE)C:\WinAVR\bin\avr-gcc.exe$(QUOTE) -Wa,-gdwarf2 -x assembler-with-cpp -c -mmcu=atmega32 -Wall -gdwarf-2 -std=gnu99 -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums  -DF_CPU=16000000UL  -DMEDIA="MMC" -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<"
	@echo Finished building: $<
	




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP) $(LIB_DEP)
	@echo Building target: $@
	@echo Invoking: AVR/GNU Linker : 20100110.0
	$(QUOTE)C:\WinAVR\bin\avr-gcc.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="SD_fatfs_new.map" -Wl,--start-group  -Wl,--end-group -Wl,--gc-sections -mmcu=atmega32 
	@echo Finished building target: $@
	"C:\WinAVR\bin\avr-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature  "SD_fatfs_new.elf" "SD_fatfs_new.hex"
	"C:\WinAVR\bin\avr-objcopy.exe" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "SD_fatfs_new.elf" "SD_fatfs_new.eep" || exit 0
	"C:\WinAVR\bin\avr-objdump.exe" -h -S "SD_fatfs_new.elf" > "SD_fatfs_new.lss"
	"C:\WinAVR\bin\avr-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature  "SD_fatfs_new.elf" "SD_fatfs_new.srec"
	"C:\WinAVR\bin\avr-size.exe" "SD_fatfs_new.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "SD_fatfs_new.elf" "SD_fatfs_new.a" "SD_fatfs_new.hex" "SD_fatfs_new.lss" "SD_fatfs_new.eep" "SD_fatfs_new.map" "SD_fatfs_new.srec"
	