
#ifndef _SSD1306_H_
#define _SSD1306_H_

#include <avr/io.h>
#ifndef _AVR_IOXXX_H_
#include <avr/iom328p.h>
#endif 
#include <avr/pgmspace.h>
#include <util/delay.h>

#ifdef SSD1306_SPI
#include "spi.h"
#else
#include "i2c.h"
#endif

#include "defines.h"

#ifndef SH1106
#define SH1106 0 /* по умолчанию экран SSD1306 */
#endif

#define SCREEN_BUFFER_LENGTH 1024 // 1024 * 8 or 128 * 64

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define BUFSIZE_128x64 (128*64/8)
#define BUFSIZE_128x32 (128*32/8)

// #ifndef OLED_SPI_SPEED
// #define OLED_SPI_SPEED 1000000ul
// #endif

uint8_t _address = 0x3c;
uint8_t addressWrite = 0x78; // address << 1 | 0x00
uint8_t addressRead = 0x79; // address << 1 | 0x01

uint8_t command = 0x00; // or 0x80 
uint8_t dataByte = 0x40;
uint8_t dataArray = 0xc0;

uint8_t scr_buffer[SCREEN_BUFFER_LENGTH]; // Буфер дисплея

// список инициализации
static const uint8_t _oled_init[] PROGMEM = {
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
	OLED_HEIGHT_64, // OLED_HEIGHT_64 or OLED_HEIGHT_32
	
	OLED_SETMULTIPLEX,
	OLED_64, // OLED_64 or OLED_32
};


void display_init(uint8_t address);
void disp_write(uint8_t mode, uint8_t data); 
void disp_write_array(void);
void screen_clear(void);
void screen_update(void);
void test_screen(void);


// Функция инициализации дисплея
void display_init(uint8_t address) 
{
#ifdef SSD1306_SPI
    SPI_Init();     
    PORT_SPI &= ~(1 << DD_RES); // reset_on();
    _delay_ms(10);
    PORT_SPI |= (1 << DD_RES); // reset_off();
#else
    I2C_Init(400000);
#endif
    _address = address;
    // Посылаем команды в дисплей из массива инициализации
    for(uint8_t i = 0; i < sizeof _oled_init; i++) disp_write(0, pgm_read_byte(&_oled_init[i]));
}

// Функция записи данных/команды в дисплей
void disp_write(uint8_t mode, uint8_t data) // Режим: 1-данные, 0-команда
{
#ifdef SSD1306_SPI
    PORT_SPI &= ~(1 << DD_SS); // select_chip(); 
    if(mode) PORT_SPI |= (1 << DD_DC); // data_mode();
    else PORT_SPI &= ~(1 << DD_DC); // command_mode();
    // USART_Transmit("eee");
    SPI_Transmit(data);
    // USART_Transmit("uf'");
    PORT_SPI |= (1 << DD_SS); // deselect_chip();
#else
    if(mode) mode = dataByte; // Режим данных
    else mode = command; // Режим команды
    
    I2C_Start();
    I2C_Write_Address(_address); // команда на запись (7 бит адреса + 1 бит на запись/чтение )
    I2C_Write(mode);   //Control Byte - Command
    I2C_Write(data);    //payload
    I2C_Stop();
#endif
}

// Функция записи массива данных из буфера в дисплей
void disp_write_array(void) 
{
#ifdef SSD1306_SPI
    PORT_SPI &= ~(1 << DD_SS); // select_chip(); 
    PORT_SPI |= (1 << DD_DC); // data_mode();
    for(uint16_t i = 0; i < SCREEN_BUFFER_LENGTH; i++) SPI_Transmit(scr_buffer[i]);
    PORT_SPI |= (1 << DD_SS); // deselect_chip();
#else
    I2C_Start();
    I2C_Write_Address(_address); // команда на запись (7 бит адреса + 1 бит на запись/чтение )  
    for(uint16_t i = 0; i < SCREEN_BUFFER_LENGTH; i++) {
        I2C_Write(dataArray);
        I2C_Write(scr_buffer[i]);    
    }
    I2C_Stop();    
#endif
}

// Функция очистки буфера дисплея
void screen_clear(void) 
{
    for(uint16_t i = 0; i < SCREEN_BUFFER_LENGTH; i++) scr_buffer[i] = 0;
    screen_update();
}

// Функция обновления дисплея
void screen_update(void) 
{
    if (SH1106) {  // для SSD1106
        disp_write(0, 0x00);
        disp_write(0, 0x10);
        disp_write(0, 0x40);    
        uint16_t ptr = 0;                
        for (uint8_t i = 0; i < (64 >> 3); i++) {
            disp_write(0, 0xB0 + i + 0);    //set page address
            disp_write(0, 0x02);//2 & 0xf);        //set lower column address
            disp_write(0, 0x10);            //set higher column address
            for (uint8_t a = 0; a < OLED_WIDTH; a++) {
                disp_write(1, scr_buffer[a + (i * OLED_WIDTH)]);
                //disp_write(1, scr_buffer[((ptr&0x7F)<<3)+(ptr>>7)]); 
                // 0, 1,  2,  3,  4,  5,  6,  7,  8, ...,  127,  128
                // 0, 8, 16, 24, 32, 40, 48, 56, 64, ..., 1016, 1024
                ptr++;
            }
        }        
    }else {
        disp_write(0, 0x21); // Установка столбца
        disp_write(0, 0);    // Начальный адрес
        disp_write(0, 127);  // Конечный адрес
        
        disp_write(0 ,0x22); // Установка строки
        disp_write(0, 0);    // Начальный адрес
        if (SCREEN_BUFFER_LENGTH == 1024) disp_write(0, 7);    // Конечный адрес
        else disp_write(0, 3);    // Конечный адрес
        
        // Запись данных из буфера в дисплей
        // for(uint16_t i = 0; i < SCREEN_BUFFER_LENGTH; i++) disp_write(1, scr_buffer[i]);
        disp_write_array();  
    }
}

// тесты экрана
void test_screen(void) 
{
    screen_clear();

    _delay_ms(1000);

    uint8_t flag = 0xff;
    for(uint16_t i = 0; i < 1024; i++) {
        scr_buffer[i] = flag;
        if (flag) flag = 0x00;
        else flag = 0xff;
    }
    screen_update();
    
    _delay_ms(1000);

    screen_clear();
    
    _delay_ms(1000);
    
    for(uint16_t i = 0; i < SCREEN_BUFFER_LENGTH; i++) scr_buffer[i] = 0xff;
    screen_update();
}


#endif  /* _SSD1306_H_ */
