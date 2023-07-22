
#include "debug.h"
// #include <ch32v30x.h>
#include "tft_shield_3_2.h"

#define LED_R       3 // PB3
#define LED_G       4 // PB4
#define LED_B       5 // PB5


//Список функций
void GPIO_Leds_Init(void);
void Leds_Blink(uint16_t delay);


// Основная функция
int main(void)
{
    Delay_Init();    
    USART_Printf_Init(115200);
    printf("SystemClk: %d\r\n", SystemCoreClock);    
    GPIO_Leds_Init();
        
    Lcd_Init();
    Lcd_Fill(WHITE);
    Delay_Ms(1000);
    Lcd_Fill(0xff00);
    Delay_Ms(1000);

    Lcd_Dot(120, 160, BLACK);
    Lcd_Dot(121, 160, BLACK);
    Lcd_Dot(120, 161, BLACK);
    Lcd_Dot(121, 161, BLACK);    
    Delay_Ms(1000);

    Lcd_Print('a',BLACK);
    Lcd_Print('b',BLACK);
    Lcd_Print('c',BLACK);
    Lcd_Print('d',BLACK);
    Lcd_Print('e',BLACK);
    Lcd_Print('f',BLACK);
    Lcd_Print('g',BLACK);
            
    while(1) 
    {
	    Leds_Blink(100);
    }
}


// Инициализация PB3, PB4, PB5
void GPIO_Leds_Init(void)
{
    // включаем тактирование порта B
    RCC->APB2PCENR |= RCC_APB2Periph_GPIOB;
    // PB3
    GPIOB->CFGLR &= ~(0b1111 << LED_R*4); // GPIO_Msk
    GPIOB->CFGLR |= (0b0011 << LED_R*4); // GPIO_Speed_50
    // PB4
    GPIOB->CFGLR &= ~(0b1111 << LED_G*4); // GPIO_Msk
    GPIOB->CFGLR |= (0b0011 << LED_G*4); // GPIO_Speed_50
    // PB5
    GPIOB->CFGLR &= ~(0b1111 << LED_B*4); // GPIO_Msk
    GPIOB->CFGLR |= (0b0011 << LED_B*4); // GPIO_Speed_50

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
