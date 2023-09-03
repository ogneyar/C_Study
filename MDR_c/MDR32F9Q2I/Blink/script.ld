OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
MEMORY
{
    FLASH  (rx) : ORIGIN = 0x08000000, LENGTH = 128K
    RAM (rwx) : ORIGIN = 0x20000000, LENGTH = 32K
}
SECTIONS
{
    .isr_vector :
    {
        KEEP(*(.isr_vector))
 
    } > FLASH
   .text :
   {
       *(.text*)
   } > FLASH
   PROVIDE(__StackTop = ORIGIN(RAM) + LENGTH(RAM));
}