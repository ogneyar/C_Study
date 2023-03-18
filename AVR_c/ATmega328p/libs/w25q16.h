
#ifndef _W25Q16_H_
#define _W25Q16_H_


#include <avr/io.h>
#ifndef _AVR_IOXXX_H_
#include <avr/iom328p.h>
#endif 
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "spi.h"

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define W25_ENABLE_RESET 0x66
#define W25_RESET 0x99
#define W25_WRITE_ENABLE 0x06
#define W25_WRITE_DISABLE 0x04
#define W25_PAGE_PROGRAM 0x02
#define W25_READ_STATUS_REGISTER_1 0x05
#define W25_READ_STATUS_REGISTER_2 0x35
#define W25_READ_STATUS_REGISTER_3 0x15
#define W25_STATUS_REGISTER_WE_1 0x50 // SR Write Enable 
#define W25_WRITE_STATUS_REGISTER_1 0x01
#define W25_WRITE_STATUS_REGISTER_2 0x31
#define W25_WRITE_STATUS_REGISTER_3 0x11
#define W25_READ_DATA 0x03
#define W25_SECTOR_ERASE_4K 0x20
#define W25_BLOCK_ERASE_32K 0x52
#define W25_BLOCK_ERASE_64K 0xd8
#define W25_CHIP_ERASE 0xC7
#define W25_POWER_DOWN 0xB9
#define W25_RELEASE_POWER_DOWN 0xAB
#define W25_MANUFACTURER_ID 0x90
#define W25_READ_UNIQUE_ID 0x4B
#define W25_JEDEC_ID 0x9F

#define cs_set() PORT_SPI &= ~(1 << DD_SS)
#define cs_reset() PORT_SPI |= (1 << DD_SS)


void flash_init(void);
uint8_t flash_read(uint16_t page, uint8_t pageAddress);
void flash_readPage(uint16_t page, uint8_t pageAddress, uint8_t *data);
void flash_write(uint16_t page, uint8_t pageAddress, uint8_t val);
void flash_writePage(uint16_t page, uint8_t pageAddress, uint8_t *data);
uint8_t flash_manufacturerID(void);
uint8_t flash_deviceID(void);
uint8_t flash_uniqueID(void);
uint16_t flash_jedecID(void);
void flash_chipErase(void);
void flash_sectorErase(uint16_t sector);
void flash_powerDown(void);
void flash_releasePowerDown(void);
void flash_notBusy(void);
void flash_writeEnable(void);
void flash_writeDisable(void);
void flash_setBlockProtect(uint8_t val);
uint8_t flash_readStatusRegister1(void);


// Функция инициализации памяти
void flash_init(void) 
{
    SPI_Init(1); // mode = 1
	cs_set();
    _delay_ms(100);
    flash_reset();
    _delay_ms(100);
	flash_releasePowerDown();
	flash_writeDisable();
    cs_reset();
}

void flash_reset(void)
{
    cs_set();
    SPI_Transmit(W25_ENABLE_RESET);
    SPI_Transmit(W25_RESET);
    flash_notBusy();
    cs_reset();
}

uint8_t flash_read(uint16_t page, uint8_t pageAddress)
{
    cs_set();
    SPI_Transmit(W25_READ_DATA);
    SPI_Transmit((page >> 8) & 0xFF);
    SPI_Transmit((page >> 0) & 0xFF);
    SPI_Transmit(pageAddress);
    uint8_t val = SPI_Transfer(0);
    cs_reset();
    flash_notBusy();
    return val;
}

// Чтение страницы
void flash_readPage(uint16_t page, uint8_t pageAddress, uint8_t *data)
{    
    cs_set();
    SPI_Transmit(W25_READ_DATA);
    SPI_Transmit((page >> 8) & 0xFF);
    SPI_Transmit((page >> 0) & 0xFF);
    SPI_Transmit(pageAddress);
    for(int i = 0; i < 256; i++)
    {
        data[i] = SPI_Transfer(0);
    }
    cs_reset();
    flash_notBusy();
}

void flash_write(uint16_t page, uint8_t pageAddress, uint8_t val)
{
    // flash_notBusy();
    flash_setBlockProtect(0x00);
    flash_writeEnable();
    cs_set();
    SPI_Transmit(W25_PAGE_PROGRAM);
    SPI_Transmit((page >> 8) & 0xFF);
    SPI_Transmit((page >> 0) & 0xFF);
    SPI_Transmit(pageAddress);
    SPI_Transmit(val);
    cs_reset();
    flash_notBusy();
    flash_writeDisable();
    flash_setBlockProtect(0x0F);
}


void flash_writePage(uint16_t page, uint8_t pageAddress, uint8_t *data)
{      
    flash_setBlockProtect(0x00);
    flash_writeEnable();
    cs_set();
    SPI_Transmit(W25_PAGE_PROGRAM);
    SPI_Transmit((page >> 8) & 0xFF);
    SPI_Transmit((page >> 0) & 0xFF);
    SPI_Transmit(pageAddress);
    
    for(uint16_t i = 0; i < 256; i++)
    {
        SPI_Transmit(data++);
    }
    cs_reset();
    flash_notBusy();
    flash_writeDisable();
    flash_setBlockProtect(0x0F);
}

