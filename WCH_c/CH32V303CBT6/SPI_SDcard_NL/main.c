

// в разработке!!! 


#include <stdint.h>
#include <stdio.h>
#include "main.h"
#include "defines.h"
#include "system.h"
#include "delay.h"
#include "usart.h"
#include "spi.h"
#include "sd_card.h"


// Основная функция
int main(void)
{
    Delay_Init();
    GPIO_Init();
    USART1_Init(115200);
    printf("SystemClk:%d\r\n", SystemCoreClock);
    printf("SPI SDcard\r\n");
    
    // if ( ! SD_Init(GPIOA, PA8) ) while(1) ;
    SD_Init_TypeDef * init = SD_Init(GPIOA, PA8);

    if (init->error) while(1) ;
      
    while(1)
    {
        Test_Screen();
    }
}

// Инициализация PB3, PB4, PB5
void GPIO_Init(void)
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

    GPIOB->BSHR |= (1 << LED_G);
}

// мигание светодиодов
void Leds_Blink(uint16_t delay)
{    
    GPIOB->OUTDR ^= (1 << LED_R);
    GPIOB->OUTDR ^= (1 << LED_G);
    GPIOB->OUTDR ^= (1 << LED_B);
    Delay_Ms(delay);
}

// тест дисплея
void Test_Screen(void) 
{    
    uint16_t delay = 500;
    
	Leds_Blink(delay);
}

