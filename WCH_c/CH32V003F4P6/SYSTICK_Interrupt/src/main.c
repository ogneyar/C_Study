
/*
 *@Note
 Systick interrupt routine:

*/

#include "debug.h"
#include <ch32v00x.h>



// основная функция
int main(void)
{
    USART_Printf_Init(115200);
    printf("SystemClk:%d\r\n", SystemCoreClock);

    NVIC_EnableIRQ(SysTicK_IRQn);
    SysTick->SR &= ~(1 << 0);
    SysTick->CMP = SystemCoreClock-1;
    SysTick->CNT = 0;
    SysTick->CTLR = 0xF;

    while(1);
}


void SysTick_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

//
void SysTick_Handler(void)
{
    printf("Systick\r\n");
    SysTick->SR = 0;
}
