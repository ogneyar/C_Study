riscv-none-embed-objcopy -O ihex "Bootloader.elf" "Bootloader.hex"
..\srec_cat\srec_cat.exe "Bootloader.hex" -intel -o "BootloaderCode.h" -C-Array data -C_COMpressed