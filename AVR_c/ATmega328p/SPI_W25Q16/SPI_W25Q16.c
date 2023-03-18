
#include <avr/io.h>
#ifndef _AVR_IOXXX_H_
#include <avr/iom328p.h>
#endif 
#include <util/delay.h>

#include "../libs/usart.h"
#include "../libs/w25q16.h"
// #include "../libs/defines.h"

unsigned int page = 0;
uint8_t pageAddress = 0;
uint8_t data;


int main(void) 
{
    USART_Init(9600);
    flash_init();

    // flash_chipErase();
    
    USART_Transmit("\n\n");
    uint8_t manID = flash_manufacturerID(); 
    USART_Transmit("Manufacturer ID: ");
    USART_TransmitHEX(manID);

    USART_Transmit("\n\n");
    uint8_t devID = flash_deviceID(); 
    USART_Transmit("Device ID: ");
    USART_TransmitHEX(devID);

    USART_Transmit("\n\n");
    uint8_t uniqID = flash_uniqueID(); 
    USART_Transmit("Unique ID: ");
    USART_TransmitHEX(uniqID);

    USART_Transmit("\n\n");
    uint16_t jedecID = flash_jedecID(); 
    USART_Transmit("JDEC ID: ");
    USART_TransmitHEX2(jedecID);
    
    flash_powerDown(); 
  
    _delay_ms(2000);
    
    flash_releasePowerDown(); 

    USART_Transmit("\n\n");
    data = flash_read(page, pageAddress);    
    USART_Transmit("ReadData: ");
    USART_TransmitHEX(data);

    USART_Transmit("\n\n");
    flash_write(page, pageAddress, data-1);        
    USART_Transmit("WriteData: ");
    USART_TransmitHEX(data-1);

    USART_Transmit("\n\n");
    data = flash_read(page, pageAddress);    
    USART_Transmit("ReadData: ");
    USART_TransmitHEX(data);
    
    flash_powerDown(); 

    while(1) {}

    return 0;  
}

