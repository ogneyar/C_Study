
#include <stdint.h>
#include "main.h"
#include "defines.h"
#include "system.h"
#include "delay.h"
#include "usart.h"
#include "bkp.h"


// Основная функция
int main(void)
{
    Delay_Init();
    GPIOB_Leds_Init();
    USART1_Init(115200);
    printf("SystemClk:%d\r\n", SystemCoreClock);
    printf("BKP NoLib Test\r\n");
    
    BKP_Tamper_Init();

    while(1)
    {    
		Leds_Blink(500);
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

// обработчик прерываний TAMPER
void TAMPER_IRQHandler(void)
{
    if( BKP_GetITStatus() == SET )
    {
        printf("TAMPER_IRQHandler\r\n");
        printf("BKP_DR1: 0x%04x\r\n", BKP->DATAR1);
        printf("BKP_DR2: 0x%04x\r\n", BKP->DATAR2);
        printf("BKP_DR3: 0x%04x\r\n", BKP->DATAR3);
        printf("BKP_DR4: 0x%04x\r\n", BKP->DATAR4);
    }

    // BKP Clear IT Pending Bit
    BKP->TPCSR |= BKP_CTI;
}
