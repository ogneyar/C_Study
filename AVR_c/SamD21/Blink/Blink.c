

// #include <Arduino.h>
// #include <avr/io.h>
// #include <util/delay.h>


// int main(void) {
// 	DDRB |= (1<<PB7);
	
// 	while(1) {
// 		_delay_ms(100);
// 		PORTB ^= (1<<PB7);
// 	}
// }


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}


void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
  delay(900);
}

