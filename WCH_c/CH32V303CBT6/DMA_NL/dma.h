
#ifndef __DMA_H_
#define __DMA_H_

#include "stdio.h"
#include <stdint.h>
#include "defines.h"


// Список функций
void DMA1_CH3_Init(void);
uint8_t BufCmp(uint32_t *buf1, uint32_t *buf2, uint16_t buflength);



// Инициализация третьего канала первого DMA
void DMA1_CH3_Init(void)
{
    // DMA_InitTypeDef DMA_InitStructure = {0};
    // RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    // DMA_StructInit(&DMA_InitStructure);
    // DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(SRC_BUF);
    // DMA_InitStructure.DMA_MemoryBaseAddr = (u32)DST_BUF;
    // DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    // DMA_InitStructure.DMA_BufferSize = Buf_Size * 4;
    // DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
    // DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    // DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    // DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
    // DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    // DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    // DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;
    // DMA_Init(DMA1_Channel3, &DMA_InitStructure);
    // DMA_ClearFlag(DMA1_FLAG_TC3);

    // DMA_Cmd(DMA1_Channel3, ENABLE);
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
