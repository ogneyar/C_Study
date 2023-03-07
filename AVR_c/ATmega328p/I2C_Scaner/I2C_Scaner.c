
#include <avr/io.h>
#ifndef _AVR_IOXXX_H_
#include <avr/iom328p.h>
#endif 
#include <util/delay.h>

#include "../libs/usart.h"
#include "../libs/i2c.h"
#include "../libs/defines.h"

#define F_CPU 16000000UL

void I2C_Scan(void);


int main(void)
{
	DDRB |= (1 << PB5); // пин LED на выход
	USART_Init(115200); // инициализация UART

	USART_Transmit("Поиск I2C устройства начался!");
	USART_Transmit("\n\n");

	I2C_Init(100000); // инициализация I2C

	I2C_Scan();

	while (1)
	{
		PORTB |= (1 << PB5);
		_delay_ms(100);
		PORTB &= ~(1 << PB5);
		_delay_ms(500);
	}

	return 0;
}

// =================================================
// поиск устройства и вывод адреса в порт
void I2C_Scan(void)
{
	uint8_t status, flag = 0;
	uint8_t I2C_Address = 0;

	while (I2C_Address < 128)
	{
		I2C_Start();
		
		status = TWSR & TW_STATUS_MSK;
		if ( (status != TW_START) && (status != TW_RE_START) )
		{
			while(1) {
				PORTB |= (1 << PB5);
				_delay_ms(50);
				PORTB &= ~(1 << PB5);
				_delay_ms(50);
			}
		}
		
		I2C_Write_Address(I2C_Address);

		status = TWSR & TW_STATUS_MSK;
		if (status == TW_MT_SLA_ACK)
		{
			flag = 1;
			I2C_Stop();
			break;
		}

		I2C_Stop();
		I2C_Address++;
	}

	if (flag)
	{
		USART_Transmit("Адрес устройства: 0b");
		for (uint8_t iter = 0; iter < 8; iter++)
		{
			if (I2C_Address & 0x01)
				USART_Transmit("1");
			else
				USART_Transmit("0");
			I2C_Address = (I2C_Address >> 1);
		}
	}
	else
	{
		USART_Transmit("Нет найденных устройств!");
	}

	USART_Transmit("\n\n");
	USART_Transmit("Поиск I2C устройств завершён!");
	USART_Transmit("\n\n");
}
