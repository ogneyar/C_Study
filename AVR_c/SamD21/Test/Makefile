
MAIN_FILE_NAME = main
# LD_FILE_NAME = samd21g18a_flash
LD_FILE_NAME = script

CC=arm-none-eabi-gcc
LD=arm-none-eabi-ld
OBJCOPY=arm-none-eabi-objcopy
OBJDUMP=arm-none-eabi-objdump

CFLAGS = -mcpu=cortex-m0plus -mthumb
CFLAGS += -Wall -Werror -std=c11
CFLAGS += -O0
CFLAGS += -g
CFLAGS += -ffunction-sections -fdata-sections # cargo-cult
CFLAGS += -Wl,--gc-sections -Wl,-Map=$(MAIN_FILE_NAME).map
CFLAGS += -T ../libs/$(LD_FILE_NAME).ld

# CFLAGS += -I ./config
# CFLAGS += -I asf/sam0/utils
# CFLAGS += -I asf/sam0/utils/cmsis/samd21/include
# CFLAGS += -I asf/sam0/utils/cmsis/samd21/source

CFLAGS += -D __SAMD21G18A__
CFLAGS += -D SYSTICK_MODE

SRCS = $(MAIN_FILE_NAME).c
# SRCS += ../libs/samd21/source/as_gcc/startup_samd21.c
# SRCS += ../libs/samd21/source/system_samd21.c

.PHONY: all
all: $(MAIN_FILE_NAME).bin

$(MAIN_FILE_NAME).bin: $(MAIN_FILE_NAME).elf $(MAIN_FILE_NAME).lst
	$(OBJCOPY) $< $@ -O binary

$(MAIN_FILE_NAME).lst: $(MAIN_FILE_NAME).elf
	$(OBJDUMP) -D $^ > $@

$(MAIN_FILE_NAME).elf: $(SRCS)
	$(CC) $(CFLAGS) $^ -o $@


.PHONY: clean
clean:
	rm -f -- *.bin *.elf *.o *.lst

.PHONY: upload
upload:
	openocd -d2 -f ../libs/openocd.cfg -c "program {main.bin} verify reset; shutdown"

.PHONY: bossa
bossa:
	bossac --port=com2 --info --erase --write --verify --reset main.bin
#	bossac --port "COM2" --arduino-erase --verify main.bin
#	bossac --port "COM2" --info --verify main.bin
#	bossac -pcom2 -r0x10000 image.bin 

.PHONY: bootloader
bootloader:
	openocd -d2 -f ../libs/openocd.cfg -c init -c halt -c "flash erase_sector 0x00000000 0 last" -c exit
	openocd -d2 -f ../libs/openocd.cfg -c "init; halt; at91samd bootloader 0; program {bootloader/samd21_sam_ba.hex} verify reset; exit"

#   эта команда прошила файл bootloader/samd21_sam_ba.bin в SamD
#	openocd -d2 -f ../libs/openocd.cfg -c "init; halt; at91samd bootloader 0; program {bootloader/samd21_sam_ba.bin} verify reset; shutdown"
