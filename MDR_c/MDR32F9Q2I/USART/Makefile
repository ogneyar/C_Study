
GCC_PATH	:= E:\Program\MounRiver\MounRiver_Studio\toolchain\arm-none-eabi-gcc\bin

OPENOCD_INTERFACE := interface/stlink.cfg
OPENOCD_TARGET := target/mdr32f9q2i.cfg


PREFIX = arm-none-eabi-

CC = "$(GCC_PATH)/$(PREFIX)gcc"
AS = "$(GCC_PATH)/$(PREFIX)gcc" -x assembler-with-cpp
CP = "$(GCC_PATH)/$(PREFIX)objcopy"
SZ = "$(GCC_PATH)/$(PREFIX)size"

HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S


# SCRIPT = script.ld
SCRIPT = ../libs/MDR32F9Qx.ld

BUILD_DIR	:= build

TARGET = $(BUILD_DIR)


C_SOURCES = main.c \
	../libs/SPL/MDR32FxQI/src/MDR32FxQI_port.c \
	../libs/SPL/MDR32FxQI/src/MDR32FxQI_rst_clk.c \
	../libs/SPL/MDR32FxQI/src/MDR32FxQI_uart.c #\
	../libs/SPL/MDR32FxQI/src/MDR32FxQI_adc.c \
	../libs/SPL/MDR32FxQI/src/MDR32FxQI_arinc429r.c \
	../libs/SPL/MDR32FxQI/src/MDR32FxQI_arinc429t.c \
	../libs/SPL/MDR32FxQI/src/MDR32FxQI_audio.c \
	../libs/SPL/MDR32FxQI/src/MDR32FxQI_bkp.c \
	../libs/SPL/MDR32FxQI/src/MDR32FxQI_can.c \
	../libs/SPL/MDR32FxQI/src/MDR32FxQI_comp.c \
	../libs/SPL/MDR32FxQI/src/MDR32FxQI_dac.c \
	../libs/SPL/MDR32FxQI/src/MDR32FxQI_dma.c \
	../libs/SPL/MDR32FxQI/src/MDR32FxQI_dsp.c \
	../libs/SPL/MDR32FxQI/src/MDR32FxQI_ebc.c \
	../libs/SPL/MDR32FxQI/src/MDR32FxQI_eeprom.c \
	../libs/SPL/MDR32FxQI/src/MDR32FxQI_eth.c \
	../libs/SPL/MDR32FxQI/src/MDR32FxQI_i2c.c \
	../libs/SPL/MDR32FxQI/src/MDR32FxQI_iwdg.c \
	../libs/SPL/MDR32FxQI/src/MDR32FxQI_mil_std_1553.c \
	../libs/SPL/MDR32FxQI/src/MDR32FxQI_power.c \
	../libs/SPL/MDR32FxQI/src/MDR32FxQI_sdio.c \
	../libs/SPL/MDR32FxQI/src/MDR32FxQI_ssp.c \
	../libs/SPL/MDR32FxQI/src/MDR32FxQI_timer.c \
	../libs/SPL/MDR32FxQI/src/MDR32FxQI_usb.c \
	../libs/SPL/MDR32FxQI/src/MDR32FxQI_wwdg.c

# C includes
C_INCLUDES =  \
	-I. \
	-I../libs \
	-I../libs/SPL/MDR32FxQI \
	-I../libs/CMSIS/MDR32FxQI/CoreSupport/CM3 \
	-I../libs/CMSIS/MDR32FxQI/DeviceSupport/MDR32F9Q2I/startup \
	-I../libs/SPL/MDR32FxQI/inc 

CFLAGS = $(C_INCLUDES) -c -g -Wall -mcpu=cortex-m3 -mthumb -std=gnu99


ASM_SOURCES = #..\libs\CMSIS\MDR32FxQI\DeviceSupport\MDR32F9Q2I\startup\arm\startup_ch32v30x_D8.S


all: flash

build: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))

OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.S=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile
	@if not exist $(BUILD_DIR) (mkdir $(BUILD_DIR))
	$(CC) $(CFLAGS) "$<" -o "$@"

$(BUILD_DIR)/%.o: %.s Makefile
	@if not exist $(BUILD_DIR) (mkdir $(BUILD_DIR))
	$(AS) $(CFLAGS) "$<" -o "$@"

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	@if not exist $(BUILD_DIR) (mkdir $(BUILD_DIR))
	$(CC) $(OBJECTS) -g -T$(SCRIPT) -o $@ -Wl,--print-memory-usage
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf
	@if not exist $(BUILD_DIR) (mkdir $(BUILD_DIR))
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf
	@if not exist $(BUILD_DIR) (mkdir $(BUILD_DIR))
	$(BIN) $< $@	
	

clean:
	@-rmdir /s /q build



flash: build # erase 
#	@openocd -f $(OPENOCD_INTERFACE) -f $(OPENOCD_TARGET) -c init -c halt -c "program $(BUILD_DIR)/$(TARGET).elf" -c "verify_image $(BUILD_DIR)/$(TARGET).elf" -c reset -c exit
	@openocd -f $(OPENOCD_INTERFACE) -f $(OPENOCD_TARGET) -c "program $(BUILD_DIR)/$(TARGET).elf" -c "verify_image $(BUILD_DIR)/$(TARGET).elf" -c exit
#	@openocd -f $(OPENOCD_INTERFACE) -f $(OPENOCD_TARGET) -c init -c halt -c "program $(BUILD_DIR)/$(TARGET).elf" -c exit


erase: build 
	@openocd -f $(OPENOCD_INTERFACE) -f $(OPENOCD_TARGET) -c init -c halt -c "flash erase_sector mdr 0 last" -c reset -c exit




size: 
#	@$(GCC_PATH)\arm-none-eabi-nm –print-size $(OBJ)
	@$(GCC_PATH)\arm-none-eabi-nm --print-size $(ELF)

# запуск для дебага
ocd:
	@openocd -f $(OPENOCD_INTERFACE) -f $(OPENOCD_TARGET)
	
debug:
	@$(GCC_PATH)\arm-none-eabi-gdb $(ELF)

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

