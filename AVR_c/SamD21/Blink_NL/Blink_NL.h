
#ifndef __BLINK_NL_H__
#define __BLINK_NL_H__

#include <stdint.h>

#define PORTA  0
#define PORT_PA17 (1ul << 17) // Led  pin

typedef volatile const uint8_t  RoReg8;

typedef union {
  struct {
    uint32_t DIR:32;
  } bit;
  uint32_t reg;
} PORT_DIR_Type;

typedef union {
  struct {
    uint32_t DIRCLR:32;
  } bit;
  uint32_t reg;
} PORT_DIRCLR_Type;

typedef union {
  struct {
    uint32_t DIRSET:32;
  } bit;
  uint32_t reg;
} PORT_DIRSET_Type;

typedef union {
  struct {
    uint32_t DIRTGL:32;
  } bit;
  uint32_t reg;
} PORT_DIRTGL_Type;

typedef union {
  struct {
    uint32_t OUT:32;
  } bit;
  uint32_t reg;
} PORT_OUT_Type;

typedef union {
  struct {
    uint32_t OUTCLR:32;
  } bit;
  uint32_t reg;
} PORT_OUTCLR_Type;

typedef union {
  struct {
    uint32_t OUTSET:32;
  } bit;
  uint32_t reg;
} PORT_OUTSET_Type;

typedef union {
  struct {
    uint32_t OUTTGL:32;
  } bit;
  uint32_t reg;
} PORT_OUTTGL_Type;

typedef struct {
    PORT_DIR_Type             DIR;         /**< \brief Offset: 0x00 (R/W 32) Data Direction */
    PORT_DIRCLR_Type          DIRCLR;      /**< \brief Offset: 0x04 (R/W 32) Data Direction Clear */
    PORT_DIRSET_Type          DIRSET;      /**< \brief Offset: 0x08 (R/W 32) Data Direction Set */
    PORT_DIRTGL_Type          DIRTGL;      /**< \brief Offset: 0x0C (R/W 32) Data Direction Toggle */
    PORT_OUT_Type             OUT;         /**< \brief Offset: 0x10 (R/W 32) Data Output Value */
    PORT_OUTCLR_Type          OUTCLR;      /**< \brief Offset: 0x14 (R/W 32) Data Output Value Clear */
    PORT_OUTSET_Type          OUTSET;      /**< \brief Offset: 0x18 (R/W 32) Data Output Value Set */
    PORT_OUTTGL_Type          OUTTGL;      /**< \brief Offset: 0x1C (R/W 32) Data Output Value Toggle */
    RoReg8                    Reserved1[0x4];
    RoReg8                    Reserved2[0x4];
} PortGroup;

typedef struct {
    PortGroup Group[2];
} Port;

#define PORT              ((Port     *)0x41004400UL) /**< \brief (PORT) APB Base Address */


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
