
#include "Blink.h" 

void GPIO_Init(void);
void delay(uint16_t inter);

uint16_t delay_ms = 500;

#define PORTA  0


int main(void) {  

    GPIO_Init();

    while(1){
        PORT->Group[PORTA].OUTTGL.reg = PORT_PA17;  // toggle
        delay(delay_ms);
    }
}


void GPIO_Init(void)
{    
    PORT->Group[PORTA].DIRSET.reg = PORT_PA17;      // Set pin as output
    PORT->Group[PORTA].OUTCLR.reg = PORT_PA17;      // Set pin to low
}

void delay(uint16_t inter) {
	for(uint32_t i = 0; i < (uint32_t)(inter<<7); i++) __asm("nop");
}

