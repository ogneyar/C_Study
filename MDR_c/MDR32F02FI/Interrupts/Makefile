
TARGET = firmware

# OPT = -Os
OPT = -O0

BUILD_DIR = build

C_SOURCES =  \
	src/main.c \
	src/init.c \
	src/syscalls.c \
	../libs/SPL/src/MLDR187_bkp.c \
	../libs/SPL/src/MLDR187_gpio.c \
	../libs/SPL/src/MLDR187_rst_clk.c \
	../libs/SPL/src/MLDR187_timer.c \
	../libs/SPL/src/MLDR187_uart.c #\
	../libs/SPL/src/MLDR187_adc.c \
	../libs/SPL/src/MLDR187_adcui.c \
	../libs/SPL/src/MLDR187_clk_msr.c \
	../libs/SPL/src/MLDR187_crc.c \
	../libs/SPL/src/MLDR187_cryptoAes.c \
	../libs/SPL/src/MLDR187_cryptoKuz.c \
	../libs/SPL/src/MLDR187_dma.c \
	../libs/SPL/src/MLDR187_eeprom.c \
	../libs/SPL/src/MLDR187_i2c.c \
	../libs/SPL/src/MLDR187_iso7816.c \
	../libs/SPL/src/MLDR187_iwdg.c \
	../libs/SPL/src/MLDR187_math.c \
	../libs/SPL/src/MLDR187_mathTables.c \
	../libs/SPL/src/MLDR187_otp.c \
	../libs/SPL/src/MLDR187_power.c \
	../libs/SPL/src/MLDR187_rng.c \
	../libs/SPL/src/MLDR187_sensors.c \
	../libs/SPL/src/MLDR187_ssp.c \
	../libs/SPL/src/MLDR187_wwdg.c

ASM_SOURCES =  \
	../libs/Startup/startup_MLDR187.S #\
	../libs/SPL/MLDR187_cryptoAes.S \
	../libs/SPL/MLDR187_cryptoKuz.S \
	../libs/SPL/MLDR187_math.S

GCC_PATH = C:/Program Files/Toolchain/xpack-riscv-none-embed-gcc-10.2.0-1.1/bin
OCD_PATH = C:/Program Files/Toolchain/xpack-openocd-0.10.0-15/bin

OCD = "$(OCD_PATH)/openocd"

PREFIX = riscv-none-embed-

CC = "$(GCC_PATH)/$(PREFIX)gcc"
AS = "$(GCC_PATH)/$(PREFIX)gcc" -x assembler-with-cpp
CP = "$(GCC_PATH)/$(PREFIX)objcopy"
SZ = "$(GCC_PATH)/$(PREFIX)size"

HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S 
 
# CPU = -march=rv32ecxw -mabi=ilp32e -msmall-data-limit=0 -msave-restore -fno-common -Wunused -Wuninitialized
CPU = -march=rv32imc -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore

MCU = $(CPU) -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-move-loop-invariants -Wall -Wextra -g3

C_INCLUDES =  \
	-I. \
	-Isrc \
	-I../libs/SPL/inc

ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -MMD -MP -MF"$(@:%.o=%.d)"

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -MMD -MP -MF"$(@:%.o=%.d)"

LDSCRIPT = ../libs/Ld/link_Flash.ld

LDFLAGS = $(MCU) --specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -nostartfiles -Xlinker --gc-sections -Wl,-Map=$(BUILD_DIR)/$(TARGET).map $(OPT) #--specs=nosys.specs -g

all: flash 


build: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))

OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.S=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | make_folder 
	$(CC) -c $(CFLAGS) -g -std=gnu11 -MT"$(@)" -c "$<" -o "$@"
#	$(CC) -c $(CFLAGS) -g -std=gnu99 -MT"$(@)" -c "$<" -o "$@"

$(BUILD_DIR)/%.o: %.s Makefile | make_folder
	$(AS) -c $(CFLAGS) -g -MT"$(@)" -c "$<" -o "$@"

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile | make_folder
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | make_folder
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | make_folder
	$(BIN) $< $@	
	
make_folder:
	@-mkdir $(BUILD_DIR)		


clean:
	@-rmdir /s /q build


OPENOCD_CFG=../libs/Util/MLDR187-ft2232hl.cfg


erase: build
	$(OCD) -f $(OPENOCD_CFG) -c init -c halt -c "flash erase_sector bank0 0 last" -c exit
#	$(OCD) -f $(OPENOCD_CFG) -c init -c halt -c "flash erase_sector wch_riscv 0 last" -c exit
	

flash: erase 
	$(OCD) -f $(OPENOCD_CFG) -c init -c halt -c "program $(BUILD_DIR)/$(TARGET).hex" -c reset -c exit
#	$(OCD) -f $(OPENOCD_CFG) -c init -c halt -c "program $(BUILD_DIR)/$(TARGET).hex" -c wlink_reset_resume -c exit
	

reset:
	$(OCD) -f $(OPENOCD_CFG) -c init -c reset -c resume -c exit
	
