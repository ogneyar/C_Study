
/*
 *@Note
 PWM output routine:
 TIM1_CH1(PA8)
 This example demonstrates that the TIM_CH1(PA8) pin outputs PWM in PWM
 mode 1 and PWM mode 2.
 
*/

#include "debug.h"

/* PWM Output Mode Definition */
#define PWM_MODE1   0
#define PWM_MODE2   1

/* PWM Output Mode Selection */
//#define PWM_MODE PWM_MODE1
#define PWM_MODE PWM_MODE2

/* Список функций*/
void TIM1_PWMOut_Init(u16 arr, u16 psc, u16 ccp);


/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{
	Delay_Init();
	USART_Printf_Init(115200);
	printf("SystemClk:%d\r\n",SystemCoreClock);
	
	/*
	первый параметр: период
	второй параметр: частота (при SystemCoreClock = 48МГц, 48000-1 выдаст 1Гц, 48-1 - 1КГц)
	третий параметр: заполняемость периода, пульс (при периоде = 100, а пульсе 10, яркость светодиода будет 10%)

	TIM1_PWMOut_Init( 100, 48-1, 10 );
	*/

	int pulse = 0;
	u8 direction = 1; // направление, 1 - рост, 0 - падение
	while(1) {
		TIM1_PWMOut_Init( 100, 48-1, pulse );
		if (direction) pulse++;
		else pulse--;
		if (pulse == 100) direction = 0;
		if (pulse == 0) direction = 1;
		Delay_Ms(10);
	}
}


/*********************************************************************
 * @fn      TIM1_OutCompare_Init
 *
 * @brief   Initializes TIM1 output compare.
 *
 * @param   arr - the period value.
 *          psc - the prescaler value.
 *          ccp - the pulse value.
 *
 * @return  none
 */
void TIM1_PWMOut_Init(u16 arr, u16 psc, u16 ccp)
{
	GPIO_InitTypeDef GPIO_InitStructure={0};
	TIM_OCInitTypeDef TIM_OCInitStructure={0};
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure={0};

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_TIM1, ENABLE );

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOA, &GPIO_InitStructure );

	TIM_TimeBaseInitStructure.TIM_Period = arr;
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit( TIM1, &TIM_TimeBaseInitStructure);

#if (PWM_MODE == PWM_MODE1)
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;

#elif (PWM_MODE == PWM_MODE2)
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;

#endif

	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = ccp;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init( TIM1, &TIM_OCInitStructure );

	TIM_CtrlPWMOutputs(TIM1, ENABLE );
	TIM_OC1PreloadConfig( TIM1, TIM_OCPreload_Disable );
	TIM_ARRPreloadConfig( TIM1, ENABLE );
	TIM_Cmd( TIM1, ENABLE );
}
