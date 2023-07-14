
#include <stdint.h>
#include "main.h"
#include "defines.h"
#include "system.h"
#include "delay.h"
#include "usart.h"
#include "exti.h"


// Основная функция
int main(void)
{
    Delay_Init();
    GPIOB_Leds_Init();
    USART1_Init(115200);
    printf("SystemClk:%d\r\n", SystemCoreClock);
    printf("EXTI NoLib Test\r\n");
    
    EXTI2_Init();

    while(1)
    {    
        Leds_Blink(333);
        Leds_Blink(333);
        Leds_Blink(333);
        printf("Run at main\r\n");
    }
}

// Инициализация PA2, PB3, PB4, PB5
void GPIOB_Leds_Init(void)
{
    // включаем тактирование порта B
    RCC->APB2PCENR |= RCC_APB2PCENR_IOPBEN;
    // PB3
    GPIOB->CFGLR &= ~(GPIO_Msk << LED_R*4); // ~(0b1111);
    GPIOB->CFGLR |= (GPIO_Speed_50 << LED_R*4); // 0b0011;
    // PB4
    GPIOB->CFGLR &= ~(GPIO_Msk << LED_G*4); // ~(0b1111);
    GPIOB->CFGLR |= (GPIO_Speed_50 << LED_G*4); // 0b0011;
    GPIOB->BSHR |= (1 << LED_G);
    // PB5
    GPIOB->CFGLR &= ~(GPIO_Msk << LED_B*4); // ~(0b1111);
    GPIOB->CFGLR |= (GPIO_Speed_50 << LED_B*4); // 0b0011;
}

// мигание светодиодов
void Leds_Blink(uint16_t delay)
{    
    GPIOB->OUTDR ^= (1 << LED_R);
    GPIOB->OUTDR ^= (1 << LED_G);
    GPIOB->OUTDR ^= (1 << LED_B);
    Delay_Ms(delay);
}


//
void EXTI2_IRQHandler(void)
{
    if(EXTI_GetITStatusLine2()!=RESET)
    {
        printf("Run at EXTI\r\n");
        EXTI->INTFR = EXTI_Line2;     /* Clear Flag */
    }
}


//
uint8_t EXTI_GetITStatusLine2(void)
{
    uint8_t bitstatus = RESET;
    uint32_t enablestatus = 0;

    enablestatus = EXTI->INTENR & EXTI_Line2;
    if(((EXTI->INTFR & EXTI_Line2) != (uint32_t)RESET) && (enablestatus != (uint32_t)RESET))
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }
    return bitstatus;
}