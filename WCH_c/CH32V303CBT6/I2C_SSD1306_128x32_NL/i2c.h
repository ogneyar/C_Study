
#ifndef __I2C_H_
#define __I2C_H_

#include "defines.h"


/* Список функций */
void I2C2_Init(u32 bound, u16 address);
void I2C2_Start(void);
void I2C2_Stop(void);
void I2C2_Send7bitAddress(uint8_t Address, uint8_t I2C_Direction);
void I2C2_SendData(uint8_t Data);
uint8_t I2C2_GetFlagStatus(uint32_t I2C_FLAG);
uint8_t I2C2_CheckEvent(uint32_t I2C_EVENT);



// Инициализация I2C2 (при тактировании от внутреннего осцилятора и PLL = 6 => 48MHz)
void I2C2_Init(u32 bound, u16 address)
{
    uint16_t tmpreg = 0, freqrange = 0;
    uint16_t result = 0x04;
    uint32_t pclk1 = 24000000;  // SYSCLK_Frequency = 48MHz; PCLK1_Frequency = 24MHz;

    // включаем тактирование порта B
    RCC->APB2PCENR |= RCC_APB2PCENR_IOPBEN;
    // PB10
    GPIOB->CFGHR &= ~(GPIO_Msk << (PB10-8)*4); // ~(0b1111);
    GPIOB->CFGHR |= (GPIO_AF_OD_50 << (PB10-8)*4); // 0b1111;
    // PB11
    GPIOB->CFGHR &= ~(GPIO_Msk << (PB11-8)*4); // ~(0b1111);
    GPIOB->CFGHR |= (GPIO_AF_OD_50 << (PB11-8)*4); // 0b1111;
    
    // включаем тактирование I2C2
    RCC->APB1PCENR |= RCC_APB1PCENR_I2C2EN;
    
    tmpreg = I2C2->CTLR2;
    tmpreg &= CTLR2_FREQ_Reset;    
    freqrange = (uint16_t)(pclk1 / 1000000);
    tmpreg |= freqrange;
    I2C2->CTLR2 = tmpreg;

    I2C2->CTLR1 &= CTLR1_PE_Reset;
    tmpreg = 0;

    if(bound <= 100000)
    {
        result = (uint16_t)(pclk1 / (bound << 1));
        if(result < 0x04) result = 0x04;
        tmpreg |= result;
        I2C2->RTR = freqrange + 1;
    }
    else
    {
        result = (uint16_t)(pclk1 / (bound * 3));
        if((result & CKCFGR_CCR_Set) == 0) result |= (uint16_t)0x0001;
        tmpreg |= (uint16_t)(result | CKCFGR_FS_Set);
        I2C2->RTR = (uint16_t)(((freqrange * (uint16_t)300) / (uint16_t)1000) + (uint16_t)1);
    }

    I2C2->CKCFGR = tmpreg;
    I2C2->CTLR1 |= CTLR1_PE_Set;

    tmpreg = I2C2->CTLR1;
    tmpreg &= CTLR1_CLEAR_Mask;
    tmpreg |= (uint16_t)((uint32_t)I2C_Mode_I2C | I2C_Ack_Enable);
    I2C2->CTLR1 = tmpreg;

    I2C2->OADDR1 = (I2C_AcknowledgedAddress_7bit | address);

    // включаем I2C2
	I2C2->CTLR1 |= CTLR1_PE_Set;

    // включаем ACK
	I2C2->CTLR1 |= CTLR1_ACK_Set;
}


// команда СТАРТ
void I2C2_Start(void)
{
    I2C2->CTLR1 |= CTLR1_START_Set;
}


// команда СТОП
void I2C2_Stop(void)
{
    I2C2->CTLR1 |= CTLR1_STOP_Set;
}


// отправка 7ми битного адреса
void I2C2_Send7bitAddress(uint8_t Address, uint8_t I2C_Direction)
{
    if(I2C_Direction != I2C_Direction_Transmitter)
    {
        Address |= OADDR1_ADD0_Set;
    }
    else
    {
        Address &= OADDR1_ADD0_Reset;
    }
    I2C2->DATAR = Address;
}


// отправка данных
void I2C2_SendData(uint8_t Data)
{
    I2C2->DATAR = Data;
}


// получение статус флага
uint8_t I2C2_GetFlagStatus(uint32_t I2C_FLAG)
{
    uint8_t bitstatus = RESET;
    __IO uint32_t i2creg = 0, i2cxbase = 0;

    i2cxbase = (uint32_t)I2C2;
    i2creg = I2C_FLAG >> 28;
    I2C_FLAG &= FLAG_Mask;

    if(i2creg != 0)
    {
        i2cxbase += 0x14;
    }
    else
    {
        I2C_FLAG = (uint32_t)(I2C_FLAG >> 16);
        i2cxbase += 0x18;
    }

    if(((*(__IO uint32_t *)i2cxbase) & I2C_FLAG) != (uint32_t)RESET)
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }

    return bitstatus;
}


// проверка события
uint8_t I2C2_CheckEvent(uint32_t I2C_EVENT)
{
    uint32_t lastevent = 0;
    uint32_t flag1 = 0, flag2 = 0;
    uint8_t status = NoREADY;

    flag1 = I2C2->STAR1;
    flag2 = I2C2->STAR2;
    flag2 = flag2 << 16;

    lastevent = (flag1 | flag2) & FLAG_Mask;

    if((lastevent & I2C_EVENT) == I2C_EVENT)
    {
        status = READY;
    }
    else
    {
        status = NoREADY;
    }

    return status;
}


#endif /* __I2C_H_ */
