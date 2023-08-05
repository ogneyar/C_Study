
/*
 *@Note
 External interrupt line routine:
 EXTI_Line4(PC4)
 PC4 sets the pull-up input, and the falling edge triggers the interrupt.
*/

#include "debug.h"
#include <ch32v00x.h>


void EXTI7_0_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void EXTI4_Init(void);



/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{
    // NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    Delay_Init();
    USART_Printf_Init(115200);
    printf("SystemClk:%d\r\n", SystemCoreClock);

    printf("EXTI Test\r\n");
    EXTI4_Init();

    while(1)
    {
        Delay_Ms(1000);
        printf("Run at main\r\n");
    }
}


/*********************************************************************
 * @fn      EXTI_IRQHandler
 *
 * @brief   This function handles EXTI Handler.
 *
 * @return  none
 */
void EXTI7_0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line4)!=RESET)
	{
		printf("Run at EXTI\r\n");
		EXTI_ClearITPendingBit(EXTI_Line4);     /* Clear Flag */
	}
}


/*********************************************************************
 * @fn      EXTI4_Init
 *
 * @brief   Initializes EXTI collection.
 *
 * @return  none
 */
void EXTI4_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    EXTI_InitTypeDef EXTI_InitStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* GPIOC ----> EXTI_Line4 */
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource4);
    EXTI_InitStructure.EXTI_Line = EXTI_Line4;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI7_0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
