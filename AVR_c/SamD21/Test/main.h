
#ifndef __MAIN_H__
#define __MAIN_H__

#include <stdint.h>

extern int main(void);
  
// __attribute__((noreturn))
void Reset_Handler(){ 
    // int response = main();
    main();
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

#endif /* __MAIN_H__ */ 
