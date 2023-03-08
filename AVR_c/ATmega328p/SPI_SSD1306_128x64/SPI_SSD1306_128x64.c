
#define SSD1306 1 // SH1106 or SSD1306
#define SSD1306_SPI 1 // SSD1306_SPI or SSD1306_I2C

#include <avr/io.h>
#ifndef _AVR_IOXXX_H_
#include <avr/iom328p.h>
#endif 
#include <util/delay.h>

#include "../libs/ssd1306.h"
#include "../libs/defines.h"

uint8_t address = 0x3c;


int main(void) 
{
    display_init(address);
    
    test_screen();

    while(1) {}

    return 0;  
}

