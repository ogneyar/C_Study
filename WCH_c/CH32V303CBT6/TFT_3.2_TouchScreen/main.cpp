
#include "debug.h"
#include <ch32v30x.h>
#include "tft_shield_3_2.h"
#include "xpt2046.h"

#define LED_R       3 // PB3
#define LED_G       4 // PB4
#define LED_B       5 // PB5

// #define __DEBUG__

volatile uint8_t status = 0;
// volatile uint8_t delay = 0;
// volatile uint16_t x_kor, y_kor;


//Список функций
void GPIO_Leds_Init(void);
void Leds_Blink(uint16_t delay);
void Test_Screen(void);
void EXTI1_Init(void);
// void EXTI1_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
// uint8_t EXTI_GetITStatusLine1(void);


const int chipSelect = 0;


// Основная функция
int main(void)
{
    Delay_Init();    
    USART_Printf_Init(115200);
    printf("SystemClk: %d\r\n", SystemCoreClock);    
    GPIO_Leds_Init();
        
    Lcd_Init();
    Lcd_Fill(0xffff);
    
    EXTI1_Init(); // PB1 - External Interrupt
    XPT_Init(GPIOA, PA1); // PA1 - Chip Select Pin
            
    while(1) 
    {
        // Test_Screen();
        // printf("Test\r\n");
	    Leds_Blink(100);

        // if (status && ! delay) {
        if (status) {
            // delay = 1;

            // XPT_GetTouch_xy(&x_kor, &y_kor);
            // printf("x_kor: %d\r\n", x_kor);
            // printf("y_kor: %d\r\n\r\n", y_kor);

            // Delay_Ms(100);
            // delay = 0;
            status = 0;
        }
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


// Инициализация EXTI, NVIC, GPIO 
void EXTI1_Init(void) // PB1
{
    uint8_t PB1 = 1;
    uint32_t tmp;

    // включаем тактирование порта B и альтернативных функций
    RCC->APB2PCENR |= RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO;
    // PB1
    GPIOB->CFGLR &= ~(0b1111 << PB1*4); // GPIO_Msk
    GPIOB->CFGLR |= (0b1000 << PB1*4); // GPIO_IPU
    GPIOB->BSHR |= (1 << PB1);

    /* GPIOB ----> EXTI_Line1 */
    tmp = 0x00;

    tmp = ((uint32_t)0x0F) << (0x04 * (GPIO_PinSource1 & (uint8_t)0x03));
    AFIO->EXTICR[GPIO_PinSource1 >> 0x02] &= ~tmp;
    AFIO->EXTICR[GPIO_PinSource1 >> 0x02] |= (((uint32_t)GPIO_PortSourceGPIOB) << (0x04 * (GPIO_PinSource1 & (uint8_t)0x03)));

    tmp = 0;

    tmp = (uint32_t)EXTI_BASE;
    EXTI->INTENR &= ~EXTI_Line1;
    EXTI->EVENR &= ~EXTI_Line1;
    tmp += EXTI_Mode_Interrupt;
    *(__IO uint32_t *)tmp |= EXTI_Line1;

    tmp = (uint32_t)EXTI_BASE;
    EXTI->RTENR &= ~EXTI_Line1;
    EXTI->FTENR &= ~EXTI_Line1;    
    tmp += EXTI_Trigger_Falling;
    *(__IO uint32_t *)tmp |= EXTI_Line1;
        

    uint8_t NVIC_IRQChannelSubPriority = 2;
    NVIC->IPRIOR[(uint32_t)(EXTI1_IRQn)] = (NVIC_IRQChannelSubPriority << 4);
    NVIC->IENR[((uint32_t)(EXTI1_IRQn) >> 5)] = (1 << ((uint32_t)(EXTI1_IRQn) & 0x1F));
}


//
// void EXTI1_IRQHandler(void)
// {
//     if(EXTI_GetITStatusLine1()!=RESET)
//     {
//         status = 1;
//         // XPT_GetTouch_xy(&x_kor, &y_kor);
// #ifdef __DEBUG__
//             // printf("Run at EXTI\r\n");
//         // if (x_kor <= 240 && y_kor <= 320) {
//         //     printf("x_kor: %d\r\n", x_kor);
//         //     printf("y_kor: %d\r\n\r\n", y_kor);
//         // }
// #endif /* __DEBUG__ */
//         EXTI->INTFR = EXTI_Line1;     /* Clear Flag */
//     }
// }


//
// uint8_t EXTI_GetITStatusLine1(void)
// {
//     uint8_t bitstatus = RESET;
//     uint32_t enablestatus = 0;

//     enablestatus = EXTI->INTENR & EXTI_Line1;
//     if(((EXTI->INTFR & EXTI_Line1) != (uint32_t)RESET) && (enablestatus != (uint32_t)RESET))
//     {
//         bitstatus = SET;
//     }
//     else
//     {
//         bitstatus = RESET;
//     }
//     return bitstatus;
// }
