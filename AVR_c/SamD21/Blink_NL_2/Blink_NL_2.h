
#ifndef __BLINK_NL_H__
#define __BLINK_NL_H__

#include <stdint.h>

#define PORT_PA17 (1ul << 17) // Led  pin

typedef volatile uint32_t RwReg;

#define REG_PORT_DIR0              (*(RwReg  *)0x41004400U) /**< \brief (PORT) Data Direction 0 */
#define REG_PORT_DIRCLR0           (*(RwReg  *)0x41004404U) /**< \brief (PORT) Data Direction Clear 0 */
#define REG_PORT_DIRSET0           (*(RwReg  *)0x41004408U) /**< \brief (PORT) Data Direction Set 0 */
#define REG_PORT_DIRTGL0           (*(RwReg  *)0x4100440CU) /**< \brief (PORT) Data Direction Toggle 0 */
#define REG_PORT_OUT0              (*(RwReg  *)0x41004410U) /**< \brief (PORT) Data Output Value 0 */
#define REG_PORT_OUTCLR0           (*(RwReg  *)0x41004414U) /**< \brief (PORT) Data Output Value Clear 0 */
#define REG_PORT_OUTSET0           (*(RwReg  *)0x41004418U) /**< \brief (PORT) Data Output Value Set 0 */
#define REG_PORT_OUTTGL0           (*(RwReg  *)0x4100441CU) /**< \brief (PORT) Data Output Value Toggle 0 */


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

#endif /* __BLINK_NL_H__ */ 
