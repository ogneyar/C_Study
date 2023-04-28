
PREFIX		:= arm-none-eabi-
TOOL_CHAIN	:= E:/Program/MounRiver/MounRiver_Studio/toolchain/arm-none-eabi-gcc/bin/$(PREFIX)
OPENOCD_CFG	:= ../libs/openocd.cfg
MCPU		:= cortex-m0plus

SCRIPT		:= ../libs/samd21g18a_flash.ld

BUILD_DIR	:= build

FILE_NAME	:= Blink
CFILE		:= $(FILE_NAME).c
OBJ			:= $(BUILD_DIR)/$(FILE_NAME).o
ELF			:= $(BUILD_DIR)/$(FILE_NAME).elf
HEX			:= $(BUILD_DIR)/$(FILE_NAME).hex
BIN			:= $(BUILD_DIR)/$(FILE_NAME).bin

# C includes
C_INCLUDES =  \
	-I. \
	-I ../libs \
	-I ../libs/samd21 \
	-I ../libs/samd21/source \
	-I ../libs/samd21/source/as_gcc \
	-I ../libs/samd21/include

all: flash
	@echo finish All

flash: obj
	@bossac.exe -i -d --port=COM29 -i -e -w -v $(BIN) -R
	@echo bossac finish

obj: link
	@$(TOOL_CHAIN)objcopy.exe -O binary $(ELF) $(BIN)
	@$(TOOL_CHAIN)objcopy.exe -O ihex $(ELF) $(HEX)
	@echo objcopy finish

link: compile
	@$(TOOL_CHAIN)ld -g -T$(SCRIPT) -o $(ELF) $(OBJ)
	@echo ld finish
    
compile:
	@if not exist $(BUILD_DIR) (mkdir $(BUILD_DIR))
	@$(TOOL_CHAIN)gcc.exe -c -g -Wall -mcpu=$(MCPU) -mthumb -std=gnu99 -o $(OBJ) $(CFILE) $(C_INCLUDES)
	@echo gcc finish



size: 
	@$(TOOL_CHAIN)nm --print-size $(ELF)

clean:
	rm -f -- *.bin *.elf *.o *.lst

upload: # через программатор
	@openocd -d2 -f openocd.cfg -c "program $(ELF)" -c "verify_image $(ELF)" -c reset -c exit
