OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
MEMORY
{
    ROM  (rx) : ORIGIN = 0x08000000, LENGTH = 32K
    RAM (rwx) : ORIGIN = 0x20000000, LENGTH = 10K
}
SECTIONS
{
    .isr_vector :
    {
        KEEP(*(.isr_vector))
 
    } > ROM
   .text :
   {
       *(.text*)
   } > ROM
   PROVIDE(__StackTop = ORIGIN(RAM) + LENGTH(RAM));
}