//
#include <stdint.h>
 
#define APB2ENR   (*(uint32_t*)(0x40021000+0x18))
#define APB2ENR_ENIOC (1u<<4u)
 
#define GPIOC_CRH (*(uint32_t*)(0x40011000+0x04))
#define xGPIO_CRH_MODE13_0 (1u << 20u)
#define xGPIO_CRH_MODE13_1 (1u << 21u)
#define xGPIO_CRH_CNF13 (3u << 22u) 
 
#define GPIOC_ODR (*(uint32_t*)(0x40011000+0x0c))
#define GPIO_ODR_PIN_13 (1u << 13u)
 
uint16_t delay_ms = 10;

__attribute__((noreturn))
void Reset_Handler(){
    //Разрешаем тактировать GPIOC на шине APB2
    APB2ENR |= APB2ENR_ENIOC;
    // Настраиваем GPIO Pin 13 как выход Push-Pull на максимальной частоте
    // GPIOC_CRH &= ~xGPIO_CRH_CNF13;
    GPIOC_CRH |= xGPIO_CRH_MODE13_0 | xGPIO_CRH_MODE13_1;
    while(1){
        // Переключаем пин 13 на порте C
        GPIOC_ODR ^= GPIO_ODR_PIN_13;
        delay(delay_ms);
    }
}
 
// Объявим тип - указатель на прерывание
typedef void (*isr_routine)(void);
 
// Опишем структуру таблицы векторов прерываний
typedef struct  {
    const uint32_t * stack_top;
    const isr_routine reset;
} ISR_VECTOR_t;
 
//Получим адрес указателья на стек из скрипта линковки
extern const uint32_t __StackTop;
 
//Укажем линковщику, что эту константу нужно положить в секцию .isr_vector
__attribute__((section(".isr_vector"), __unused__))
const ISR_VECTOR_t  isr_vector = {
    .stack_top = &__StackTop,
    .reset  = &Reset_Handler,
};


// delay
void delay(uint16_t inter) {
	for(uint32_t i = 0; i < (uint32_t)(inter<<12); i++) __asm("nop");
}
