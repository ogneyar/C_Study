
OPT = -Os

BUILD_DIR = build

TARGET = $(BUILD_DIR)

C_SOURCES =  \
	main.c 

ASM_SOURCES = startup_ch32v20x_D6.S

GCC_PATH = E:\Program\MounRiver/MounRiver_Studio/toolchain/RISC-V Embedded GCC/bin
OCD_PATH = E:\Program\MounRiver/MounRiver_Studio/toolchain/OpenOCD/bin

OCD = "$(OCD_PATH)/openocd"

PREFIX = riscv-none-embed-

CC = "$(GCC_PATH)/$(PREFIX)gcc"
AS = "$(GCC_PATH)/$(PREFIX)gcc" -x assembler-with-cpp
CP = "$(GCC_PATH)/$(PREFIX)objcopy"
SZ = "$(GCC_PATH)/$(PREFIX)size"

HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
 
CPU = -march=rv32ecxw

MCU = $(CPU) -mabi=ilp32e -msmall-data-limit=0 -msave-restore -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized

AS_DEFS = 

C_DEFS =  

AS_INCLUDES = 

C_INCLUDES =  \
-I. 

ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT)

CFLAGS += $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT)

CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

LDSCRIPT = ch32v20x_map.ld

LIBS = 
LIBDIR = 
LDFLAGS = $(MCU) -g --specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -nostartfiles -Xlinker \
	--gc-sections -Wl,-Map=$(BUILD_DIR)/$(TARGET).map --specs=nosys.specs $(OPT)

all: flash

build: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.S=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile
	@if not exist $(BUILD_DIR) (mkdir $(BUILD_DIR))
	$(CC) -c $(CFLAGS) -g -std=gnu99 -MT"$(@)" -c "$<" -o "$@"

$(BUILD_DIR)/%.o: %.s Makefile
	@if not exist $(BUILD_DIR) (mkdir $(BUILD_DIR))
	$(AS) -c $(CFLAGS) -g -MT"$(@)" -c "$<" -o "$@"

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	@if not exist $(BUILD_DIR) (mkdir $(BUILD_DIR))
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf
	@if not exist $(BUILD_DIR) (mkdir $(BUILD_DIR))
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf
	@if not exist $(BUILD_DIR) (mkdir $(BUILD_DIR))
	$(BIN) $< $@	
	
$(BUILD_DIR):
	@if not exist $(BUILD_DIR) (mkdir $(BUILD_DIR))	

clean:
#	-rm -fR $(BUILD_DIR)
	@-rmdir /s /q build

OPENOCD_CFG = ../libs/Util/wch-riscv.cfg

erase: build
	$(OCD) -f $(OPENOCD_CFG) -c init -c halt -c "flash erase_sector wch_riscv 0 last" -c exit

flash: erase 
	$(OCD) -f $(OPENOCD_CFG) -c init -c halt  -c "program $(BUILD_DIR)/$(TARGET).hex" -c wlink_reset_resume -c exit

reset: #flash
	$(OCD) -f $(OPENOCD_CFG) -c init -c reset -c resume -c exit


