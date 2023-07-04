
#ifndef __SPI_H_
#define __SPI_H_

 // НЕ RGB А RBG !!!
 
/*
 pins:
    CS   -- PA2
    RES  -- PA3
    DC   -- PA4
    CLK  -- PA5
    MISO -- PA6
    MOSI -- PA7
*/

#include "defines.h"
#include "system.h"


/* Список функций */
void SPI1_Master_Init(uint8_t mode);
void SPI1_SendData(uint16_t Data);
uint16_t SPI1_ReceiveData(void);
uint8_t SPI1_GetFlagStatus(uint16_t SPI_FLAG);


#define DD_CS 		(1 << PA2)
#define DD_RES 		(1 << PA3)
#define DD_DC 		(1 << PA4)
#define DD_CLK 		(1 << PA5)
#define DD_MISO 	(1 << PA6)
#define DD_MOSI 	(1 << PA7)


// Инициализация SPI1
void SPI1_Master_Init(uint8_t mode)
{
    uint16_t tmpreg = 0;

    // включаем тактирование порта A
    RCC->APB2PCENR |= RCC_APB2PCENR_IOPAEN;
    
    // PA2
    GPIOA->CFGLR &= ~(GPIO_Msk << PA2*4); // ~(0b1111);
    GPIOA->CFGLR |= (GPIO_Speed_50 << PA2*4); // 0b0011;
    GPIOA->BSHR |= DD_CS; // (1 << PA2);
    // PA3
    GPIOA->CFGLR &= ~(GPIO_Msk << PA3*4); // ~(0b1111);
    GPIOA->CFGLR |= (GPIO_Speed_50 << PA3*4); // 0b0011;
    GPIOA->BSHR |= DD_RES; // (1 << PA3);
    // PA4
    GPIOA->CFGLR &= ~(GPIO_Msk << PA4*4); // ~(0b1111);
    GPIOA->CFGLR |= (GPIO_Speed_50 << PA4*4); // 0b0011;
    GPIOA->BSHR |= DD_DC; // (1 << PA4);
    // PA5
    GPIOA->CFGLR &= ~(GPIO_Msk << PA5*4); // ~(0b1111);
    GPIOA->CFGLR |= (GPIO_AF_50 << PA5*4); // 0b1011;
    // PA6
    GPIOA->CFGLR &= ~(GPIO_Msk << PA6*4); // ~(0b1111);
    GPIOA->CFGLR |= (GPIO_IN_FLOAT << PA6*4); // 0b0100;
    // PA7
    GPIOA->CFGLR &= ~(GPIO_Msk << PA7*4); // ~(0b1111);
    GPIOA->CFGLR |= (GPIO_AF_50 << PA7*4); // 0b1011;

    // включаем тактирование SPI1
    RCC->APB2PCENR |= RCC_APB2PCENR_SPI1EN;

    tmpreg = SPI1->CTLR1;
    tmpreg &= CTLR1_CLEAR_Mask;
    tmpreg |= SPI_Direction_2Lines_FullDuplex | SPI_Mode_Master | SPI_DataSize_8b |
                SPI_NSS_Soft | SPI_BaudRatePrescaler_8 | SPI_FirstBit_MSB;
    if (mode == 0) tmpreg |= SPI_CPOL_Low | SPI_CPHA_1Edge;  // Mode 0 (CPOL = 0, CPHA = 0)
    if (mode == 1) tmpreg |= SPI_CPOL_Low | SPI_CPHA_2Edge;  // Mode 1 (CPOL = 0, CPHA = 1)
    if (mode == 2) tmpreg |= SPI_CPOL_High | SPI_CPHA_1Edge; // Mode 2 (CPOL = 1, CPHA = 0)
    if (mode == 3) tmpreg |= SPI_CPOL_High | SPI_CPHA_2Edge; // Mode 3 (CPOL = 1, CPHA = 1)
    SPI1->CTLR1 = tmpreg;

    SPI1->I2SCFGR &= SPI_Mode_Select;
    SPI1->CRCR = 7; // SPI_CRCPolynomial

    // включаем SPI1
    SPI1->CTLR1 |= CTLR1_SPE_Set;
}


// передача данных
void SPI1_SendData(uint16_t Data)
{
    SPI1->DATAR = Data;
}


// приём данных
uint16_t SPI1_ReceiveData(void)
{
    return SPI1->DATAR;
}


// получение статус флага
uint8_t SPI1_GetFlagStatus(uint16_t SPI_FLAG)
{
    if((SPI1->STATR & SPI_FLAG) != (uint16_t)RESET)
    {
        return SET;
    }
    else
    {
        return RESET;
    }
}


#endif /* __SPI_H_ */
