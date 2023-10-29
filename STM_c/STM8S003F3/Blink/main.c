
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

#define GPIOA_BaseAddress       0x5000
#define GPIOB_BaseAddress       0x5005
#define GPIOC_BaseAddress       0x500A
#define GPIOD_BaseAddress       0x500F
#define GPIOE_BaseAddress       0x5014
#define GPIOA ((GPIO_TypeDef *) GPIOA_BaseAddress)
#define GPIOB ((GPIO_TypeDef *) GPIOB_BaseAddress)
#define GPIOC ((GPIO_TypeDef *) GPIOC_BaseAddress)
#define GPIOD ((GPIO_TypeDef *) GPIOD_BaseAddress)
#define GPIOE ((GPIO_TypeDef *) GPIOE_BaseAddress)

#define TIM1_CR1    (*(volatile uint8_t *)0x5250)
#define TIM1_IER    (*(volatile uint8_t *)0x5254)
#define TIM1_SR1    (*(volatile uint8_t *)0x5255)
#define TIM1_CNTRH    (*(volatile uint8_t *)0x525E)
#define TIM1_CNTRL    (*(volatile uint8_t *)0x525F)
#define TIM1_PSCRH    (*(volatile uint8_t *)0x5260)
#define TIM1_PSCRL    (*(volatile uint8_t *)0x5261)

#define LED 3
#define GPIO GPIOC

volatile uint8_t led = 0;

void TIM1_overflow_Handler() __interrupt(11)
{
    TIM1_SR1 &= ~1;
    if (led == 1) {
       GPIO->ODR |= (1 << LED);
    }
    else
    {
       GPIO->ODR &= ~(1 << LED);  
    }
    led  ^= 1;
}


void main(void)
{
    CLK_DIVR = 0x00;
    CLK_PCKENR1 = 0xFF;

    // GPIO->DDR |= (1 << LED);
    __asm__ ("bset	0x500C, #3");	

    TIM1_PSCRH = 0x00;
    TIM1_PSCRL = 0xF4;
    TIM1_CR1 = 0x01;
    TIM1_IER = 0x01;
    __asm__ ("rim");
    while(1)
    {
        __asm__ ("WFI");
    }
}
