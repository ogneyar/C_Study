
#include <avr/io.h>
#ifndef _AVR_IOXXX_H_
#include <avr/iom328p.h>
#endif 
#include <util/delay.h>

#include "../libs/usart.h"
#include "../libs/w25q16.h"

uint16_t page = 0x0000;
uint8_t pageAddress = 0x00;
uint8_t data = 0x00;
uint8_t dataPage[256] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};

uint16_t statReg;

void get_manufacturerID(void);
void get_deviceID(void);
void get_uniqueID(void);
void get_jedecID(void);
void get_statusRegister1(void);
void get_readData(uint16_t _page, uint8_t _address);
void get_readPage(uint16_t _page);
void set_writeData(uint16_t _page, uint8_t _address, uint8_t _data);
void set_writePage(uint16_t _page, uint8_t * _data, uint32_t _length);


int main(void) 
{
    USART_Init(115200);
    flash_init();
    // flash_chipErase();
    flash_sectorErase(0);
    get_manufacturerID();
    // get_deviceID();
    // get_uniqueID();
    get_jedecID();
    // flash_writeStatusRegister1(0xe0);    
    get_statusRegister1();    

    // get_readData(page, pageAddress);
    get_readPage(page);

    // set_writeData(page, 7, 0x05);
    // set_writeData(page, 3, 0x05);
    // set_writeData(page, 0, 0x05);

    uint16_t leng = sizeof dataPage;
    for(int i = 0; i < leng; i++) dataPage[(uint8_t)i] = (uint8_t)(leng-1-i); // от 0 до 0xff
    // for(int i = 0; i < sizeof dataPage; i++) dataPage[(uint8_t)i] = (uint8_t)i; // от 0 до 0xff
    // for(int i = 0; i < sizeof dataPage; i++) dataPage[(uint8_t)i] = 0x55; // от 0 до 0xff    
    set_writePage(page, dataPage, sizeof dataPage);
     
    // get_statusRegister1();    

    // get_readData(page, pageAddress);
    get_readPage(page);
        
    flash_powerDown(); 

    while(1) {}

    return 0;  
}


void get_manufacturerID(void)
{
    USART_Transmit("\n");
    uint8_t manID = flash_manufacturerID(); 
    USART_Transmit("Manufacturer ID: ");
    USART_TransmitHEX(manID);
}
    
void get_deviceID(void)
{
    USART_Transmit("\n");
    uint8_t devID = flash_deviceID(); 
    USART_Transmit("Device ID: ");
    USART_TransmitHEX(devID);
}
    
void get_uniqueID(void)
{
    USART_Transmit("\n");
    uint8_t uniqID = flash_uniqueID(); 
    USART_Transmit("Unique ID: ");
    USART_TransmitHEX(uniqID);
}
    
void get_jedecID(void)
{
    USART_Transmit("\n");
    uint16_t jedecID = flash_jedecID(); 
    USART_Transmit("JDEC ID: ");
    USART_TransmitHEX2(jedecID);
}
    
void get_statusRegister1(void)
{
    USART_Transmit("\n");
    statReg = flash_readStatusRegister1(); 
    USART_Transmit("StatusReg: ");
    USART_TransmitHEX(statReg);
}
    
void get_readData(uint16_t _page, uint8_t _address)
{
    USART_Transmit("\n\n");
    data = flash_read(_page, _address);
    USART_Transmit("ReadData: ");
    USART_TransmitHEX(data);    
}
    
void get_readPage(uint16_t _page)
{
    uint8_t dataP[256] = {0};
    flash_readPage(_page, &dataP);
    USART_Transmit("\n\n");
    for(int i = 0; i < 16; i++)
    {        
        for(int j = 0; j < 16; j++)
        {        
            USART_TransmitHEX(dataP[(uint8_t)(j+(i*16))]);
            USART_Transmit("\t");
        }
        USART_Transmit("\n");
    }
}
    
void set_writeData(uint16_t _page, uint8_t _address, uint8_t _data)
{
    USART_Transmit("\n\n");
    flash_write(_page, _address, _data);
    USART_Transmit("WriteData: ");
    USART_TransmitHEX(_data);
}
    
void set_writePage(uint16_t _page, uint8_t * _data, uint32_t _length)
{
    USART_Transmit("\n\n");
    flash_writePage(_page, _data, _length);
    USART_Transmit("WritePage: xxxx");
}
