
#include "delay.h"

// delay
void delay(uint16_t inter)
{
	for(uint32_t i = 0; i < (uint32_t)(inter<<10); i++) __asm("nop");
}
