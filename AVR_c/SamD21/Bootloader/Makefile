
all: 
#	очистка flash памяти
	openocd -d2 -f openocd.cfg -c init -c halt -c "flash erase_sector 0x00000000 0 last" -c exit
#   эта команда прошила файл bootloader'а samd21_sam_ba.bin в SamD (Arduino Zero)
	openocd -d2 -f openocd.cfg -c "init; halt; at91samd bootloader 0; program {samd21_sam_ba.bin} verify reset; exit"
