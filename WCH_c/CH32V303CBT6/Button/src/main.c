
#include "debug.h"
#include <ch32v30x.h>

/* Список функций */
void GPIO_Toggle_INIT(void);


/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{
    u8 i = 0;

    // NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    Delay_Init();
    USART_Printf_Init(115200);
    printf("SystemClk:%d\r\n", SystemCoreClock);

    printf("GPIO Toggle TEST\r\n");
    GPIO_Toggle_INIT();

    while(1)
    {
        GPIO_WriteBit(GPIOB, GPIO_Pin_3, (i == 0) ? (i = Bit_SET) : (i = Bit_RESET));
        GPIO_WriteBit(GPIOB, GPIO_Pin_4, (i == 0) ? (i = Bit_SET) : (i = Bit_RESET));
        GPIO_WriteBit(GPIOB, GPIO_Pin_5, (i == 0) ? (i = Bit_SET) : (i = Bit_RESET));
        u8 btn = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2);
        if ( ! btn ) Delay_Ms(100); // ! btn - кнопка нажата
        else Delay_Ms(500);
    }
}



/*********************************************************************
 * @fn      GPIO_Toggle_INIT
 *
 * @brief   Initializes GPIOB.3 GPIOB.4 GPIOB.5 GPIOA.2
 *
 * @return  none
 */
void GPIO_Toggle_INIT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
    // PB3     
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    // PB4  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    // PB5  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    // PA2 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    // GPIO_SetBits(GPIOA, GPIO_Pin_2);
}