uint8_t flash_manufacturerID(void)
{
    cs_set();
    SPI_Transmit(W25_MANUFACTURER_ID);
    SPI_Transmit(0);
    SPI_Transmit(0);
    SPI_Transmit(0);
    uint8_t val = SPI_Transfer(0);
    cs_reset();
    flash_notBusy();
    return val;
}

uint8_t flash_deviceID(void)
{
    cs_set();
    SPI_Transmit(W25_MANUFACTURER_ID);
    SPI_Transmit(0);
    SPI_Transmit(0);
    SPI_Transmit(0);
    SPI_Transmit(0);
    uint8_t val = SPI_Transfer(0);
    cs_reset();
    flash_notBusy();
    return val;
}

uint8_t flash_uniqueID(void)
{
    cs_set();
    SPI_Transmit(W25_READ_UNIQUE_ID);
    SPI_Transmit(0);
    SPI_Transmit(0);
    SPI_Transmit(0);
    SPI_Transmit(0);
    uint8_t val = SPI_Transfer(0);
    cs_reset();
    flash_notBusy();
    return val;
}

uint16_t flash_jedecID(void)
{
    flash_notBusy();
    cs_set();
    SPI_Transmit(W25_JEDEC_ID);
    uint8_t val1 = SPI_Transfer(0);
    uint8_t val2 = SPI_Transfer(0);
    uint8_t val3 = SPI_Transfer(0);
    cs_reset();
    flash_notBusy();
    // return (val1<<16)|(val2<<8)|(val3);
    return (val2<<8)|(val3);
}

void flash_chipErase(void)
{
    // flash_notBusy();
    flash_setBlockProtect(0x00);
    flash_writeEnable();
    cs_set();
    SPI_Transmit(W25_CHIP_ERASE);
    cs_reset();
    flash_notBusy();
    flash_writeDisable();
    flash_setBlockProtect(0x0F);
}

// Очистка сектора
void flash_sectorErase(uint16_t sector)
{
    flash_setBlockProtect(0x00);
  
    sector = (sector << 1); 

    cs_set();
    // Передать команду и адрес страницы
    SPI_Transmit(W25_WRITE_ENABLE);
    cs_reset();

    cs_set();
    // Передать команду и адрес страницы
    SPI_Transmit(W25_SECTOR_ERASE_4K);
    SPI_Transmit((uint8_t)(sector >> 8));
    SPI_Transmit((uint8_t)(sector & 0xFF));
    SPI_Transmit(0x00);
    cs_reset();

    flash_notBusy();
      
    cs_set();
    SPI_Transmit(W25_WRITE_DISABLE);
    cs_reset();

    flash_setBlockProtect(0x0F);
}

void flash_powerDown(void)
{
    cs_set();
    SPI_Transmit(W25_POWER_DOWN);
    cs_reset();
    flash_notBusy();
}

void flash_releasePowerDown(void)
{
    cs_set();
    SPI_Transmit(W25_RELEASE_POWER_DOWN);
    cs_reset();
    flash_notBusy();
}

void flash_notBusy(void)
{
    uint8_t data;
    do {
        cs_set();
        SPI_Transmit(W25_READ_STATUS_REGISTER_1);
        data = SPI_Transfer(0);
        cs_reset();
    }while (data & 0x01);
}

void flash_writeEnable(void)
{
    flash_notBusy();
    cs_set();
    SPI_Transmit(W25_WRITE_ENABLE);
    cs_reset();
    flash_notBusy();
}

void flash_writeDisable(void)
{
    flash_notBusy();
    cs_set();
    SPI_Transmit(W25_WRITE_DISABLE);
    cs_reset();
    flash_notBusy();
}

void flash_setBlockProtect(uint8_t val)
{
    flash_notBusy(); 
    flash_writeEnable();
    cs_set();
    SPI_Transmit(W25_STATUS_REGISTER_WE_1);
    cs_reset();
    cs_set();
    SPI_Transmit(W25_WRITE_STATUS_REGISTER_1);
    SPI_Transmit((val & 0x0F) << 2);
    cs_reset();
    flash_writeDisable();
    flash_notBusy(); 
}

uint8_t flash_readStatusRegister1(void)
{
    uint8_t data;
    cs_set();
    SPI_Transmit(W25_READ_STATUS_REGISTER_1);
    data = SPI_Transfer(0);
    cs_reset();
    return data;
}

void flash_writeStatusRegister1(uint8_t data)
{
    flash_notBusy(); 
    flash_writeEnable();
    cs_set();
    SPI_Transmit(W25_STATUS_REGISTER_WE_1);
    cs_reset();
    cs_set();
    SPI_Transmit(W25_WRITE_STATUS_REGISTER_1);
    SPI_Transmit(data);
    cs_reset();
    flash_writeDisable();
    flash_notBusy(); 
}

#endif // _W25Q16_H_
