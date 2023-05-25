

OPT = -Os

BUILD_DIR = build

TARGET = target #${BUILD_DIR}

C_SOURCES =  \
	delay.c \
	main.c 

ASM_SOURCES =  \
	../libs/Startup/startup_ch32v30x_D8C.S

GCC_PATH = E:/Program/MounRiver/MounRiver_Studio/toolchain/RISC-V Embedded GCC/bin
OCD_PATH = E:/Program/MounRiver/MounRiver_Studio/toolchain/OpenOCD/bin

OCD = "$(OCD_PATH)/openocd"

PREFIX = riscv-none-embed-

CC = "$(GCC_PATH)/$(PREFIX)gcc"
AR = "$(GCC_PATH)/$(PREFIX)ar"
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

LDSCRIPT = ../libs/Ld/ch32v307_map.ld

LIBS = 
LIBDIR = 
LDFLAGS = $(MCU) -g --specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -nostartfiles -Xlinker \
	--gc-sections -Wl,-Map=build/target.map --specs=nosys.specs $(OPT)


all: flash 


build: build/target.elf build/target.hex build/target.bin
# build: clean build/target.elf build/target.hex build/target.bin


OBJECTS = build/main.o \
	build/startup_ch32v30x_D8C.o 

# build/delay.o: delay.c Makefile
# 	@if not exist build (mkdir build)	
# 	$(CC) -c $(CFLAGS) -g -std=gnu99 -MT"$(@)" -c "$<" -o "$@"

# build/delay.a: build/delay.o Makefile
# 	@if not exist build (mkdir build)	
# 	$(AR) q "$@" "$<"

build/main.o: main.c Makefile
	@if not exist build (mkdir build)	
	$(CC) -c $(CFLAGS) -g -std=gnu99 -MT"$(@)" -c "$<" -o "$@"


build/startup_ch32v30x_D8C.o: ../libs/Startup/startup_ch32v30x_D8C.S Makefile
	@if not exist build (mkdir build)	
	$(AS) -c $(CFLAGS) -g -MT"$(@)" -c "$<" -o "$@"

build/target.elf: $(OBJECTS)  Makefile
	@if not exist build (mkdir build)	
	$(CC) $(OBJECTS) build/delay.a $(LDFLAGS) -o $@
	$(SZ) $@

build/target.hex: build/target.elf
	@if not exist build (mkdir build)	
	$(HEX) $< $@
	
build/target.bin: build/target.elf
	@if not exist build (mkdir build)	
	$(BIN) $< $@	
	
build:
	@if not exist build (mkdir build)	
	@-mkdir $@		

clean:
	@-rmdir /s /q build

OPENOCD_CFG=../libs/Util/wch-riscv.cfg

erase: build
	$(OCD) -f $(OPENOCD_CFG) -c init -c halt -c "flash erase_sector wch_riscv 0 last" -c exit

flash: erase 
	$(OCD) -f $(OPENOCD_CFG) -c init -c halt -c "program build/target.hex" -c wlink_reset_resume -c exit

reset:
	$(OCD) -f $(OPENOCD_CFG) -c init -c reset -c resume -c exit

