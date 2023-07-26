
#include "debug.h"
#include <ch32v00x.h>


// список функций
void TIM2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void TIM2_Init(u16 period);


// основная функция
int main(void)
{
    Delay_Init();
    USART_Printf_Init(115200);
    printf("SystemClk:%d\r\n", SystemCoreClock);
    printf("TIM2 Interrupt\r\n");

    TIM2_Init(1000); // прерывание таймера раз в секунду

    while(1)
    {         
        printf("Run at main\r\n");
        Delay_Ms(3000);
    }
}

//
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_CC4) != RESET)
    {        
        printf("Run at TIM2 IRQ\r\n");
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC4);     /* Clear Flag */
    }

}

//
void TIM2_Init(u16 period)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure={0};

    RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM2, ENABLE );
   
    TIM_TimeBaseInitStructure.TIM_Period = period;
    TIM_TimeBaseInitStructure.TIM_Prescaler = (SystemCoreClock/1000)-1;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit( TIM2, &TIM_TimeBaseInitStructure);

    TIM_Cmd( TIM2, ENABLE );

    NVIC_EnableIRQ(TIM2_IRQn);
    TIM_ITConfig(TIM2, TIM_IT_CC4, ENABLE);
}
