
#ifndef _SPI_H_
#define _SPI_H_

#include <avr/io.h>
#ifndef _AVR_IOXXX_H_
#include <avr/iom328p.h>
#endif 

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define DDR_SPI  DDRB
#define PORT_SPI PORTB

#define DD_RES 	0 // PB0
#define DD_DC 	1 // PB1
#define DD_SS 	2 // PB2
#define DD_MOSI 3 // PB3
#define DD_MISO 4 // PB4
#define DD_SCK 	5 // PB5


// Инициализация режима Master с управлением потоком по опросу.
void SPI_Init(void) 
{
    /* Настройка на выход */
    DDR_SPI |= (1 << DD_MOSI) | (1 << DD_SCK) | (1 << DD_DC) | (1 << DD_RES) | (1 << DD_SS);
    /* Разрешить работу SPI, режим Master,
        установить скорость тактов: */
    // SPI2X SPR1 SPR0 - 0 0 0 = fck/4; 0 0 1 = fck/16; 0 1 0 = fck/64; 0 1 1 = fck/128; 1 0 0 = fck/2; 1 0 1 = fck/8; 1 1 0 = fck/32; 1 1 1 = fck/64
    SPCR |= (1 << SPE) | (1 << MSTR);// | (1 << SPR1) | (1 << SPR0); 
    SPSR |= (1 << SPI2X);
    
    // SPCR &= ~(1 << CPOL); SPCR &= ~(1 << CPHA);   // Mode 0 (CPOL = 0, CPHA = 0)
    // SPCR &= ~(1 << CPOL); SPCR |= (1 << CPHA);    // Mode 1 (CPOL = 0, CPHA = 1)
    // SPCR |= (1 << CPOL);  SPCR &= ~(1 << CPHA);   // Mode 2 (CPOL = 1, CPHA = 0)
    // SPCR |= (1 << CPOL);  SPCR |= (1 << CPHA);    // Mode 3 (CPOL = 1, CPHA = 1)
}

// Передача данных
void SPI_Transmit(uint8_t data) 
{   
    SPDR = data;
    /* Ожидание завершения передачи: */
    while (!(SPSR & (1 << SPIF))) ;
    asm volatile("nop");
    asm volatile("nop"); // для Nano при максимальной скорости
}

// Передача массива данных
void SPI_SendArray(uint8_t* data) 
{   
    uint8_t* PtrToStrChar = data;  // Установить указатель на начало строки.
    while (*PtrToStrChar != 0) {// Цикл "если не конец строки", то ..
        SPI_Transmit(*PtrToStrChar);
        PtrToStrChar++;  // переход к следующему символу в строке.
    }  
}
// =================================================

#endif  /* _SPI_H_ */
