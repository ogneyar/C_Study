
// #include "main.h"
#include <stdint.h>

void SystemInit (void){}

#define USR_BTN         2 // PA2
#define LED_R           3 // PB3
#define LED_G           4 // PB4
#define LED_B           5 // PB5
#define GPIO_Msk        0b1111
#define GPIO_PP_50      0b0011
#define GPIO_Mode_IPU   0b1000

#define PERIPH_BASE     (0x40000000)
#define APB2PERIPH_BASE (PERIPH_BASE + 0x10000)
#define AHBPERIPH_BASE  (PERIPH_BASE + 0x20000)
#define RCC_BASE        (AHBPERIPH_BASE + 0x1000)  // 0x40021000 //
#define GPIOA_BASE      (APB2PERIPH_BASE + 0x0800) // 0x40010800 //
#define GPIOB_BASE      (APB2PERIPH_BASE + 0x0C00) // 0x40010C00 //

#define RCC_APB2PCENR_IOPAEN (1 << 2)
#define RCC_APB2PCENR_IOPBEN (1 << 3)

#define RCC_APB2PCENR   (*(uint32_t*)(RCC_BASE + 0x18))
#define GPIOA_CFGLR     (*(uint32_t*)(GPIOA_BASE + 0x00))
#define GPIOA_INDR      (*(uint32_t*)(GPIOA_BASE + 0x08)) // Port Input Register
#define GPIOA_BSHR      (*(uint32_t*)(GPIOA_BASE + 0x10)) // Port Set/Reset Register
#define GPIOA_BCR       (*(uint32_t*)(GPIOA_BASE + 0x14)) // Port Reset Register
#define GPIOB_CFGLR     (*(uint32_t*)(GPIOB_BASE + 0x00))
#define GPIOB_OUTDR     (*(uint32_t*)(GPIOB_BASE + 0x0c)) // Port Output Register

// Список функций
void GPIO_Init(void);
void delay(uint16_t inter);


// Основная функция
int main(void)
{
    GPIO_Init();

    GPIOB_OUTDR ^= (1 << LED_G);

    while(1)
    {
        GPIOB_OUTDR ^= (1 << LED_R);
        GPIOB_OUTDR ^= (1 << LED_G);
        GPIOB_OUTDR ^= (1 << LED_B);
        uint8_t btn = (GPIOA_INDR & (1 << USR_BTN));
        if ( ! btn ) delay(100);
        else delay(500);
    }
}


// Инициализация PA2, PB3, PB4, PB5
void GPIO_Init(void)
{
    // включаем тактирование порта A
    RCC_APB2PCENR |= RCC_APB2PCENR_IOPAEN;
    // включаем тактирование порта B
    RCC_APB2PCENR |= RCC_APB2PCENR_IOPBEN;
    // PA2
    GPIOA_CFGLR &= ~(GPIO_Msk << USR_BTN*4); // ~(0b1111);
    GPIOA_CFGLR |= (GPIO_Mode_IPU << USR_BTN*4); // 0b1000;
    GPIOA_BSHR |= (1 << USR_BTN);
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
