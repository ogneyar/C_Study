OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
MEMORY
{
    ROM  (rx) : ORIGIN = 0x08000000, LENGTH = 32K  /* Объявляем регион ROM */
    RAM (rwx) : ORIGIN = 0x20000000, LENGTH = 8K  /* Объявляем регион RAM */
}
 
SECTIONS
{
    .isr_vector ORIGIN(ROM):  /* Объявляем выходную секцию .isr_vector, адрес укажем явно: начало региона ROM */
    {
        KEEP(*(.isr_vector))  /* помещаем сюда все символы с атрибутом section(".isr_vector") */
    }
 
    .text ALIGN(16):        /* Объявляем выходную секцию .text, попросим компоновщик выровнять секцию кратно 16 байт */
    {
        *(.text*);          /* Развернем сюда все символы с атрибутом section начинающимся со слова .text */
    }
 
    .data_rom ALIGN(4):     /* Выходная секция для значений инициализации данных из .data */
    {
       /* Создадим символ __data_rom__ с адресом равным текущему */
        PROVIDE(__data_rom__ = .);   
        /* Подвинем указатель на текущий адрес на размер выходной секции .data, она будет объявлена позже */
        . += SIZEOF(.data);     
    }
 
 
    /* Секция для неинициализированных переменных, адрес - начало региона RAM
       Тип NOLOAD, говорит компоновщику, что нужно только распределить адреса, данные нам не важны*/
    .common ORIGIN(RAM) (NOLOAD) : 
    {
        *(COMMON*)                 
    }
 
    .bss ALIGN(4) (NOLOAD): {
        PROVIDE(__bss_start__ = .);
        *(.bss*)
        PROVIDE(__bss_end__ = .);
    }
 
    /* Секция .data, для инициализированных переменных, 
    оператор AT() назначает LMA для этой секции */
    .data ALIGN(4): AT(__data_rom__)
    {
        PROVIDE(__data_start__ = .);
        *(.data*)
        PROVIDE(__data_end__ = .);
    } >RAM
 
    /* Создадим символ __stack_top__ */
    PROVIDE(__stack_top__ = ORIGIN(RAM) + LENGTH(RAM));
}