
#ifndef __DAC_H_
#define __DAC_H_

#include "stdio.h"
#include <stdint.h>
#include "defines.h"


// Список функций
void DAC_Init(void);
void DAC_SetChannel1Data(uint32_t DAC_Align, uint16_t Data);


// Инициализация DAC
void DAC_Init(void)
{
    uint32_t tmpreg1 = 0, tmpreg2 = 0;

    // включаем тактирование порта A
    RCC->APB2PCENR |= RCC_APB2PCENR_IOPAEN;
    // PA4
    GPIOA->CFGLR &= ~(GPIO_Msk << PA4*4); // ~(0b1111);
    GPIOA->BSHR |= (1 << PA4);

    // включаем тактирование DAC
    RCC->APB1PCENR |= RCC_APB1PCENR_DACEN;
    
    tmpreg1 = DAC->CTLR;
    tmpreg1 &= ~(CTLR_CLEAR_MASK << DAC_Channel_1);
    tmpreg2 = (DAC_Trigger_None | DAC_WaveGeneration_None | DAC_LFSRUnmask_Bit0 | DAC_OutputBuffer_Disable);
    tmpreg1 |= tmpreg2 << DAC_Channel_1;
    DAC->CTLR = tmpreg1;

    // включаем DAC
    DAC->CTLR |= (DAC_EN1 << DAC_Channel_1);

    DAC_SetChannel1Data(DAC_Align_12b_R, 0);
}


// установка значения
void DAC_SetChannel1Data(uint32_t DAC_Align, uint16_t Data)
{
    __IO uint32_t tmp = 0;

    tmp = (uint32_t)DAC_BASE;
    tmp += DHR12R1_OFFSET + DAC_Align;

    *(__IO uint32_t *)tmp = Data;
}


#endif /* __DAC_H_ */
