
#define SH1106 1 // закомментируй для экрана SSD1306

#include <avr/io.h>
#ifndef _AVR_IOXXX_H_
#include <avr/iom328p.h>
#endif 
#include <util/delay.h>
#include <avr/pgmspace.h>

#include "../libs/i2c.h"
#include "../libs/ssd1306.h"
#include "../libs/defines.h"

uint8_t address = 0x3c;


int main(void) 
{
    I2C_Init(400000);
    display_init(address);
    
    screen_clear();
    screen_update();

    _delay_ms(1000);

    test_screen();

    // while(1) {}

    return 0;  
}


