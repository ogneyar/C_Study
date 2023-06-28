
#ifndef __DELAY_H_
#define __DELAY_H_

#include "stdio.h"
#include <stdint.h>
#include "defines.h"
#include "system.h"

// Список функций
void Delay_Init(void);
void Delay(uint32_t n, uint8_t p);
void Delay_Us (uint32_t n);
void Delay_Ms (uint32_t n);


static uint8_t  p_us = 0;
static uint16_t p_ms = 0;


// Инициализация задержки
void Delay_Init(void)
{
    p_us = SystemCoreClock / 8000000;
    p_ms = (uint16_t)p_us * 1000;
}


// Задержка
void Delay(uint32_t n, uint8_t p) // p = 1 => p_ms; p = 0 => p_us;
{
    uint32_t i;
    uint16_t puls;

    if (p) puls = (uint16_t)p_ms;
    else puls = (uint16_t)p_us;

    SysTick->SR &= ~(1 << 0);
    i = (uint32_t)n * puls;

    SysTick->CMP = i;
    SysTick->CTLR |= (1 << 4);
    SysTick->CTLR |= (1 << 5) | (1 << 0);

    while((SysTick->SR & (1 << 0)) != (1 << 0))
        ;
    SysTick->CTLR &= ~(1 << 0);
}


// Задержка в микросекундах (1 секунда = 1 000 000 мкс)
void Delay_Us(uint32_t n)
{
    Delay(n, 0);
}


// Задержка в милисекундах (1 секунда = 1 000 мс)
void Delay_Ms(uint32_t n)
{
    Delay(n, 1);
}


#endif /* __DELAY_H_ */
