
######################################
# target
######################################
TARGET = firmware


######################################
# building variables
######################################
# debug build?
DEBUG = 0
# optimization
OPT = -Os


#######################################
# paths
#######################################
# Build path
BUILD_DIR = build

######################################
# source
######################################
# C sources
C_SOURCES =  \
	main.c \
	src/system_ch32v00x.c \
	../libs/Debug/debug.c \
	../libs/Peripheral/src/ch32v00x_adc.c \
	../libs/Peripheral/src/ch32v00x_dbgmcu.c \
	../libs/Peripheral/src/ch32v00x_dma.c \
	../libs/Peripheral/src/ch32v00x_exti.c \
	../libs/Peripheral/src/ch32v00x_flash.c \
	../libs/Peripheral/src/ch32v00x_gpio.c \
	../libs/Peripheral/src/ch32v00x_i2c.c \
	../libs/Peripheral/src/ch32v00x_iwdg.c \
	../libs/Peripheral/src/ch32v00x_misc.c \
	../libs/Peripheral/src/ch32v00x_opa.c \
	../libs/Peripheral/src/ch32v00x_pwr.c \
	../libs/Peripheral/src/ch32v00x_rcc.c \
	../libs/Peripheral/src/ch32v00x_spi.c \
	../libs/Peripheral/src/ch32v00x_tim.c \
	../libs/Peripheral/src/ch32v00x_usart.c \
	../libs/Peripheral/src/ch32v00x_wwdg.c

# ASM sources
ASM_SOURCES =  \
	../libs/Startup/startup_ch32v00x.S

##########################################################################################################################
# modified by Ngo Hung Cuong
ifeq ($(OS),Windows_NT)
# Windows
	GCC_PATH = E:\Program\MounRiver/MounRiver_Studio/toolchain/RISC-V Embedded GCC/bin
	OCD_PATH = E:\Program\MounRiver/MounRiver_Studio/toolchain/OpenOCD/bin
else
# Linux
	GCC_PATH = "/home/ngohungcuong/Downloads/MRS_Toolchain_Linux_x64_V1.60/RISC-V Embedded GCC/bin"
	OCD_PATH = "/home/ngohungcuong/Downloads/MRS_Toolchain_Linux_x64_V1.60/OpenOCD/bin"
endif

ifdef OCD_PATH
	ifeq ($(OS), Windows_NT)
		OCD = "$(OCD_PATH)/openocd"
	else
		OCD = $(OCD_PATH)/openocd
	endif
else
	OCD = openocd
endif
##########################################################################################################################

#######################################
# binaries
#######################################
PREFIX = riscv-none-embed-
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
	ifeq ($(OS), Windows_NT)
		CC = "$(GCC_PATH)/$(PREFIX)gcc"
		AS = "$(GCC_PATH)/$(PREFIX)gcc" -x assembler-with-cpp
		CP = "$(GCC_PATH)/$(PREFIX)objcopy"
		SZ = "$(GCC_PATH)/$(PREFIX)size"
	else
		CC = $(GCC_PATH)/$(PREFIX)gcc
		AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
		CP = $(GCC_PATH)/$(PREFIX)objcopy
		SZ = $(GCC_PATH)/$(PREFIX)size
	endif
else
	CC = $(PREFIX)gcc
	AS = $(PREFIX)gcc -x assembler-with-cpp
	CP = $(PREFIX)objcopy
	SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
 
#######################################
# CFLAGS
#######################################
# cpu
CPU = -march=rv32ecxw

# fpu
# NONE for Cortex-M0/M0+/M3

# float-abi

# mcu
MCU = $(CPU) -mabi=ilp32e -msmall-data-limit=0 -msave-restore -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized

#riscv-none-embed-gcc   -c -o "$@" "$<"
# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS =  

# AS includes
AS_INCLUDES = 

# C includes
C_INCLUDES =  \
-I. \
-Isrc \
-I../libs/Core \
-I../libs/Debug \
-I../libs/Peripheral/inc

# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT)

CFLAGS += $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT)

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif


# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"


#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = ../libs/Ld/ch32v00x_map.ld

# libraries
#LIBS = -lc -lm -lnosys 
LIBS = 
LIBDIR = 
LDFLAGS = $(MCU) -g --specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -nostartfiles -Xlinker \
	--gc-sections -Wl,-Map=$(BUILD_DIR)/$(TARGET).map --specs=nosys.specs $(OPT)

# default action: build all
all: flash #$(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin clear_build


build: clear_build #$(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin clear_build


clear_build: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin
#	-del $(BUILD_DIR)\*.o
#	-del $(BUILD_DIR)\*.d
	

#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.S=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -g -std=gnu99 -MT"$(@)" -c "$<" -o "$@"

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) -g -MT"$(@)" -c "$<" -o "$@"

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	@-mkdir $@		

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)

#######################################
# OPENOCD config file
#######################################
OPENOCD_CFG = ../libs/Util/wch-riscv.cfg

#######################################
# erase
#######################################
erase: #build
	$(OCD) -f $(OPENOCD_CFG) -c init -c halt -c "flash erase_sector wch_riscv 0 last" -c exit

#######################################
# flash
#######################################
flash: build #erase #$(BUILD_DIR)/$(TARGET).hex
	$(OCD) -f $(OPENOCD_CFG)  -c init -c halt  -c "program $(BUILD_DIR)/$(TARGET).hex" -c wlink_reset_resume -c exit
#	$(OCD) -f $(OPENOCD_CFG)  -c init -c halt  -c "program $(BUILD_DIR)/$(TARGET).hex" -c "verify_image $(BUILD_DIR)/$(TARGET).hex" -c wlink_reset_resume -c exit

#######################################
# reset
#######################################
reset: #flash
	$(OCD) -f $(OPENOCD_CFG) -c init -c reset -c resume -c exit

#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***