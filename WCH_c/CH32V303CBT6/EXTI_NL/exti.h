
#ifndef __EXTI_H_
#define __EXTI_H_

#include "stdio.h"
#include <stdint.h>
#include "defines.h"
#include "irqn.h"


// Список функций
void EXTI2_Init(void);


// Инициализация EXTI, NVIC, GPIO 
void EXTI2_Init(void)
{
    uint32_t tmp;

    // включаем тактирование порта A и альтернативных функций
    RCC->APB2PCENR |= RCC_APB2PCENR_IOPAEN | RCC_APB2PCENR_AFIOEN;
    // PA2
    GPIOA->CFGLR &= ~(GPIO_Msk << PA2*4); // ~(0b1111);
    GPIOA->CFGLR |= (GPIO_IPU << PA2*4); // 0b1000;
    GPIOA->BSHR |= (1 << PA2);

    /* GPIOA ----> EXTI_Line2 */
    tmp = 0x00;

    tmp = ((uint32_t)0x0F) << (0x04 * (GPIO_PinSource2 & (uint8_t)0x03));
    AFIO->EXTICR[GPIO_PinSource2 >> 0x02] &= ~tmp;
    AFIO->EXTICR[GPIO_PinSource2 >> 0x02] |= (((uint32_t)GPIO_PortSourceGPIOA) << (0x04 * (GPIO_PinSource2 & (uint8_t)0x03)));

    tmp = 0;

    tmp = (uint32_t)EXTI_BASE;
    EXTI->INTENR &= ~EXTI_Line2;
    EXTI->EVENR &= ~EXTI_Line2;
    tmp += EXTI_Mode_Interrupt;
    *(__IO uint32_t *)tmp |= EXTI_Line2;

    tmp = (uint32_t)EXTI_BASE;
    EXTI->RTENR &= ~EXTI_Line2;
    EXTI->FTENR &= ~EXTI_Line2;    
    tmp += EXTI_Trigger_Falling;
    *(__IO uint32_t *)tmp |= EXTI_Line2;
        

    uint8_t NVIC_IRQChannelSubPriority = 2;
    NVIC->IPRIOR[(uint32_t)(EXTI2_IRQn)] = (NVIC_IRQChannelSubPriority << 4);
    NVIC->IENR[((uint32_t)(EXTI2_IRQn) >> 5)] = (1 << ((uint32_t)(EXTI2_IRQn) & 0x1F));
}




#endif /* __EXTI_H_ */
