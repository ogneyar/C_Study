
#include "debug.h"
#include <ch32v30x.h>


// Список функций
void GPIO_Toggle_Init(void);


// Основная функция
int main(void)
{
    u8 i = 0;

    // NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    Delay_Init();
    USART_Printf_Init(115200);
    printf("SystemClk: %d\r\n", SystemCoreClock);

    printf("GPIO Toggle TEST\r\n");
    GPIO_Toggle_Init();

    while(1)
    {
        GPIO_WriteBit(GPIOB, GPIO_Pin_3, (i == 0) ? (i = Bit_SET) : (i = Bit_RESET));
        GPIO_WriteBit(GPIOB, GPIO_Pin_4, (i == 0) ? (i = Bit_SET) : (i = Bit_RESET));
        GPIO_WriteBit(GPIOB, GPIO_Pin_5, (i == 0) ? (i = Bit_SET) : (i = Bit_RESET));
        Delay_Ms(100);
    }
}


//
void GPIO_Toggle_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}
