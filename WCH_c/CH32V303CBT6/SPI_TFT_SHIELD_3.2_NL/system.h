
#ifndef __SYSTEM_H_
#define __SYSTEM_H_

#include <stdint.h>
#include "defines.h" 

extern uint32_t SystemCoreClock;

// Список функций 
extern void SystemInit(void);
static void SetSysClockTo48_HSI(void);
static void SetSysClockTo144_HSI(void);


// #define SYSCLK_FREQ            HSI_VALUE
// #define SYSCLK_FREQ            SYSCLK_FREQ_48MHz_HSI
#define SYSCLK_FREQ            SYSCLK_FREQ_144MHz_HSI


uint32_t SystemCoreClock = SYSCLK_FREQ;

__I uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};


//
void SystemInit (void)
{
    RCC->CTLR |= (uint32_t)0x00000001;
    RCC->CFGR0 &= (uint32_t)0xF0FF0000;
    RCC->CTLR &= (uint32_t)0xFEF6FFFF;
    RCC->CTLR &= (uint32_t)0xFFFBFFFF;
    RCC->CFGR0 &= (uint32_t)0xFF80FFFF;
    RCC->INTR = 0x009F0000;   
    
    if (SystemCoreClock == SYSCLK_FREQ_48MHz_HSI)
        SetSysClockTo48_HSI();
    else if (SystemCoreClock == SYSCLK_FREQ_144MHz_HSI)
        SetSysClockTo144_HSI();
}


//
static void SetSysClockTo48_HSI(void)
{
    EXTEN->EXTEN_CTR |= EXTEN_PLL_HSI_PRE;

    // HCLK = SYSCLK
    RCC->CFGR0 |= (uint32_t)RCC_HPRE_DIV1;
    // PCLK2 = HCLK
    RCC->CFGR0 |= (uint32_t)RCC_PPRE2_DIV1;
    // PCLK1 = HCLK
    RCC->CFGR0 |= (uint32_t)RCC_PPRE1_DIV2;

    //  PLL configuration: PLLCLK = HSI * 6 = 48 MHz
    RCC->CFGR0 &= (uint32_t)((uint32_t)~(RCC_PLLSRC | RCC_PLLXTPRE | RCC_PLLMULL));

    RCC->CFGR0 |= (uint32_t)(RCC_PLLSRC_HSI_Div2 | RCC_PLLMULL6);

    // Enable PLL
    RCC->CTLR |= RCC_PLLON;
    // Wait till PLL is ready
    while((RCC->CTLR & RCC_PLLRDY) == 0)
        ;
    // Select PLL as system clock source
    RCC->CFGR0 &= (uint32_t)((uint32_t)~(RCC_SW));
    RCC->CFGR0 |= (uint32_t)RCC_SW_PLL;
    // Wait till PLL is used as system clock source
    while ((RCC->CFGR0 & (uint32_t)RCC_SWS) != (uint32_t)0x08)
        ;
}


//
static void SetSysClockTo144_HSI(void)
{
    EXTEN->EXTEN_CTR |= EXTEN_PLL_HSI_PRE;

    /* HCLK = SYSCLK */
    RCC->CFGR0 |= (uint32_t)RCC_HPRE_DIV1;
    /* PCLK2 = HCLK */
    RCC->CFGR0 |= (uint32_t)RCC_PPRE2_DIV1;
    /* PCLK1 = HCLK */
    RCC->CFGR0 |= (uint32_t)RCC_PPRE1_DIV2;

    /*  PLL configuration: PLLCLK = HSI * 18 = 144 MHz */
    RCC->CFGR0 &= (uint32_t)((uint32_t)~(RCC_PLLSRC | RCC_PLLXTPRE | RCC_PLLMULL));

    RCC->CFGR0 |= (uint32_t)(RCC_PLLSRC_HSI_Div2 | RCC_PLLMULL18);

    /* Enable PLL */
    RCC->CTLR |= RCC_PLLON;
    /* Wait till PLL is ready */
    while((RCC->CTLR & RCC_PLLRDY) == 0)
        ;
    /* Select PLL as system clock source */
    RCC->CFGR0 &= (uint32_t)((uint32_t)~(RCC_SW));
    RCC->CFGR0 |= (uint32_t)RCC_SW_PLL;
    /* Wait till PLL is used as system clock source */
    while ((RCC->CFGR0 & (uint32_t)RCC_SWS) != (uint32_t)0x08)
        ;
}


#endif /*__SYSTEM_H_ */



