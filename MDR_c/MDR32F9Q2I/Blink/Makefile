
# TOOL_CHAIN	:= E:/Program/arm-gnu-toolchain/bin
TOOL_CHAIN	:= E:\Program\MounRiver\MounRiver_Studio\toolchain\arm-none-eabi-gcc\bin
OPENOCD_INTERFACE := interface/stlink.cfg
OPENOCD_TARGET := target/mdr32f9q2i.cfg

SCRIPT		:= script.ld
# SCRIPT		:= ../libs/MDR32F9Qx.ld

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
	-I../libs \
	-I../libs/SPL/MDR32FxQI \
	-I../libs/CMSIS/MDR32FxQI/CoreSupport/CM3 \
	-I../libs/CMSIS/MDR32FxQI/DeviceSupport/MDR32F9Q2I/startup

C_FLAGS		:= $(C_INCLUDES) -c -g -Wall -mcpu=cortex-m3 -mthumb -std=gnu99

all: flash
	@echo finish All

flash: obj
	@openocd -f $(OPENOCD_INTERFACE) -f $(OPENOCD_TARGET) -c "program $(ELF)" -c "verify_image $(ELF)" -c reset -c exit

obj: link
	@$(TOOL_CHAIN)\arm-none-eabi-objcopy.exe -O binary $(ELF) $(BIN)
	@$(TOOL_CHAIN)\arm-none-eabi-objcopy.exe -O ihex $(ELF) $(HEX)

link: compile
	@$(TOOL_CHAIN)\arm-none-eabi-ld -g -T$(SCRIPT) -o $(ELF) $(OBJ) --print-memory-usage
    
compile:
	@if not exist $(BUILD_DIR) (mkdir $(BUILD_DIR))
	@$(TOOL_CHAIN)\arm-none-eabi-gcc.exe $(C_FLAGS) -o $(OBJ) $(CFILE)

size: 
#	@$(TOOL_CHAIN)\arm-none-eabi-nm –print-size $(OBJ)
	@$(TOOL_CHAIN)\arm-none-eabi-nm --print-size $(ELF)

# запуск для дебага
ocd:
	@openocd -f $(OPENOCD_INTERFACE) -f $(OPENOCD_TARGET)
	
debug:
	@$(TOOL_CHAIN)\arm-none-eabi-gdb $(ELF)

# target remote 127.0.0.1:3333
# monitor halt — остановка чипа
# load - загрузка файла Test.elf
# monitor reset init - инициализировать чип (сбросить регистры тд)
# x/20wux 0x800000 - показать дамп памяти по адресу 0x8000000, 20 слов (w) беззнаково (u) в HEX (x)
# x/20wux 0x00 - дамп области начала памяти
# p/x *(uint32_t*) 0x04 - команда p – печать переменной, через дробь параметр x – отобразить в HEX, а дальше обычный синтаксис C
# p/x &Reset_Handler
# step - выполнение до следующей строки исходника
# stepi - выполнение до следующей машинной инструкции
# continue - запуск программы пока она не будет прервана вручную или точной останова

