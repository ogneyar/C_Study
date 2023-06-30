
#ifndef __USART_H_
#define __USART_H_

#include "stdio.h"
#include <stdint.h>
#include "defines.h"
#include "system.h"

// Список функций
void USART1_Init(uint32_t baudrate);
void USART1_SendData(uint16_t Data);
uint16_t USART1_ReceiveData(void);
uint8_t USART1_GetFlagStatus(uint16_t USART_FLAG);


// Инициализация USART (при тактировании от внутреннего осцилятора и PLL = 6 => 48MHz или PLL = 18 => 144MHz)
void USART1_Init(uint32_t baudrate)
{
    uint32_t apbclock = SYSCLK_FREQ; // PCLK2_Frequency (APB) == SYSCLK_Frequency

    uint32_t tmpreg = 0x00;
    uint32_t integerdivider = 0x00;
    uint32_t fractionaldivider = 0x00;

    // включаем тактирование порта A
    RCC->APB2PCENR |= RCC_APB2PCENR_IOPAEN;
    // PA9
    GPIOA->CFGHR &= ~(GPIO_Msk << (PA9-8)*4); // ~(0b1111);
    GPIOA->CFGHR |= (GPIO_AF_50 << (PA9-8)*4); // 0b1011;

    // включаем тактирование USART1
    RCC->APB2PCENR |= RCC_APB2PCENR_USART1EN;
    
    USART1->CTLR1 &= CTLR1_CLEAR_Mask;
    // USART1->CTLR1 |= USART_WordLength_8b | USART_Parity_No;
    USART1->CTLR1 |= USART_Mode_Tx;

    // USART1->CTLR2 &= CTLR2_STOP_CLEAR_Mask;
    // USART1->CTLR2 |= USART_StopBits_1;

    // USART1->CTLR3 &= CTLR3_CLEAR_Mask;
    // USART1->CTLR3 |= USART_HardwareFlowControl_None;

    integerdivider = ((25 * apbclock) / (4 * (baudrate)));
    tmpreg = (integerdivider / 100) << 4;
    fractionaldivider = integerdivider - (100 * (tmpreg >> 4));
    tmpreg |= ((((fractionaldivider * 16) + 50) / 100)) & ((uint8_t)0x0F);

    USART1->BRR = (uint16_t)tmpreg;
    
    // USART1 ENABLE
    USART1->CTLR1 |= CTLR1_UE_Set;
}


// отправка данных
void USART1_SendData(uint16_t Data)
{
    USART1->DATAR = (Data & (uint16_t)0x01FF);
}


// приём данных
uint16_t USART1_ReceiveData(void)
{
    return (uint16_t)(USART1->DATAR & (uint16_t)0x01FF);
}


// получить флаг
uint8_t USART1_GetFlagStatus(uint16_t USART_FLAG)
{
    uint8_t bitstatus = RESET;

    if((USART1->STATR & USART_FLAG) != (uint16_t)RESET)
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }
    return bitstatus;
}


// Функция, для работы Printf
__attribute__((used)) int _write(int fd, char *buf, int size)
{
    int i;

    for(i = 0; i < size; i++)
    {
        while(USART1_GetFlagStatus(USART_FLAG_TC) == RESET);
        USART1_SendData(*buf++);
    }

    return size;
}

// Функция, для работы Printf
__attribute__((used)) void *_sbrk(ptrdiff_t incr)
{
    extern char _end[];
    extern char _heap_end[];
    static char *curbrk = _end;

    if ((curbrk + incr < _end) || (curbrk + incr > _heap_end))
    return NULL - 1;

    curbrk += incr;
    return curbrk - incr;
}



#endif /* __USART_H_ */
