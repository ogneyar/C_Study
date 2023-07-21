
#include <stdint.h>
#include "main.h"
#include "defines.h"
#include "system.h"
#include "delay.h"
#include "usart.h"
#include "rtc.h"



// Основная функция
int main(void)
{
    Delay_Init();
    GPIOB_Leds_Init();
    USART1_Init(115200);
    printf("SystemClk:%d\r\n", SystemCoreClock);
    printf("RTC NoLib Test\r\n");
    
    RTC_Init();

    // RTC_Alarm_Set(2023, 7, 20, 14, 20, 30);

    while(1)
    {
        Leds_Blink(250);
        Leds_Blink(250);
        Leds_Blink(250);
        Leds_Blink(250);
        printf("year/month/day/week/hour/min/sec:\r\n");
        printf("%d-%d-%d  %d  %d:%d:%d\r\n", 
            calendar.w_year, calendar.w_month, calendar.w_date,
            calendar.week, calendar.hour, calendar.min, calendar.sec);
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
    GPIOB->BSHR |= (1 << LED_G);
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

// обработчик прерываний RTC
void RTC_IRQHandler(void)
{
    if(RTC_GetITStatus(RTC_IT_SEC) != RESET) // Seconds interrupt
    {
        RTC_Get();
    }
    if(RTC_GetITStatus(RTC_IT_ALR) != RESET) // Alarm clock interrupt
    {
        printf("\r\n\r\nAlarm clock interrupt\r\n\r\n");
        // RTC Clear IT Pending Bit
        RTC->CTLRL &= (uint16_t)~RTC_IT_ALR;
        RTC_Get();
    }
    
    // RTC Clear IT Pending Bit
    RTC->CTLRL &= (uint16_t)~(RTC_IT_SEC | RTC_IT_OW);
    RTC_WaitForLastTask();
}