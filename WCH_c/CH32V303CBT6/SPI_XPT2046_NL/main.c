
#include <stdint.h>
#include <stdio.h>
#include "main.h"
#include "Hutorino.h"
#include "system.h"
#include "delay.h"
#include "usart.h"
#include "spi.h"
#include "xpt.h"
#include "exti.h"


volatile uint16_t x_kor, y_kor;

// Основная функция
int main(void)
{
    Delay_Init();
    GPIO_Init();
    USART1_Init(115200);
    printf("SystemClk: %d\r\n", SystemCoreClock);
    printf("SPI XPT2046\r\n");
    
    EXTI1_Init(); // PB1 - External Interrupt

    /* 
    если SystemCoreClock = HSI_VALUE, тогда SPI_BaudRatePrescaler_
    если SystemCoreClock = SYSCLK_FREQ_48MHz_HSI, тогда SPI_BaudRatePrescaler_
    если SystemCoreClock = SYSCLK_FREQ_144MHz_HSI, тогда SPI_BaudRatePrescaler_
    */
     
    XPT_Init(GPIOA, PA1); // PA1 - Chip Select Pin
      
    while(1)
    {
        Test_Screen();
        Test_Screen();
        printf("Test\r\n");
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


//
void EXTI1_IRQHandler(void)
{
    if(EXTI_GetITStatusLine1()!=RESET)
    {
        XPT_GetTouch_xy(&x_kor, &y_kor);
        printf("Run at EXTI\r\n");
        // printf("x_kor: %d\r\n", x_kor);
        // printf("y_kor: %d\r\n", y_kor);
        EXTI->INTFR = EXTI_Line1;     /* Clear Flag */
    }
}


//
uint8_t EXTI_GetITStatusLine1(void)
{
    uint8_t bitstatus = RESET;
    uint32_t enablestatus = 0;

    enablestatus = EXTI->INTENR & EXTI_Line1;
    if(((EXTI->INTFR & EXTI_Line1) != (uint32_t)RESET) && (enablestatus != (uint32_t)RESET))
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }
    return bitstatus;
}