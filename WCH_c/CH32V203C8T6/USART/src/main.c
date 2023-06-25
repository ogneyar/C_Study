
/*
 *@Note
 USART Print debugging routine:
 USART1_Tx(PA9).
 //USART2_Tx(PA2).
 //USART3_Tx(PB10).
 This example demonstrates using USART1(PA9) as a print debug port output.

*/

#include "debug.h"
#include <ch32v20x.h>

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    Delay_Init();
    USART_Printf_Init(115200);
    printf("SystemClk:%d\r\n", SystemCoreClock);

    printf("This is printf example\r\n");

    while(1)
    {
    }
}
