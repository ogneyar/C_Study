
/*
 *@Note
 Output comparison mode routine:
 TIM2_CH4(PD7)
 This example demonstrates the output waveform of the TIM2_CH4(PD7) pin in 4 output comparison modes.
  Output compare modes include
 OutCompare_Timing\OutCompare_Active\OutCompare_Inactive\OutCompare_Toggle.

*/

#include "debug.h"
#include <ch32v00x.h>

/* Output Compare Mode Definition */
#define OutCompare_Timing   0
#define OutCompare_Active   1
#define OutCompare_Inactive 2
#define OutCompare_Toggle   3

/* Output Compare Mode Selection */
// #define OutCompare_MODE OutCompare_Timing
// #define OutCompare_MODE OutCompare_Active
// #define OutCompare_MODE OutCompare_Inactive
#define OutCompare_MODE OutCompare_Toggle


void TIM2_OutCompare_Init(u16 arr, u16 psc, u16 ccp);



// основная функция
int main(void)
{
    USART_Printf_Init(115200);
    printf("SystemClk:%d\r\n",SystemCoreClock);

    TIM2_OutCompare_Init( 100, 48000-1, 50 );

    while(1);
}

//  arr - the period value.
//  psc - the prescaler value.
//  ccp - the pulse value.
void TIM2_OutCompare_Init(u16 arr, u16 psc, u16 ccp)
{
    GPIO_InitTypeDef GPIO_InitStructure={0};
    TIM_OCInitTypeDef TIM_OCInitStructure={0};
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure={0};

    RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM2, ENABLE );
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD, ENABLE );

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( GPIOD, &GPIO_InitStructure );

    TIM_TimeBaseInitStructure.TIM_Period = arr;
    TIM_TimeBaseInitStructure.TIM_Prescaler = psc;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit( TIM2, &TIM_TimeBaseInitStructure);

#if (OutCompare_MODE == OutCompare_Timing)
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;

#elif (OutCompare_MODE == OutCompare_Active)
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Active;

#elif (OutCompare_MODE == OutCompare_Inactive)
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Inactive;

#elif (OutCompare_MODE == OutCompare_Toggle)
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;

#endif

    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = ccp;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC4Init( TIM2, &TIM_OCInitStructure );

    TIM_CtrlPWMOutputs(TIM2, ENABLE );
    TIM_OC4PreloadConfig( TIM2, TIM_OCPreload_Disable );
    TIM_ARRPreloadConfig( TIM2, ENABLE );
    TIM_Cmd( TIM2, ENABLE );
}
