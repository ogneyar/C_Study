
#ifndef _I2C_H_
#define _I2C_H_

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

void I2C_Init(uint32_t BaudRate);
void I2C_Start();
void I2C_Stop();
void I2C_Write(uint8_t data);
uint8_t I2C_Read(uint8_t ack);
void I2C_Wait();
void I2C_Write_Address(uint8_t address);

// Example
// Fcpu = 16000000 ; 16MHz
// BaudRate = 100000 ; 100kHz
// BaudDivider = ((Fcpu/BaudRate)-16)/2 = 72

// =================================================
// -- функция инициализации I2C
void I2C_Init(uint32_t BaudRate)
{
	uint8_t BaudDivider = ((F_CPU/BaudRate)-16)/2;
	
	TWSR = 0; // TWSR = 0 => prescaler = 1
	TWBR = BaudDivider; // 72 при 100KHz
	
	// SDA и SCL пины на выход
	DDRC |= (1 << PC4) | (1 << PC5); 	
	PORTC |= (1 << PC4) | (1 << PC5); 
} 

// I2C команда СТАРТ
void I2C_Start()
{
	TWCR |= (1 << TWINT) | (1 << TWEN) | (1 << TWSTA); // TWSTA - команда СТАРТ
	I2C_Wait();	
}

// I2C команда СТОП
void I2C_Stop()
{
	TWCR |= (1 << TWINT) | (1 << TWEN) | (1 << TWSTO); // TWSTO - команда СТОП
	while(TWCR & (1 << TWSTO)) ;	
}

	
// I2C передача данных 
void I2C_Write(uint8_t data)
{
	TWDR = data;
	TWCR = (1 << TWINT) | (1 << TWEN);
	I2C_Wait();
}

// I2C приём данных
uint8_t I2C_Read(uint8_t ack)
{
	if (ack) 
	{
		TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA); // TWEA - Enable Ack
	}
	else 
	{
		TWCR = (1 << TWINT) | (1 << TWEN);
	}		
	I2C_Wait();
	
	return TWDR;
}

// I2C, ожидание снятия флага TWINT
void I2C_Wait()
{
	while(TWCR & (1 << TWINT)) ;
}

// I2C передача адреса 
void I2C_Write_Address(uint8_t address)
{
	I2C_Write(address << 1);
}
// =================================================

#endif  /* _I2C_H_ */

