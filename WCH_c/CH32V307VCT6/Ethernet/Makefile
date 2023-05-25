

OPT = -Os

BUILD_DIR = build

TARGET = ${BUILD_DIR}

C_SOURCES =  \
	main.c \
	src/system_ch32v30x.c \
	../libs/Debug/debug.c \
	../libs/Peripheral/src/ch32v30x_adc.c \
	../libs/Peripheral/src/ch32v30x_bkp.c \
	../libs/Peripheral/src/ch32v30x_can.c \
	../libs/Peripheral/src/ch32v30x_crc.c \
	../libs/Peripheral/src/ch32v30x_dac.c \
	../libs/Peripheral/src/ch32v30x_dbgmcu.c \
	../libs/Peripheral/src/ch32v30x_dma.c \
	../libs/Peripheral/src/ch32v30x_dvp.c \
	../libs/Peripheral/src/ch32v30x_eth.c \
	../libs/Peripheral/src/ch32v30x_exti.c \
	../libs/Peripheral/src/ch32v30x_flash.c \
	../libs/Peripheral/src/ch32v30x_fsmc.c \
	../libs/Peripheral/src/ch32v30x_gpio.c \
	../libs/Peripheral/src/ch32v30x_i2c.c \
	../libs/Peripheral/src/ch32v30x_iwdg.c \
	../libs/Peripheral/src/ch32v30x_misc.c \
	../libs/Peripheral/src/ch32v30x_opa.c \
	../libs/Peripheral/src/ch32v30x_pwr.c \
	../libs/Peripheral/src/ch32v30x_rcc.c \
	../libs/Peripheral/src/ch32v30x_rng.c \
	../libs/Peripheral/src/ch32v30x_rtc.c \
	../libs/Peripheral/src/ch32v30x_sdio.c \
	../libs/Peripheral/src/ch32v30x_spi.c \
	../libs/Peripheral/src/ch32v30x_tim.c \
	../libs/Peripheral/src/ch32v30x_usart.c \
	../libs/Peripheral/src/ch32v30x_wwdg.c

ASM_SOURCES =  \
	../libs/Startup/startup_ch32v30x_D8C.S

GCC_PATH = E:/Program/MounRiver/MounRiver_Studio/toolchain/RISC-V Embedded GCC/bin
OCD_PATH = E:/Program/MounRiver/MounRiver_Studio/toolchain/OpenOCD/bin

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
	-I. \
	-Isrc \
	-I../libs/Core \
	-I../libs/Debug \
	-I../libs/Peripheral/inc

ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT)

CFLAGS += $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT)

CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

LDSCRIPT = ../libs/Ld/ch32v307_map.ld

LIBS = 
LIBDIR = 
LDFLAGS = $(MCU) -g --specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -nostartfiles -Xlinker \
	--gc-sections -Wl,-Map=$(BUILD_DIR)/$(TARGET).map --specs=nosys.specs $(OPT)


# all: build
all: flash 


build: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))

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
	@-mkdir $@		

clean:
	-rm -fR $(BUILD_DIR)

OPENOCD_CFG=../libs/Util/wch-riscv.cfg

erase: build
	$(OCD) -f $(OPENOCD_CFG) -c init -c halt -c "flash erase_sector wch_riscv 0 last" -c exit

flash: erase 
	$(OCD) -f $(OPENOCD_CFG) -c init -c halt -c "program $(BUILD_DIR)/$(TARGET).hex" -c wlink_reset_resume -c exit

reset:
	$(OCD) -f $(OPENOCD_CFG) -c init -c reset -c resume -c exit

