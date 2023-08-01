#--------------------------------------------------------------------------------------------------
#--------------------------------------------- GCC ------------------------------------------------
#--------------------------------------------------------------------------------------------------

-include config.mk

REAL_PATH := $(realpath .)

MAP := $(OUTPUT_NAME).map
ELF := $(OUTPUT_NAME).elf
LSS := $(OUTPUT_NAME).lss
HEX := $(FIRMWARE_NAME)$(VERSION).hex

NIL :=
SPACE := $(NIL) $(NIL)

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))
C_SRC := $(call rwildcard, $(SOURCE), *.c)
S_SRC := $(call rwildcard, $(SOURCE), *.S)
C_OBJ := $(notdir $(C_SRC:.c=.o))
S_OBJ := $(notdir $(S_SRC:.S=.o))
VPATH := $(subst $(SPACE),:,$(dir $(C_SRC)) $(dir $(S_SRC)))

#--------------------------------------------------------------------------------------------------

GCC_CFLAGS := \
$(addprefix -I, $(INCLUDE)) -mcpu=$(TARGET_MCU) -mthumb -O1 -g \
-ffunction-sections -fdata-sections \
-Wall -Wextra -Wshadow -Wredundant-decls -Wno-unused-parameter \
-Wno-missing-field-initializers -Wno-main \
-pipe $(GCC_DEF)

GCC_LFLAGS := \
-T $(LSCRIPT) \
-ggdb \
--specs=nano.specs \
--specs=nosys.specs \
-Wl,--gc-sections -ffreestanding -nostartfiles \
-Wl,-Map=$(MAP) 

#--------------------------------------------------------------------------------------------------

all: $(HEX)
	@rm $(C_OBJ) $(S_OBJ) $(MAP)
	@echo Building done

clean: 
	@echo Cleaning
	@rm $(ELF) $(LSS) $(HEX)

%.o: %.c makefile
	@echo -e Compile: $@ '\t\t' $<
	@arm-none-eabi-gcc $(GCC_CFLAGS) -c -o $@ $<
	
%.o: %.S makefile
	@echo -e Compile: $@ '\t' $<
	@arm-none-eabi-gcc $(GCC_CFLAGS) -c -o $@ $<

$(ELF): $(C_OBJ) $(S_OBJ)
	@echo -e Linking: $@ 
	@arm-none-eabi-gcc $(GCC_CFLAGS) $(GCC_LFLAGS) $(C_OBJ) $(S_OBJ) -o $(ELF) 
	
$(LSS): $(ELF)
	@echo -e Generate $@
	@arm-none-eabi-objdump -h -S $(ELF) > $(LSS)

$(HEX): $(ELF) $(LSS)
	@echo 
	@arm-none-eabi-size $<
	@echo -e '\n'Generate firmware: $@
	@arm-none-eabi-objcopy -O ihex $< $@ 
	
	
#--------------------------------------------------------------------------------------------------
#------------------------------------------- OpenOCD ----------------------------------------------
#--------------------------------------------------------------------------------------------------

OCD_SHUTDOWN := \
-c 'shutdown'

OCD_PROGRAM := \
-c 'init' \
$(OCD_RESET_CMD) \
-c 'reset halt' \
-c 'wait_halt' \
-c 'flash write_image erase $(REAL_PATH)/$(ELF)' \
-c 'flash verify_image $(REAL_PATH)/$(ELF)' \
-c 'reset' \
$(OCD_RESET_CMD)

OCD_ERASE := \
-c 'init' \
-c 'reset halt' \
-c 'flash erase_sector 0 0 31' \
-c 'flash erase_check 0' \
-c 'reset' \

OCD_READ := \
-c 'init' \
-c 'reset' \
-c 'halt' \
-c 'flash read_bank 0 test.bin 0 0x20000' \
-c 'reset' \

#--------------------------------------------------------------------------------------------------
	
OCD_DBG := \
-c 'bindto 0.0.0.0' \
-c 'debug_level 0' \
$(OCD_PROGRAM) \
-c 'reset halt' 


prog: all
	@openocd -f $(OCD_INTERFACE) -f $(OCD_TARGET) $(OCD_PROGRAM) $(OCD_SHUTDOWN)

erase:
	@openocd -f $(OCD_INTERFACE) -f $(OCD_TARGET) $(OCD_ERASE) $(OCD_SHUTDOWN)
	
read:
	@openocd -f $(OCD_INTERFACE) -f $(OCD_TARGET) $(OCD_READ) $(OCD_SHUTDOWN)

ocd:
	@openocd -f $(OCD_INTERFACE) -f $(OCD_TARGET) $(OCD_DBG)

#--------------------------------------------------------------------------------------------------

GDB_DBG := \
-ex 'target remote localhost:3333' \
-ex 'break main' \
-ex 'continue' 
#-ex 'layout src' 

debug:
	@openocd -f $(OCD_INTERFACE) -f $(OCD_TARGET) $(OCD_DBG) &
	@arm-none-eabi-gdb $(GDB_DBG) $(ELF)
	@killall openocd
	@echo "\033[33mDebuging closed\033[37m\n"
	
