

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
    printf("SystemClk: %d\r\n", SystemCoreClock);
    printf("SPI SDcard\r\n");
    
    SD_Init_TypeDef init = SD_Init(GPIOA, PA0);

    if (init.error) {
        // while(1) ;
        printf("init error: %d\r\n", init.error);
    }
    
    if (init.status) printf("init status: %d\r\n", init.status);
    if (init.type) {
        printf("init type: %d\r\n", init.type);
        switch (init.type) {
            case 1:
                printf("SD_CARD_TYPE: SD1\r\n");
            break;
            case 2:
                printf("SD_CARD_TYPE: SD2\r\n");
            break;            
            case 3:
                printf("SD_CARD_TYPE: SDHC\r\n");
            break;
            default:
                printf("SD_CARD_TYPE: udefined\r\n");
            break;
        }
    }
      
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

