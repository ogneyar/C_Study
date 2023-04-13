//
#include "Test.h"
#include "../libs/stm32f10x.h"
#include "../libs/system_stm32f10x.h"


int main(void) {    
    // Включаем тактирование МК на 72MHz
    SystemInit();
    //Разрешаем тактировать GPIOC на шине APB2
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    // Настраиваем GPIO Pin 13 как выход Push-Pull на максимальной частоте
    GPIOC->CRH |= GPIO_CRH_MODE13_0 | GPIO_CRH_MODE13_1;
    while(1){
// loop_main:
        // Переключаем пин 13 на порте C
        GPIOC->ODR ^= GPIO_ODR_ODR13;
        delay(1000);
    }
    // goto loop_main;
}


// delay
void delay(uint16_t inter) {
	for(uint32_t i = 0; i < (uint32_t)(inter<<12); i++) __asm("nop");
}
