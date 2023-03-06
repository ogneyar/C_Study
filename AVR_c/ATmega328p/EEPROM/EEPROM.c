
#include <avr/io.h>
#include <util/delay.h>

#include "../libs/eeprom.h"

uint16_t address = 1;
uint8_t key = 137;
uint8_t flag = 0;


uint16_t main(void) {	
	DDRB |= (1 << PB5);
	
	if (EEPROM_read(address) != key) {
		EEPROM_write(address, key);
		flag = 1;
	}
	
	while(1) {
		PORTB |= (1 << PB5);
		_delay_ms(100);
		PORTB &= ~(1 << PB5);
		if (flag) _delay_ms(500);
		else _delay_ms(100);
	}
	
	return 0;
}

