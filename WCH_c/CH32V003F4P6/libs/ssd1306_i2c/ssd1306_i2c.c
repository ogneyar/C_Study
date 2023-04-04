//
#include <ch32v00x.h>
#include "ssd1306_i2c.h"


u8 scr_buffer[Size] = { 0x00 };
  
static const uint8_t columnAndRow[] = { 
    0x21, // Установка столбца
    0x00, // Начальный адрес
    127,  // Конечный адрес
    0x22, // Установка строки
    0x00, // Начальный адрес
    3     // Конечный адрес
};

const uint8_t command = 0;
const uint8_t data = 1;
const uint8_t dataByte = 0x40;
const uint8_t dataArray = 0xc0;

// список инициализации
static const uint8_t ssd1306_128x32_init[] = {
    OLED_DISPLAY_OFF,
    OLED_CLOCKDIV,
    0x80,    // value
    OLED_CHARGEPUMP,
    0x14,    // value
    OLED_ADDRESSING_MODE,
    OLED_HORIZONTAL, // OLED_HORIZONTAL or OLED_VERTICAL
    OLED_NORMAL_H,
    OLED_NORMAL_V,
    OLED_CONTRAST,
    0x7F,    // value
    OLED_SETVCOMDETECT,
    0x40,     // value
    OLED_NORMALDISPLAY,
    OLED_DISPLAY_ON,
	
	OLED_SETCOMPINS,
	OLED_HEIGHT_32, // OLED_HEIGHT_64 or OLED_HEIGHT_32
	
	OLED_SETMULTIPLEX,
	OLED_32, // OLED_64 or OLED_32
};


// инициализация дисплея
void SSD1306_Init(void) 
{ 
    for(uint16_t i = 0; i < sizeof ssd1306_128x32_init; i++) {
        SSD1306_Write(command, ssd1306_128x32_init[i]);
    }
}

// Функция записи данных/команды в дисплей
void SSD1306_Write(uint8_t mode, uint8_t data) // Режим: 1-данные, 0-команда
{
    if(mode) mode = dataByte; // Режим данных
    else mode = command; // Режим команды
    
    /* ------------------------------------------------ */
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) != RESET);
    I2C_GenerateSTART(I2C1, ENABLE);

    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(I2C1, addressWrite, I2C_Direction_Transmitter);

    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TXE) == RESET);
    I2C_SendData(I2C1, mode);  
                
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TXE) == RESET);
    I2C_SendData(I2C1, data);

    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    I2C_GenerateSTOP(I2C1, ENABLE);
    /* ------------------------------------------------ */
}

// Функция записи данных/команды в дисплей
void SSD1306_WriteArray(uint8_t * data, uint16_t length) // Режим: 1-данные, 0-команда
{
    uint16_t i = 0;
    uint8_t send = 0;
    
    /* ------------------------------------------------ */
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) != RESET);
    I2C_GenerateSTART(I2C1, ENABLE);

    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(I2C1, addressWrite, I2C_Direction_Transmitter);

    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    while(i < length)
    {
        if(I2C_GetFlagStatus(I2C1, I2C_FLAG_TXE) != RESET)
        {
            if ( ! send ) {
                I2C_SendData(I2C1, dataArray);     
                send = 1;           
            }else {
                I2C_SendData(I2C1, data[i]);
                i++;
                send = 0;
            }
        }
    }

    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    I2C_GenerateSTOP(I2C1, ENABLE);
    /* ------------------------------------------------ */
}


// Функция установки строк и столбцов
void SSD1306_SetColumnAndRow(void)
{    
    for(uint16_t i = 0; i < sizeof columnAndRow; i++) {
        SSD1306_Write(command, columnAndRow[i]);
    }
}

// Функция очистки дисплея
void SSD1306_ClearScreen(void)
{    
    for(uint16_t i = 0; i < Size; i++) scr_buffer[i] = 0x00;
    SSD1306_SetColumnAndRow();
    SSD1306_WriteArray(scr_buffer, Size);
}

//
void SSD1306_Fill(uint8_t data)
{ 
    for(uint16_t i = 0; i < Size; i++) scr_buffer[i] = data;    
    SSD1306_SetColumnAndRow();
    SSD1306_WriteArray(scr_buffer, Size);
}

//
void SSD1306_TestScreen(void)
{ 
    SSD1306_ClearScreen();
    Delay_Ms(1000);
    SSD1306_Fill(0xaa);
    Delay_Ms(1000);
    SSD1306_Fill(0x55);
    Delay_Ms(1000);
    SSD1306_ClearScreen();
    Delay_Ms(1000);
    SSD1306_Fill(0xff);
}

    
