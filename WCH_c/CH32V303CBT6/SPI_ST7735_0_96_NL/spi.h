
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
void SPI1_Master_Init(void);
void SPI1_SendData(uint16_t Data);
uint8_t SPI1_GetFlagStatus(uint16_t SPI_FLAG);


#define DD_CS 		(1 << PA2)
#define DD_RES 		(1 << PA3)
#define DD_DC 		(1 << PA4)
#define DD_CLK 		(1 << PA5)
#define DD_MISO 	(1 << PA6)
#define DD_MOSI 	(1 << PA7)

// SPI or I2S mode selection masks
#define SPI_Mode_Select       ((uint16_t)0xF7FF)
#define I2S_Mode_Select       ((uint16_t)0x0800)

// SPI SPE mask
#define CTLR1_SPE_Set         ((uint16_t)0x0040)
#define CTLR1_SPE_Reset       ((uint16_t)0xFFBF)

// SPI_data_direction  
#define SPI_Direction_2Lines_FullDuplex ((uint16_t)0x0000)
#define SPI_Direction_2Lines_RxOnly     ((uint16_t)0x0400)
#define SPI_Direction_1Line_Rx          ((uint16_t)0x8000)
#define SPI_Direction_1Line_Tx          ((uint16_t)0xC000)

// SPI_mode
#define SPI_Mode_Master                 ((uint16_t)0x0104)
#define SPI_Mode_Slave                  ((uint16_t)0x0000)

// SPI_data_size
#define SPI_DataSize_16b                ((uint16_t)0x0800)
#define SPI_DataSize_8b                 ((uint16_t)0x0000)

// SPI_Clock_Polarity
#define SPI_CPOL_Low                    ((uint16_t)0x0000)
#define SPI_CPOL_High                   ((uint16_t)0x0002)

// SPI_Clock_Phase
#define SPI_CPHA_1Edge                  ((uint16_t)0x0000)
#define SPI_CPHA_2Edge                  ((uint16_t)0x0001)

// SPI_Slave_Select_management
#define SPI_NSS_Soft                    ((uint16_t)0x0200)
#define SPI_NSS_Hard                    ((uint16_t)0x0000)

// SPI_BaudRate_Prescaler
#define SPI_BaudRatePrescaler_2         ((uint16_t)0x0000)
#define SPI_BaudRatePrescaler_4         ((uint16_t)0x0008)
#define SPI_BaudRatePrescaler_8         ((uint16_t)0x0010)
#define SPI_BaudRatePrescaler_16        ((uint16_t)0x0018)
#define SPI_BaudRatePrescaler_32        ((uint16_t)0x0020)
#define SPI_BaudRatePrescaler_64        ((uint16_t)0x0028)
#define SPI_BaudRatePrescaler_128       ((uint16_t)0x0030)
#define SPI_BaudRatePrescaler_256       ((uint16_t)0x0038)

// SPI_MSB_LSB_transmission
#define SPI_FirstBit_MSB                ((uint16_t)0x0000)
#define SPI_FirstBit_LSB                ((uint16_t)0x0080)

// SPI_I2S_flags_definition
#define SPI_I2S_FLAG_RXNE               ((uint16_t)0x0001)
#define SPI_I2S_FLAG_TXE                ((uint16_t)0x0002)
#define I2S_FLAG_CHSIDE                 ((uint16_t)0x0004)
#define I2S_FLAG_UDR                    ((uint16_t)0x0008)
#define SPI_FLAG_CRCERR                 ((uint16_t)0x0010)
#define SPI_FLAG_MODF                   ((uint16_t)0x0020)
#define SPI_I2S_FLAG_OVR                ((uint16_t)0x0040)
#define SPI_I2S_FLAG_BSY                ((uint16_t)0x0080)


// Инициализация SPI1
void SPI1_Master_Init(void)
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
    tmpreg |= (uint16_t)((uint32_t)SPI_Direction_2Lines_FullDuplex | SPI_Mode_Master |
                         SPI_DataSize_8b | SPI_CPOL_High | SPI_CPHA_2Edge | SPI_NSS_Soft |
                         SPI_BaudRatePrescaler_2 | SPI_FirstBit_MSB);
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
