
#include <stdint.h>
#include "main.h"
#include "defines.h"
#include "system.h"
#include "delay.h"
#include "usart.h"


// Основная функция
int main(void)
{
    Delay_Init();
    GPIOB_Leds_Init();
    USART1_Init(115200);
	printf("SystemClk:%d\r\n", SystemCoreClock);
	printf("This is printf example\r\n");

    GPIOB->BSHR |= (1 << LED_G);

    while(1)
    {
        GPIOB->OUTDR ^= (1 << LED_R);
        GPIOB->OUTDR ^= (1 << LED_G);
        GPIOB->OUTDR ^= (1 << LED_B);
        Delay_Ms(500);
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
    // PB5
    GPIOB->CFGLR &= ~(GPIO_Msk << LED_B*4); // ~(0b1111);
    GPIOB->CFGLR |= (GPIO_Speed_50 << LED_B*4); // 0b0011;
}
