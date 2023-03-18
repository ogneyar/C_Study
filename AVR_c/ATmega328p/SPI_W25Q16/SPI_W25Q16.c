
#include <avr/io.h>
#ifndef _AVR_IOXXX_H_
#include <avr/iom328p.h>
#endif 
#include <util/delay.h>

#include "../libs/usart.h"
#include "../libs/w25q16.h"
// #include "../libs/defines.h"

uint16_t page = 0x0000;
uint8_t pageAddress = 0x00;
uint8_t data = 0x00;
uint8_t * dataPage[256] = {0xaa};

uint16_t statReg;

int main(void) 
{
    USART_Init(9600);
    flash_init();

    // flash_chipErase();
    flash_sectorErase(0);
    
    USART_Transmit("\n\n");
    uint8_t manID = flash_manufacturerID(); 
    USART_Transmit("Manufacturer ID: ");
    USART_TransmitHEX(manID);

    // USART_Transmit("\n\n");
    // uint8_t devID = flash_deviceID(); 
    // USART_Transmit("Device ID: ");
    // USART_TransmitHEX(devID);

    // USART_Transmit("\n\n");
    // uint8_t uniqID = flash_uniqueID(); 
    // USART_Transmit("Unique ID: ");
    // USART_TransmitHEX(uniqID);

    USART_Transmit("\n\n");
    uint16_t jedecID = flash_jedecID(); 
    USART_Transmit("JDEC ID: ");
    USART_TransmitHEX2(jedecID);

    // flash_writeStatusRegister1(0xe0);
    
    USART_Transmit("\n\n");
    statReg = flash_readStatusRegister1(); 
    USART_Transmit("StatusReg: ");
    USART_TransmitHEX(statReg);
        
    // flash_powerDown(); 
  
    // _delay_ms(2000);
    
    // flash_releasePowerDown(); 

    USART_Transmit("\n\n");
    data = flash_read(page, pageAddress);
    USART_Transmit("ReadData: ");
    USART_TransmitHEX(data);    

    // USART_Transmit("\n\n");
    // flash_readPage(page, pageAddress, dataPage); 
    // USART_Transmit("ReadDataPage: ");
    // USART_TransmitHEX(dataPage[0]);

    // USART_Transmit("\n\n");
    // flash_write(page, pageAddress, data-1);        
    // USART_Transmit("WriteData: ");
    // USART_TransmitHEX(data-1);
    


    for(int i = 0; i < 256; i++)
    {
        dataPage[i] = 0x00;
    }
    
    USART_Transmit("\n\n");
    flash_writePage(page, pageAddress, dataPage); 
    USART_Transmit("WriteDataPage: xxxx");


    
    // USART_Transmit("\n\n");
    // statReg = flash_readStatusRegister1(); 
    // USART_Transmit("StatusReg: ");
    // USART_TransmitHEX(statReg);

    USART_Transmit("\n\n");
    data = flash_read(page, pageAddress);
    USART_Transmit("ReadData: ");
    USART_TransmitHEX(data);
    
    // USART_Transmit("\n\n");
    // flash_readPage(page, pageAddress, dataPage); 
    // USART_Transmit("ReadDataPage: ");
    // USART_TransmitHEX(dataPage[0]);
    
    // flash_powerDown(); 

    while(1) {}

    return 0;  
}

