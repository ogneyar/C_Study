
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
// void USART_Printf_Init(uint32_t baudrate);


static uint8_t  p_us = 0;
static uint16_t p_ms = 0;


//
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


// //
// void USART_Printf_Init(uint32_t baudrate)
// {
//     GPIO_InitTypeDef  GPIO_InitStructure;
//     USART_InitTypeDef USART_InitStructure;

//     RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

//     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
//     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//     GPIO_Init(GPIOA, &GPIO_InitStructure);

//     USART_InitStructure.USART_BaudRate = baudrate;
//     USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//     USART_InitStructure.USART_StopBits = USART_StopBits_1;
//     USART_InitStructure.USART_Parity = USART_Parity_No;
//     USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//     USART_InitStructure.USART_Mode = USART_Mode_Tx;

//     USART_Init(USART1, &USART_InitStructure);
//     USART_Cmd(USART1, ENABLE);
// }

// // Функция, для работы Printf
// __attribute__((used)) int _write(int fd, char *buf, int size)
// {
//     int i;

//     for(i = 0; i < size; i++)
//     {
//         while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
//         USART_SendData(USART1, *buf++);
//     }

//     return size;
// }

#endif /* __DELAY_H_ */
