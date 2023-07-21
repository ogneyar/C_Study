
#ifndef __RCC_H_
#define __RCC_H_

#include "stdio.h"
#include <stdint.h>
#include "defines.h"


// Список функций
uint8_t RCC_GetFlagStatus(uint8_t RCC_FLAG);
void RCC_LSEConfig(uint8_t RCC_LSE);


//
uint8_t RCC_GetFlagStatus(uint8_t RCC_FLAG)
{
    uint32_t tmp = 0;
    uint32_t statusreg = 0;

    uint8_t bitstatus = RESET;
    tmp = RCC_FLAG >> 5;

    if(tmp == 1)
    {
        statusreg = RCC->CTLR;
    }
    else if(tmp == 2)
    {
        statusreg = RCC->BDCTLR;
    }
    else
    {
        statusreg = RCC->RSTSCKR;
    }

    tmp = RCC_FLAG & RCC_FLAG_Mask;

    if((statusreg & ((uint32_t)1 << tmp)) != (uint32_t)RESET)
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }

    return bitstatus;
}

//
void RCC_LSEConfig(uint8_t RCC_LSE)
{
    *(__IO uint8_t *)BDCTLR_ADDRESS = RCC_LSE_OFF;
    *(__IO uint8_t *)BDCTLR_ADDRESS = RCC_LSE_OFF;

    switch(RCC_LSE)
    {
        case RCC_LSE_ON:
            *(__IO uint8_t *)BDCTLR_ADDRESS = RCC_LSE_ON;
            break;

        case RCC_LSE_Bypass:
            *(__IO uint8_t *)BDCTLR_ADDRESS = RCC_LSE_Bypass | RCC_LSE_ON;
            break;

        default:
            break;
    }
}


#endif /* __RCC_H_ */
