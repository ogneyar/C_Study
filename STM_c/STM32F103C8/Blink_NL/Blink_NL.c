//
#include "Blink_NL.h"
// #include "stm32f10x.h"

#define RCC_APB2ENR   (*(uint32_t*)(0x40021000+0x18))
#define RCC_APB2ENR_IOPCEN (1u<<4u)
 
#define GPIOC_CRH (*(uint32_t*)(0x40011000+0x04))
#define GPIO_CRH_MODE13_0 (1u <<20u)
#define GPIO_CRH_MODE13_1 (1u <<21u)
#define GPIO_CRH_CNF13 ((uint32_t)0x00C00000)
 
#define GPIOC_ODR (*(uint32_t*)(0x40011000+0x0c))
#define GPIO_ODR_ODR13 (1u << 13u)
 

int main(void) {    
    //Разрешаем тактировать GPIOC на шине APB2
    RCC_APB2ENR |= RCC_APB2ENR_IOPCEN;
    // Настраиваем GPIO Pin 13 как выход Push-Pull на максимальной частоте
    GPIOC_CRH &= ~GPIO_CRH_CNF13;
    GPIOC_CRH |= GPIO_CRH_MODE13_0 | GPIO_CRH_MODE13_1;
    // while(1){
loop_main:
        // Переключаем пин 13 на порте C
        GPIOC_ODR ^= GPIO_ODR_ODR13;
        delay(100);
    // }
    goto loop_main;
}


// delay
void delay(uint16_t inter) {
	for(uint32_t i = 0; i < (uint32_t)(inter<<9); i++) __asm("nop");
}