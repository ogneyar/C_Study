
#include <avr/io.h>
//#include <util/delay.h>
#include "../libs/usart.h"

#define F_CPU 16000000UL

uint8_t data;


int main(void) {		
	DDRB |= (1 << PB5); // пин LED на выход 
	USART_Init(115200);	// инициализация UART

	USART_Transmit("Hello Чел!!!\r\n\r\n");

	USART_Transmit("Жми 1 для включения LED\r\n"); 
	USART_Transmit("Жми 0 для выключения\r\n"); 

	while (1) 
	{
		data = USART_Receive();
		if (data == '0')
		{
			PORTB &= ~(1 << PB5);
			USART_Transmit("\n\r");
			USART_Transmit("LED выкл");
			USART_Transmit("\n\r");
		}
		else if (data == '1')
		{
			PORTB |= (1 << PB5);
			USART_Transmit("\n\r");
			USART_Transmit("LED вкл");
			USART_Transmit("\n\r");
		}
	}

	return 0;
}

