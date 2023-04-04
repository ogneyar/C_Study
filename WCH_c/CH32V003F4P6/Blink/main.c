
#include "debug.h"
#include <ch32v00x.h>

void GPIOD_Pin0_Init(void);


int main(void)
{
    u8 i = 0;

    // NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    Delay_Init();
    USART_Printf_Init(115200);
    printf("SystemClk:%d\r\n", SystemCoreClock);

    printf("GPIO Toggle TEST\r\n");
    GPIOD_Pin0_Init();

    while(1)
    {
        Delay_Ms(500);
        GPIO_WriteBit(GPIOD, GPIO_Pin_0, (i == 0) ? (i = Bit_SET) : (i = Bit_RESET));
    }
}

// инициализация PD0
void GPIOD_Pin0_Init(void)
{
    // включаем тактирование порта D
    // RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);    
    RCC->APB2PCENR |= RCC_APB2Periph_GPIOD;

    GPIO_InitTypeDef GPIO_InitStructure = {0};
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
}
