
#ifndef __MAIN_H_
#define __MAIN_H_

#include <stdint.h>

// Список функций
int main(void);
void GPIOB_Leds_Init(void);

  
__attribute__((noreturn))
void Reset_Handler(){
    // Импортируем символы, которые мы создали в скрпите линковки
    extern uint8_t __data_start__, __data_end__, __data_rom__, 
           __bss_start__, __bss_end__;
    uint8_t *dst;
    // Обнулим сецию BSS
    dst = &__bss_start__;
    while (dst < &__bss_end__)
        *dst++ = 0;
    dst = &__data_start__;
 
    // Инициализируем переменные в .data данным из флеш-памяти
    uint8_t *src = &__data_rom__;
    while (dst < &__data_end__)
        *dst++ = *src++;
 
    main();
}
 
 
// Объявим тип - указатель на прерывание
typedef void (*isr_routine)(void);
 
// Опишем структуру таблицы векторов прерываний
typedef struct  {
    const uint32_t * stack_top;
    const isr_routine reset;
} ISR_VECTOR_t;
 
// Получим адрес указателья на стек из скрипта линковки
extern const uint32_t __stack_top__;
 
// Укажем линковщику, что эту константу нужно положить в секцию .isr_vector
__attribute__((section(".isr_vector"), __unused__))
const ISR_VECTOR_t  isr_vector = {
       .stack_top = &__stack_top__,
       .reset  = &Reset_Handler,
};

#endif /* __MAIN_H_ */
