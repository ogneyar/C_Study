
#include "Blink_NL_2.h" 

void delay(uint16_t inter);

uint16_t delay_ms = 100;

// основная функция
int main(void) 
{
    REG_PORT_DIRSET0 |= PORT_PA17;      // Set pin as output

    while(1){
        REG_PORT_OUTTGL0 |= PORT_PA17;  // toggle
        delay(delay_ms);
        // REG_PORT_OUTSET0 |= PORT_PA17;      // Set pin to high
        // delay(200);
        // REG_PORT_OUTCLR0 |= PORT_PA17;      // Set pin to low
        // delay(800);
    }
}

// функция задержки
void delay(uint16_t inter) {
	for(uint32_t i = 0; i < (uint32_t)(inter<<7); i++) __asm("nop");
}
