
#   эта команда прошила файл bootloader'а samd21_sam_ba.bin в SamD (Arduino Zero)
all: 
	openocd -d2 -f openocd.cfg -c "init; halt; at91samd bootloader 0; program {samd21_sam_ba.bin} verify reset; shutdown"
