
#include <stdint.h>
#include "main.h"


#define LED             0 // PA0
#define GPIO_Msk        0b1111
#define GPIO_PP_50      0b0011

#define PERIPH_BASE     (0x40000000)
#define APB2PERIPH_BASE (PERIPH_BASE + 0x10000)
#define AHBPERIPH_BASE  (PERIPH_BASE + 0x20000)
#define RCC_BASE        (AHBPERIPH_BASE + 0x1000)  // 0x40021000 //
#define GPIOA_BASE      (APB2PERIPH_BASE + 0x0800) // 0x40010800 //

#define RCC_APB2PCENR_IOPAEN (1 << 2)

#define RCC_APB2PCENR   (*(uint32_t*)(RCC_BASE + 0x18))
#define GPIOA_CFGLR     (*(uint32_t*)(GPIOA_BASE + 0x00))
#define GPIOA_OUTDR     (*(uint32_t*)(GPIOA_BASE + 0x0c))

void GPIOA_Pin0_Init(void);
void delay(uint16_t inter);


int main(void)
{
    GPIOA_Pin0_Init();

    while(1)
    {
        GPIOA_OUTDR ^= (1 << LED);
        delay(500);
    }
}

// инициализация PA0
void GPIOA_Pin0_Init(void) 
{
    // включаем тактирование порта A 
    RCC_APB2PCENR |= RCC_APB2PCENR_IOPAEN;

    GPIOA_CFGLR &= ~(GPIO_Msk << LED); // ~(0b1111);
    GPIOA_CFGLR |= (GPIO_PP_50 << LED); // 0b0011;
}

// delay
void delay(uint16_t inter)
{
	for(uint32_t i = 0; i < (uint32_t)(inter<<10); i++) __asm("nop");
}
