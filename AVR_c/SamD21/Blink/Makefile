
# -----------------------------------------------------------------------------
# Paths
MODULE_PATH?=$(LOCALAPPDATA)/Arduino15/packages/arduino
RM=del

ARM_GCC_PATH?=$(MODULE_PATH)/tools/arm-none-eabi-gcc/7-2017q4/bin/arm-none-eabi-
BUILD_PATH=build

# -----------------------------------------------------------------------------
# Tools
CC=$(ARM_GCC_PATH)gcc
OBJCOPY=$(ARM_GCC_PATH)objcopy
NM=$(ARM_GCC_PATH)nm
SIZE=$(ARM_GCC_PATH)size

# -----------------------------------------------------------------------------
# Boards definitions
BOARD_ID?=arduino_zero
NAME?=Blink

# -----------------------------------------------------------------------------
# Compiler options
SAM_BA_INTERFACES?=SAM_BA_BOTH_INTERFACES
CFLAGS_EXTRA=-D__SAMD21G18A__ -DBOARD_ID_$(BOARD_ID) -D$(SAM_BA_INTERFACES)
CFLAGS=-mthumb -mcpu=cortex-m0plus -Wall -c -std=gnu99 -ffunction-sections -fdata-sections -nostdlib -nostartfiles --param max-inline-insns-single=500
ifdef DEBUG
  CFLAGS+=-g3 -O1 -DDEBUG=1
else
  CFLAGS+=-Os -DDEBUG=0 -flto
endif

ifdef SECURE_BY_DEFAULT
  CFLAGS+=-DSECURE_BY_DEFAULT=1
endif

ELF=$(NAME).elf
BIN=$(NAME).bin
HEX=$(NAME).hex

INCLUDES=-I"$(MODULE_PATH)/tools/CMSIS/4.5.0/CMSIS/Include/" -I"$(MODULE_PATH)/tools/CMSIS-Atmel/1.2.0/CMSIS/Device/ATMEL/"

# -----------------------------------------------------------------------------
# Linker options
LDFLAGS=-mthumb -mcpu=cortex-m0plus -Wall -Wl,--cref -Wl,--check-sections -Wl,--gc-sections -Wl,--unresolved-symbols=report-all
LDFLAGS+=-Wl,--warn-common -Wl,--warn-section-align -Wl,--warn-unresolved-symbols --specs=nano.specs --specs=nosys.specs


# -----------------------------------------------------------------------------
# Source files and objects
SOURCES= \
  Blink.c \
  


OBJECTS=$(addprefix $(BUILD_PATH)/, $(SOURCES:.c=.o))
DEPS=$(addprefix $(BUILD_PATH)/, $(SOURCES:.c=.d))


LD_SCRIPT := ../libs/samd21_map.ld



all: print_info $(SOURCES) $(BIN) $(HEX) clean


$(ELF): Makefile $(BUILD_PATH) $(OBJECTS)
	@echo ----------------------------------------------------------
	@echo Creating ELF binary
	"$(CC)" -L. -L$(BUILD_PATH) $(LDFLAGS) -Os -Wl,--gc-sections -save-temps -T$(LD_SCRIPT) -Wl,-Map,"$(BUILD_PATH)/$(NAME).map" -o "$(BUILD_PATH)/$(ELF)" -Wl,--start-group $(OBJECTS) -lm -Wl,--end-group
	"$(NM)" "$(BUILD_PATH)/$(ELF)" >"$(BUILD_PATH)/$(NAME)_symbols.txt"
	"$(SIZE)" --format=sysv -t -x $(BUILD_PATH)/$(ELF)

$(BIN): $(ELF)
	@echo ----------------------------------------------------------
	@echo Creating flash binary
	"$(OBJCOPY)" -O binary $(BUILD_PATH)/$< $@

$(HEX): $(ELF)
	@echo ----------------------------------------------------------
	@echo Creating flash binary
	"$(OBJCOPY)" -O ihex $(BUILD_PATH)/$< $@

$(BUILD_PATH)/%.o: %.c
	@echo ----------------------------------------------------------
	@echo Compiling $< to $@
	"$(CC)" $(CFLAGS) $(CFLAGS_EXTRA) $(INCLUDES) $< -o $@
	@echo ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

$(BUILD_PATH):
	@echo ----------------------------------------------------------
	@echo Creating build folder
	-mkdir $(BUILD_PATH)

print_info:
	@echo ----------------------------------------------------------
	@echo Compiling bootloader using
	@echo BASE PATH = $(MODULE_PATH)
	@echo GCC  PATH = $(ARM_GCC_PATH)
	

clean: 
	@echo ----------------------------------------------------------
	@echo Cleaning project
	-$(RM) *.out
	-$(RM) *.s
	-$(RM) *.o
	-$(RM) *.res
