

#include "spi.h"


// Инициализация SPI1
void SPI1_Master_Init(uint8_t mode)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    SPI_InitTypeDef  SPI_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1, ENABLE);
    // PA2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_SetBits(GPIOA, GPIO_Pin_2);
    // PA3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_SetBits(GPIOA, GPIO_Pin_3);
    // PA4
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_SetBits(GPIOA, GPIO_Pin_4);
    // PA5
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    // PA6
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    // PA7
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    
    if (mode == 0) // Mode 0 (CPOL = 0, CPHA = 0)
    {
        SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
        SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    }
    if (mode == 1) // Mode 1 (CPOL = 0, CPHA = 1)
    {
        SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
        SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    }
    if (mode == 2) // Mode 2 (CPOL = 1, CPHA = 0)
    {
        SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
        SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    }
    if (mode == 3) // Mode 3 (CPOL = 1, CPHA = 1)
    {
        SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
        SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    }

    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI1, &SPI_InitStructure);

    SPI_Cmd(SPI1, ENABLE);
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


// отправка данных
uint8_t SPI1_Send(uint8_t data)
{
    u8 i = 0;

    while(SPI1_GetFlagStatus(SPI_I2S_FLAG_TXE) == RESET)
    {
        i++;
        if(i > 200)
            return 0;
    }

    SPI1_SendData(data);

    return 1;
}


// приём данных
uint8_t SPI1_Receive(void)
{
    u8 i = 0;

    while(SPI1_GetFlagStatus(SPI_I2S_FLAG_RXNE) == RESET);
    {
        i++;
        if(i > 200)
            return 0;
    }

    return SPI1_ReceiveData();
}


// приёмопередача данных
uint8_t SPI1_Transmit(uint8_t data)
{
    SPI1_Send(data);

    return SPI1_Receive();
}

