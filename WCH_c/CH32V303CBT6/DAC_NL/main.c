
#include <stdint.h>
#include "main.h"
#include "defines.h"
#include "system.h"
#include "delay.h"
#include "usart.h"
#include "dac.h"


#define Num    7

u16 DAC_Value[Num] = {64, 128, 256, 512, 1024, 2048, 4095};


// Основная функция
int main(void)
{
    u8 i = 0;
    Delay_Init();
    GPIOB_Leds_Init();
    USART1_Init(115200);
    DAC_Init();
    printf("SystemClk:%d\r\n", SystemCoreClock);
    printf("DAC Normal OUT\r\n");

    GPIOB->BSHR |= (1 << LED_G);

    while(1)
    {
        for(i = 0; i < Num; i++)
        {
            DAC_SetChannel1Data(DAC_Align_12b_R, DAC_Value[i]);            
            printf("DAC_Value[%d]:%d\r\n", i, DAC_Value[i] );
            Leds_Blink(333);
            Leds_Blink(333);
            Leds_Blink(333);
        }
    }
}

// Инициализация PA2, PB3, PB4, PB5
void GPIOB_Leds_Init(void)
{
    // включаем тактирование порта B
    RCC->APB2PCENR |= RCC_APB2PCENR_IOPBEN;
    // PB3
    GPIOB->CFGLR &= ~(GPIO_Msk << LED_R*4); // ~(0b1111);
    GPIOB->CFGLR |= (GPIO_Speed_50 << LED_R*4); // 0b0011;
    // PB4
    GPIOB->CFGLR &= ~(GPIO_Msk << LED_G*4); // ~(0b1111);
    GPIOB->CFGLR |= (GPIO_Speed_50 << LED_G*4); // 0b0011;
    // PB5
    GPIOB->CFGLR &= ~(GPIO_Msk << LED_B*4); // ~(0b1111);
    GPIOB->CFGLR |= (GPIO_Speed_50 << LED_B*4); // 0b0011;
}

// мигание светодиодов
void Leds_Blink(uint16_t delay)
{    
    GPIOB->OUTDR ^= (1 << LED_R);
    GPIOB->OUTDR ^= (1 << LED_G);
    GPIOB->OUTDR ^= (1 << LED_B);
    Delay_Ms(delay);
}
