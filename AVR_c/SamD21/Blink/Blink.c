
#include "Blink.h" 


void delay(uint16_t inter);

uint16_t delay_ms = 10;


int main(void) {  
    while(1){
        delay(delay_ms);
    }
}


void delay(uint16_t inter) {
	for(uint32_t i = 0; i < (uint32_t)(inter<<12); i++) __asm("nop");
}
