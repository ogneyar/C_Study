
#include <stdint.h>
#include <stdio.h>
#include "main.h"
#include "defines.h"
#include "system.h"
#include "delay.h"
#include "usart.h"
#include "spi.h"
#include "tft_shield_3.2.h"


// Основная функция
int main(void)
{
    Delay_Init();
    GPIO_Init();
    USART1_Init(115200);
    printf("SystemClk: %d\r\n", SystemCoreClock);
    printf("SPI TFT SHIELD 3.2inch\r\n");

    SPI1_Master_Init(0);    
    Delay_Ms(100);
    
    Lcd_Init();
      
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
    
    Lcd_Fill(0xf800);
	Leds_Blink(delay);
    Lcd_Fill(0x07E0);
	Leds_Blink(delay);
    Lcd_Fill(0x001F);
	Leds_Blink(delay);
    Lcd_Clear();         
	Leds_Blink(delay);
}

