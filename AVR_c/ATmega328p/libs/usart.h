
#ifndef _USART_H_
#define _USART_H_
	
	
#include <avr/io.h>
#ifndef _AVR_IOXXX_H_
#include <avr/iom328p.h>
#endif 

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



void USART_Send_Byte(uint8_t data)
{
	while ( ! (UCSR0A & (1 << UDRE0)) ) 
		; // ждем пока не освободится регистр передатчика
	UDR0 = data;// пишем данные в регистр передатчика для отправки 
}


// -- Функция передачи данных -- 
void USART_Transmit(char* str)
{
	uint8_t i = 0;
	while (str[i])
	{
		USART_Send_Byte(str[i]);
		i++;
	}
}

// -- Функция передачи 16тиричных данных -- 
void USART_TransmitHEX(uint8_t hex)
{
	USART_Transmit("0x");

	uint8_t sixteen = ( (hex>>4) & 0x0f );
	if (sixteen > 9) USART_Send_Byte( (sixteen - 10) + 97); // символы от A до F
	else USART_Send_Byte(sixteen + 48); // число от 0 до 9

	sixteen = ( (hex) & 0x0f );
	if (sixteen > 9) USART_Send_Byte( (sixteen - 10) + 97); // символы от A до F
	else USART_Send_Byte(sixteen + 48); // число от 0 до 9
}

// -- Функция передачи 2х байт 16тиричных данных -- 
void USART_TransmitHEX2(uint16_t hex)
{
	USART_Transmit("0x");

	uint8_t sixteen = ( (hex>>12) & 0x0f );
	if (sixteen > 9) USART_Send_Byte( (sixteen - 10) + 97); // символы от A до F
	else USART_Send_Byte(sixteen + 48); // число от 0 до 9

	sixteen = ( (hex>>8) & 0x0f );
	if (sixteen > 9) USART_Send_Byte( (sixteen - 10) + 97); // символы от A до F
	else USART_Send_Byte(sixteen + 48); // число от 0 до 9

	sixteen = ( (hex>>4) & 0x0f );
	if (sixteen > 9) USART_Send_Byte( (sixteen - 10) + 97); // символы от A до F
	else USART_Send_Byte(sixteen + 48); // число от 0 до 9

	sixteen = ( (hex) & 0x0f );
	if (sixteen > 9) USART_Send_Byte( (sixteen - 10) + 97); // символы от A до F
	else USART_Send_Byte(sixteen + 48); // число от 0 до 9
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
