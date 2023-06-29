
#include <stdint.h>
#include <stdio.h>
#include "main.h"
#include "defines.h"
#include "system.h"
#include "delay.h"
#include "usart.h"
#include "i2c.h"
#include "ssd1306.h"


/* Список функций */
void Test_Screen(void);


// Основная функция
int main(void)
{
    Delay_Init();
    GPIOB_Leds_Init();
    USART1_Init(115200);
    printf("SystemClk:%d\r\n", SystemCoreClock);
    printf("I2C SSD1306 128x32\r\n");

	I2C2_Init(400000, address);	
	SSD1306_Init();	

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
	uint8_t flag = 0xff;
	uint16_t delay = 250;

	SSD1306_ScreenClear();
	Leds_Blink(delay);
	Leds_Blink(delay);
	
	for(uint16_t i = 0; i < 1024; i++) {
		scr_buffer[i] = flag;
		if (flag) flag = 0x00;
		else flag = 0xff;
	}
	SSD1306_ScreenUpdate();
	Leds_Blink(delay);
	Leds_Blink(delay);

	SSD1306_ScreenClear();	
	Leds_Blink(delay);
	Leds_Blink(delay);

	for(uint16_t i = 0; i < SCREEN_BUFFER_LENGTH; i++) scr_buffer[i] = 0xff;
	SSD1306_ScreenUpdate();
	Leds_Blink(delay);
	Leds_Blink(delay);
}


