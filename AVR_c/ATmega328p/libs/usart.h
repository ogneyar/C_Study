
#ifndef _USART_H_
#define _USART_H_
	
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

// Example:
// Fcpu = 16000000 ; 16MHz
// BaudRate = 9600
// BaudDivider = (Fcpu/8/BaudRate-1) = 207


// =================================================
// -- Инициализация USART -- 
// 8 бит в пакете, бит четности - нет, 1 стоп бит. битрейт 9600 - по умолчанию
void USART_Init(uint32_t BaudRate)
{
	//-----------------------------------
	// prescaler settings for LGT8F328P
	CLKPR = 0x80;
	CLKPR = 0x00;
	//-----------------------------------
	
	uint16_t UBRR = (F_CPU/8/BaudRate-1); // (8 * x) при U2X0 в 1, (16 * x) при U2X0 в 0
	
	UBRR0H = (uint8_t)(UBRR >> 8);
	UBRR0L = (uint8_t)UBRR;
	
	UCSR0A |= (1 << U2X0); // бит в 1
  
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0); // разрешить работу передатчика и приемника
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00); // UCSZ01 = 1 и UCSZ00 = 1 по умолчанию	
	// UCSR0C |= (1 << UPM01) // UPM01 - Enabled, Even Parity
}


// -- Функция передачи данных -- 
void USART_Transmit(char* str)
{
	uint8_t i = 0;
	while (str[i])
	{
		while ( ! (UCSR0A & (1 << UDRE0)) ) 
			; // ждем пока не освободится регистр передатчика
		UDR0 = str[i];// пишем данные в регистр передатчика для отправки 
		i++;
	}
}

// -- Функция приёма данных -- 
uint8_t USART_Receive(void)
{
	while ( ! (UCSR0A & (1 << RXC0)) )
		;
	return UDR0;
}
// =================================================

#endif  /* _USART_H_ */
