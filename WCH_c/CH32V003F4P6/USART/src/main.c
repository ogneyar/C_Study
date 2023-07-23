
/*
 *@Note
USART Print debugging routine:
 USART1_Tx(PD5).
 This example demonstrates the use of USART1(PD5) as a print debug port output.

*/

#include "debug.h"


// основная функция
int main(void)
{
    // NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    Delay_Init();
    USART_Printf_Init(115200);
    printf("SystemClk:%d\r\n", SystemCoreClock);

    printf("This is printf example\r\n");

    while(1)
    {
    }
}
