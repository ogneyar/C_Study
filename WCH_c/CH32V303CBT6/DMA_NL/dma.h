
#ifndef __DMA_H_
#define __DMA_H_

#include "stdio.h"
#include <stdint.h>
#include "defines.h"


// Список функций
void DMA1_CH3_Init(uint32_t *SRC_BUF, uint32_t *DST_BUF, uint8_t Buf_Size);
void DMA_ClearFlag(uint32_t DMAy_FLAG);
uint8_t DMA_GetFlagStatus(uint32_t DMAy_FLAG);
uint8_t BufCmp(uint32_t *buf1, uint32_t *buf2, uint16_t buflength);



// Инициализация третьего канала первого DMA
void DMA1_CH3_Init(uint32_t *SRC_BUF, uint32_t *DST_BUF, uint8_t Buf_Size)
{
    RCC->AHBPCENR |= RCC_AHBPCENR_DMA1EN;

    uint32_t tmpreg = 0;
    tmpreg = DMA1_Channel3->CFGR;
    tmpreg &= CFGR_CLEAR_Mask;
    tmpreg |= DMA_DIR_PeripheralSRC | DMA_Mode_Normal | DMA_PeripheralInc_Enable | DMA_MemoryInc_Enable |
              DMA_PeripheralDataSize_Byte | DMA_Priority_VeryHigh | DMA_M2M_Enable;
    DMA1_Channel3->CFGR = tmpreg;
    DMA1_Channel3->CNTR = Buf_Size * 4;
    DMA1_Channel3->PADDR = (u32)(SRC_BUF);
    DMA1_Channel3->MADDR = (u32)DST_BUF;

    DMA_ClearFlag(DMA1_FLAG_TC3);

    DMA1_Channel3->CFGR |= DMA_CFGR1_EN;
}


// Очистка флага
void DMA_ClearFlag(uint32_t DMAy_FLAG)
{
    if((DMAy_FLAG & DMA2_FLAG_Mask) == DMA2_FLAG_Mask)
    {
        DMA2->INTFCR = DMAy_FLAG;
    }
    else if((DMAy_FLAG & DMA2_EXTEN_FLAG_Mask) == DMA2_EXTEN_FLAG_Mask)
    {
        DMA2_EXTEN->INTFCR = DMAy_FLAG;
    }
    else
    {
        DMA1->INTFCR = DMAy_FLAG;
    }
}


// Получение статуса
uint8_t DMA_GetFlagStatus(uint32_t DMAy_FLAG)
{
    uint8_t bitstatus = RESET;
    uint32_t tmpreg = 0;

    if((DMAy_FLAG & DMA2_FLAG_Mask) == DMA2_FLAG_Mask)
    {
        tmpreg = DMA2->INTFR;
    }
    else if((DMAy_FLAG & DMA2_EXTEN_FLAG_Mask) == DMA2_EXTEN_FLAG_Mask)
    {
        tmpreg = DMA2_EXTEN->INTFR;
    }
    else
    {
        tmpreg = DMA1->INTFR;
    }

    if((tmpreg & DMAy_FLAG) != (uint32_t)RESET)
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }

    return bitstatus;
}


// Сравнение двух буферов
uint8_t BufCmp(uint32_t *buf1, uint32_t *buf2, uint16_t buflength)
{
    while(buflength--)
    {
        if(*buf1 != *buf2)
        {
            return 0;
        }
        buf1++;
        buf2++;
    }
    return 1;
}




#endif /* __DMA_H_ */
