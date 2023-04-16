
// НЕ РАБОТАЕТ!!! :(

#include <avr/io.h>
#ifndef _AVR_IOXXX_H_
#include <avr/iom328pb.h>
#endif 
#include <util/delay.h>

#define PB5 5

int main(void) {
	DDRB |= (1 << PB5);
	
	while(1) {
		_delay_ms(1000);
		PORTB ^= (1 << PB5);
	}
}
