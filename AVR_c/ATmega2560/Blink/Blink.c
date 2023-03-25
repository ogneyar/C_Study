
#include <avr/io.h>
#ifndef _AVR_IOXXX_H_
#include <avr/iom2560.h>
#endif 
#include <util/delay.h>


int main(void) {
	DDRB |= (1<<PB7);
	
	while(1) {
		_delay_ms(100);
		PORTB ^= (1<<PB7);
	}
}