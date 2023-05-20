
MEMORY
{
	FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 16K
	RAM (xrw)  : ORIGIN = 0x20000000, LENGTH = 2K
}

SECTIONS
{
	.text : { /* Секция прошивки */
		*(.text);   /* Секция исходников */
	} > FLASH
}
