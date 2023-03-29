/********************************************************************
 * Ngo Hung Cuong
 * CH32V003F4P6 Example
 * VSCode
 * Makefile
********************************************************************/

#include "debug.h"

/*********************************************************************
 * @fn      GPIO_Toggle_INIT
 *
 * @brief   Initializes GPIOA.0
 *
 * @return  none
 *//*
void GPIO_Toggle_INIT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
}*/


int main(void)
{
	/*
    u8 i = 0;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    Delay_Init();
    USART_Printf_Init(115200);
    printf("SystemClk:%d\r\n", SystemCoreClock);

    printf("GPIO Toggle TEST\r\n");
    GPIO_Toggle_INIT();

    while(1)
    {
        Delay_Ms(1000);
        GPIO_WriteBit(GPIOD, GPIO_Pin_0, (i == 0) ? (i = Bit_SET) : (i = Bit_RESET));
    }
	*/
	
	
	GPIO_InitTypeDef gpioInit;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	gpioInit.GPIO_Mode = GPIO_Mode_Out_PP;
	gpioInit.GPIO_Pin = GPIO_Pin_1;
	gpioInit.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOC, &gpioInit);

	Delay_Init();

    while(1)
    {
    	GPIO_SetBits(GPIOC, GPIO_Pin_1);
    	Delay_Ms(50);
    	GPIO_ResetBits(GPIOC, GPIO_Pin_1);
    	Delay_Ms(50);
    }
	
}
