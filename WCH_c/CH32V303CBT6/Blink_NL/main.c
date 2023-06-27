
#include "main.h"
#include <stdint.h>

void SystemInit (void){}

#define LED_R             3 // PB3
#define LED_G             4 // PB4
#define LED_B             5 // PB5
#define GPIO_Msk        0b1111
#define GPIO_PP_50      0b0011

#define PERIPH_BASE     (0x40000000)
#define APB2PERIPH_BASE (PERIPH_BASE + 0x10000)
#define AHBPERIPH_BASE  (PERIPH_BASE + 0x20000)
#define RCC_BASE        (AHBPERIPH_BASE + 0x1000)  // 0x40021000 //
#define GPIOB_BASE      (APB2PERIPH_BASE + 0x0C00) // 0x40010C00 //

#define RCC_APB2PCENR_IOPBEN (1 << 3)

#define RCC_APB2PCENR   (*(uint32_t*)(RCC_BASE + 0x18))
#define GPIOB_CFGLR     (*(uint32_t*)(GPIOB_BASE + 0x00))
#define GPIOB_OUTDR     (*(uint32_t*)(GPIOB_BASE + 0x0c))

// Список функций
void GPIOB_Leds_Init(void);
void delay(uint16_t inter);


// Основная функция
int main(void)
{
    GPIOB_Leds_Init();

    GPIOB_OUTDR ^= (1 << LED_G);

    while(1)
    {
        GPIOB_OUTDR ^= (1 << LED_R);
        GPIOB_OUTDR ^= (1 << LED_G);
        GPIOB_OUTDR ^= (1 << LED_B);
        delay(100);
    }
}

// Инициализация PA2, PB3, PB4, PB5
void GPIOB_Leds_Init(void)
{
    // включаем тактирование порта B
    RCC_APB2PCENR |= RCC_APB2PCENR_IOPBEN;
    // PB3
    GPIOB_CFGLR &= ~(GPIO_Msk << LED_R*4); // ~(0b1111);
    GPIOB_CFGLR |= (GPIO_PP_50 << LED_R*4); // 0b0011;
    // PB4
    GPIOB_CFGLR &= ~(GPIO_Msk << LED_G*4); // ~(0b1111);
    GPIOB_CFGLR |= (GPIO_PP_50 << LED_G*4); // 0b0011;
    // PB5
    GPIOB_CFGLR &= ~(GPIO_Msk << LED_B*4); // ~(0b1111);
    GPIOB_CFGLR |= (GPIO_PP_50 << LED_B*4); // 0b0011;
}

// Задержка
void delay(uint16_t inter)
{
	for(uint32_t i = 0; i < (uint32_t)(inter<<10); i++) __asm("nop");
}
