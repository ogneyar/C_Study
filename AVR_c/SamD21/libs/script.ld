OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
MEMORY
{
    /* 0x2000 offset for Arduino Zero bootloader */
    ROM      (rx)  : ORIGIN = 0x00000000+0x2000, LENGTH = 0x00040000-0x2000
    RAM      (rwx) : ORIGIN = 0x20000000, LENGTH = 0x00008000
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