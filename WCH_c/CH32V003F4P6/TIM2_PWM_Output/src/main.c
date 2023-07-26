
/*
 *@Note
 PWM output routine:
 TIM2_CH4(PD7)
 This example demonstrates that the  TIM2_CH4(PD7) pin outputs PWM in PWM mode 1
 and PWM mode 2.

*/

#include "debug.h"
#include <ch32v00x.h>

/* PWM Output Mode Definition */
#define PWM_MODE1   0
#define PWM_MODE2   1

/* PWM Output Mode Selection */
#define PWM_MODE PWM_MODE1
// #define PWM_MODE PWM_MODE2


void TIM2_PWMOut_Init(u16 arr, u16 psc, u16 ccp);


// основная функция
int main(void)
{
    USART_Printf_Init(115200);
    printf("SystemClk:%d\r\n",SystemCoreClock);

    TIM2_PWMOut_Init( 1000, (SystemCoreClock/1000)-1, 50 ); // 

    while(1)
    {        
        if (TIM2->CNT == 1000) printf("TIM2 cnt:%d\r\n", TIM2->CNT);
    }
}


//  arr - the period value.
//  psc - the prescaler value.
//  ccp - the pulse value.
void TIM2_PWMOut_Init(u16 arr, u16 psc, u16 ccp)
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

#if (PWM_MODE == PWM_MODE1)
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;

#elif (PWM_MODE == PWM_MODE2)
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;

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
