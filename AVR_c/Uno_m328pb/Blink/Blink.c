
#include <avr/io.h>
#ifndef _AVR_IOXXX_H_
#include <avr/iom328pb.h>
#define PB5 5
#endif 
#include <util/delay.h>


int main(void) {
	DDRB |= (1 << PB5);
	
	while(1) {
		_delay_ms(1000);
		PORTB ^= (1 << PB5);
	}
}
