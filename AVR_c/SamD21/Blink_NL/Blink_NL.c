
#include "Blink_NL.h" 

void delay(uint16_t inter);

uint16_t delay_ms = 100;

// основная функция
int main(void) 
{
    PORT->Group[PORTA].DIRSET.reg |= PORT_PA17;      // Set pin as output

    while(1){
        // PORT->Group[PORTA].OUTTGL.reg |= PORT_PA17;  // toggle
        // delay(delay_ms);
        PORT->Group[PORTA].OUTSET.reg |= PORT_PA17;      // Set pin to high
        delay(200);
        PORT->Group[PORTA].OUTCLR.reg |= PORT_PA17;      // Set pin to low
        delay(800);
    }
}

// функция задержки
void delay(uint16_t inter) {
	for(uint32_t i = 0; i < (uint32_t)(inter<<7); i++) __asm("nop");
}
