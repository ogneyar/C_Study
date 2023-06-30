
#include <stdint.h>
#include <stdio.h>
#include "main.h"
#include "defines.h"
#include "system.h"
#include "delay.h"
#include "usart.h"
#include "spi.h"
#include "st7735.h"


/* Список функций */
void Test_Screen(void);


// Основная функция
int main(void)
{
    Delay_Init();
    GPIOB_Leds_Init();
    USART1_Init(115200);
    printf("SystemClk:%d\r\n", SystemCoreClock);
    printf("SPI ST7735 0.96inch\r\n");

    SPI1_Master_Init();    
    Delay_Ms(100);

    ST7735_Init();    

    while(1)
    {
        Test_Screen();
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

    ST7735_Clear();    
	Leds_Blink(delay);
	Leds_Blink(delay);

    ST7735_Fill(_WHITE, 5, 5, 10, 10);    
	Leds_Blink(delay);
	Leds_Blink(delay);

    ST7735_Fill(_RED, 10, 10, 20, 20);  
	Leds_Blink(delay);
	Leds_Blink(delay);

    ST7735_Fill(_GREEN, 15, 15, 30, 30);   
	Leds_Blink(delay);
	Leds_Blink(delay);

    ST7735_Fill(_BLUE, 20, 20, 40, 40);    
	Leds_Blink(delay);
	Leds_Blink(delay);

    ST7735_Fill(_RED_BLUE, 25, 25, 50, 50);  
	Leds_Blink(delay);
	Leds_Blink(delay);

    ST7735_Fill(_BLUE_GREEN, 30, 30, 60, 60); 
	Leds_Blink(delay);
	Leds_Blink(delay);

    ST7735_Fill(_RED_GREEN, 35, 35, 70, 70);
	Leds_Blink(delay);
	Leds_Blink(delay);
}


