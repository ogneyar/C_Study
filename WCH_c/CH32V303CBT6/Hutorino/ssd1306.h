
#ifndef __SSD1306_H_
#define __SSD1306_H_

#include "i2c.h"


/* Список функций */
void SSD1306_Init(void);
void SSD1306_Write(uint8_t mode, uint8_t data);
void SSD1306_ScreenClear(void);
void SSD1306_ScreenUpdate(void);
void SSD1306_WriteArray(void);


#define SCREEN_BUFFER_LENGTH 512 // 512 * 8 or 128 * 32

uint8_t address = 0x3c;
uint8_t addressWrite = 0x78; // address << 1 | 0x00
uint8_t addressRead = 0x79; // address << 1 | 0x01

uint8_t command = 0x00; // or 0x80 
uint8_t dataByte = 0x40;
uint8_t dataArray = 0xc0;

uint16_t scr_buffer[SCREEN_BUFFER_LENGTH] = {0}; // Буфер дисплея


static const uint8_t ssd1306_128x32_init[] = {
  0xAE, // Выключить дисплей (DISPLAY_OFF)
  
  0x8D, // Управление внутреним преобразователем (CHARGEPUMP)
  0x14, // 0x10 - отключить (VCC подается извне) 0x14 - запустить внутрений DC/DC   
  0x20, // Режим автоматической адресации (ADDRESSING_MODE)
  0x00, // 0-по горизонтали с переходом на новую страницу (строку) // 1 - по вертикали с переходом на новую строку // 2 - только по выбранной странице без перехода   
    
  0xDA, // Аппаратная конфигурация COM (SETCOMPINS)
    0x02, // 0x02 - 128x32, 0x12 - 128x64  	
	
  0xAF, // Дисплей включен (DISPLAY_ON)  
};


// Инициализации дисплея
void SSD1306_Init(void) 
{
	// Посылаем команды в дисплей из массива инициализации
	for(uint8_t i = 0; i < sizeof ssd1306_128x32_init; i++) SSD1306_Write(0, ssd1306_128x32_init[i]);
}


// Функция записи данных/команды в дисплей
void SSD1306_Write(uint8_t mode, uint8_t data) // Режим: 1-данные, 0-команда
{
	if(mode) mode = dataByte; // Режим данных
	else mode = command; // Режим команды
  
	// СТАРТ
	while( I2C2_GetFlagStatus(I2C_FLAG_BUSY) != RESET );
	I2C2_Start();
	
	// отправка адреса (7 бит адреса + 1 бит на запись/чтение )	
	while( !I2C2_CheckEvent( I2C_EVENT_MASTER_MODE_SELECT ) );
	I2C2_Send7bitAddress( (address << 1), I2C_Direction_Transmitter );

	while( !I2C2_CheckEvent( I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ) );

	// отправка команды
	I2C2_SendData( mode );
	while( !I2C2_CheckEvent( I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );

	// отправка данных
	I2C2_SendData( data );
	while( !I2C2_CheckEvent( I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );

	// СТОП
	while( !I2C2_CheckEvent( I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );
	I2C2_Stop();
}


// Функция очистки буфера дисплея
void SSD1306_ScreenClear(void)
{
	for(uint16_t i = 0; i < SCREEN_BUFFER_LENGTH; i++) scr_buffer[i] = 0;
	SSD1306_ScreenUpdate();
}


// Функция обновления дисплея
void SSD1306_ScreenUpdate(void) 
{
	SSD1306_Write(0,0x21); // Установка столбца
	SSD1306_Write(0,0);    // Начальный адрес
	SSD1306_Write(0,127);  // Конечный адрес
	
	SSD1306_Write(0,0x22); // Установка строки
	SSD1306_Write(0,0);    // Начальный адрес
	SSD1306_Write(0,3);    // Конечный адрес
	
	// Запись данных из буфера в дисплей
	SSD1306_WriteArray();  
}


// Функция записи массива данных из буфера в дисплей
void SSD1306_WriteArray(void) 
{
	// СТАРТ
	while( I2C2_GetFlagStatus(I2C_FLAG_BUSY) != RESET );
	I2C2_Start();

	// отправка адреса (7 бит адреса + 1 бит на запись/чтение )	
	while( !I2C2_CheckEvent( I2C_EVENT_MASTER_MODE_SELECT ) );
	I2C2_Send7bitAddress( (address << 1), I2C_Direction_Transmitter );

	while( !I2C2_CheckEvent( I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ) );

	for(uint16_t i = 0; i < SCREEN_BUFFER_LENGTH; i++) {
	    // отправка команды
		I2C2_SendData( dataArray );
		while( !I2C2_CheckEvent( I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );

	    // отправка данных
		I2C2_SendData( scr_buffer[i] );
		while( !I2C2_CheckEvent( I2C_EVENT_MASTER_BYTE_TRANSMITTED ) ); 
	}

	// СТОП
	while( !I2C2_CheckEvent( I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );
	I2C2_Stop();
}



#endif /* __SSD1306_H_ */
