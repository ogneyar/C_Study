
#include <stdint.h>
#include <stdio.h>

#define CLK_DIVR    (*(volatile uint8_t *)0x50c6)
#define CLK_PCKENR1    (*(volatile uint8_t *)0x50c7)

#define     __IO    volatile

typedef struct GPIO_struct
{
    __IO uint8_t ODR;
    __IO uint8_t IDR;
    __IO uint8_t DDR;
    __IO uint8_t CR1;
    __IO uint8_t CR2;
}
GPIO_TypeDef;

#define GPIOB_BaseAddress       0x5005
#define GPIOE_BaseAddress       0x5014
#define GPIOB ((GPIO_TypeDef *) GPIOB_BaseAddress)
#define GPIOE ((GPIO_TypeDef *) GPIOE_BaseAddress)


#define TIM1_CR1    (*(volatile uint8_t *)0x5250)
#define TIM1_IER    (*(volatile uint8_t *)0x5254)
#define TIM1_SR1    (*(volatile uint8_t *)0x5255)
#define TIM1_CNTRH    (*(volatile uint8_t *)0x525E)
#define TIM1_CNTRL    (*(volatile uint8_t *)0x525F)
#define TIM1_PSCRH    (*(volatile uint8_t *)0x5260)
#define TIM1_PSCRL    (*(volatile uint8_t *)0x5261)

volatile uint8_t led = 0;

void TIM1_overflow_Handler() //__interrupt(11)
{
     TIM1_SR1 &= ~1;
     if (led == 1) {
        GPIOB->ODR |= (1 << 5);
     }
     else
     {
        GPIOB->ODR &= ~(1 << 5);  
     }
     led  ^= 1;
}


void main(void)
{
__asm__(
    // CLK_DIVR = 0x00;
    "mov	0x50c6+0, #0x00"
    // CLK_PCKENR1 = 0xFF;
	"mov	0x50c7+0, #0xff"
    // GPIOB->DDR |= (1 << 5);
	"bset	0x5007, #5"
    // GPIOB->ODR |= (1 << 5);
	"ld	a, 0x5005"
	"or	a, #0x20"
	"ld	0x5005, a"
    // TIM1_PSCRH = 0x00;
	"mov	0x5260+0, #0x00"
    // TIM1_PSCRL = 0xF4;
	"mov	0x5261+0, #0xf4"
    // TIM1_CR1 = 0x01;
	"mov	0x5250+0, #0x01"
    // TIM1_IER = 0x01;
	"mov	0x5254+0, #0x01"
	"rim"
"while_true:"
	"WFI"
	"jra	while_true"
);
}
