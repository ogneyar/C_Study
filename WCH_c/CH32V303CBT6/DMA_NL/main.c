
#include <stdint.h>
#include "main.h"
#include "defines.h"
#include "system.h"
#include "delay.h"
#include "usart.h"
#include "dma.h"


#define Buf_Size    32

uint32_t SRC_BUF[Buf_Size] = {
    0x01020304, 0x05060708, 0x090A0B0C, 0x0D0E0F10,
    0x11121314, 0x15161718, 0x191A1B1C, 0x1D1E1F20,
    0x21222324, 0x25262728, 0x292A2B2C, 0x2D2E2F30,
    0x31323334, 0x35363738, 0x393A3B3C, 0x3D3E3F40,
    0x41424344, 0x45464748, 0x494A4B4C, 0x4D4E4F50,
    0x51525354, 0x55565758, 0x595A5B5C, 0x5D5E5F60,
    0x61626364, 0x65666768, 0x696A6B6C, 0x6D6E6F70,
    0x71727374, 0x75767778, 0x797A7B7C, 0x7D7E7F80
};

uint32_t DST_BUF[Buf_Size] = {0};
uint8_t Flag = 0;


// Основная функция
int main(void)
{
    uint8_t i = 0;

    Delay_Init();
    GPIOB_Leds_Init();
    USART1_Init(115200);
    printf("SystemClk:%d\r\n", SystemCoreClock);
    printf("DMA MEM2MEM TEST NL\r\n");

    DMA1_CH3_Init(SRC_BUF, DST_BUF, Buf_Size);
    
    while(DMA_GetFlagStatus(DMA1_FLAG_TC3) == RESET) {}

    Flag = BufCmp(SRC_BUF, DST_BUF, Buf_Size);

    if(Flag == 0) printf("DMA Transfer Fail\r\n");
    else printf("DMA Transfer Success\r\n");

    printf("SRC_BUF:\r\n");
    for(i = 0; i < Buf_Size; i++) printf("0x%08x\r\n", SRC_BUF[i]);

    printf("DST_BUF:\r\n");
    for(i = 0; i < Buf_Size; i++) printf("0x%08x\r\n", DST_BUF[i]);


    while(1)
    {
        Leds_Blink(100);
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
