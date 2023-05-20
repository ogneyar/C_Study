
// #include "main.h"
#include <stdint.h>

void SystemInit (void){}

#define LED             0 // PD0
#define GPIO_Msk        0b1111
#define GPIO_PP_50      0b0011

#define PERIPH_BASE     (0x40000000)
#define APB2PERIPH_BASE (PERIPH_BASE + 0x10000)
#define AHBPERIPH_BASE  (PERIPH_BASE + 0x20000)
#define RCC_BASE        (AHBPERIPH_BASE + 0x1000)  // 0x40021000 //
#define GPIOD_BASE      (APB2PERIPH_BASE + 0x1400) // 0x40011400 //

#define RCC_APB2PCENR_IOPDEN (1 << 5)

#define RCC_APB2PCENR   (*(uint32_t*)(RCC_BASE + 0x18))
#define GPIOD_CFGLR     (*(uint32_t*)(GPIOD_BASE + 0x00))
#define GPIOD_OUTDR     (*(uint32_t*)(GPIOD_BASE + 0x0c))

void GPIOD_Pin0_Init(void);
void delay(uint16_t inter);


int main(void)
{
    GPIOD_Pin0_Init();

    while(1)
    {
        GPIOD_OUTDR ^= (1 << LED);
        delay(500);
    }
}

// инициализация PD0
void GPIOD_Pin0_Init(void)
{
    // включаем тактирование порта D
    RCC_APB2PCENR |= RCC_APB2PCENR_IOPDEN;

    GPIOD_CFGLR &= ~(GPIO_Msk << LED); // ~(0b1111);
    GPIOD_CFGLR |= (GPIO_PP_50 << LED); // 0b0011;
}

// delay
void delay(uint16_t inter)
{
	for(uint32_t i = 0; i < (uint32_t)(inter<<11); i++) __asm("nop");
}
