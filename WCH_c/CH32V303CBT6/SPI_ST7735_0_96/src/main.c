
/*
 *@Note
 SPI interface operation flash peripheral routine:
 Master:SPI1_SCK(PA5)、SPI1_MISO(PA6)、SPI1_MOSI(PA7).
 This example demonstrates SPI operation Winbond W25Qxx SPIFLASH.

 // НЕ RGB А RBG !!!

 pins:
    CS   -- PA2
    RES  -- PA3
    DC   -- PA4
    CLK  -- PA5
    MISO -- PA6
    MOSI -- PA7
*/

#define DD_CS 		GPIO_Pin_2
#define DD_RES 		GPIO_Pin_3
#define DD_DC 		GPIO_Pin_4
#define DD_CLK 		GPIO_Pin_5
#define DD_MISO 	GPIO_Pin_6
#define DD_MOSI 	GPIO_Pin_7

// максимальная скорость 144МГц / 4 или 72МГц / 2 = 36МГц

#include "debug.h"
#include <ch32v30x.h>

#include "defines.h"

/* Список функций */
void SPI_Master_Init(void);
void ST7735_Init(void);
void ST7735_Send(uint8_t dc, uint8_t data);
void ST7735_Clear(void);
void ST7735_Fill(uint16_t color, uint8_t hl, uint8_t hr, uint8_t vt, uint8_t vb);
void ST7735_SetColumnAndPage(uint8_t caset1, uint8_t caset2, uint8_t raset1, uint8_t raset2);
void ST7735_Display(uint16_t color);
void TestScreen(void);


/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{
    Delay_Init();
    USART_Printf_Init(115200);
    printf("SystemClk:%d\r\n", SystemCoreClock);

    SPI_Master_Init();    
    Delay_Ms(100);

    ST7735_Init();    

    while(1) 
    {
        TestScreen();
    }
}


/*********************************************************************
 * @fn      SPI_Master_Init
 *
 * @brief   Configuring the SPI for operation flash.
 *
 * @return  none
 */
void SPI_Master_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    SPI_InitTypeDef  SPI_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1, ENABLE);
    // PA2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_SetBits(GPIOA, GPIO_Pin_2);
    // PA3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_SetBits(GPIOA, GPIO_Pin_3);
    // PA4
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_SetBits(GPIOA, GPIO_Pin_4);
    // PA5
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    // PA6
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    // PA7
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI1, &SPI_InitStructure);

    SPI_Cmd(SPI1, ENABLE);
}


// Инициализация дисплея
void ST7735_Init(void) 
{
    GPIO_ResetBits(GPIOA, DD_RES); // reset enable
    Delay_Ms(10);
    GPIO_SetBits(GPIOA, DD_RES); // reset disable
    Delay_Ms(10);

    GPIO_ResetBits(GPIOA, DD_CS); // chip select enable
    
    ST7735_Send(LCD_C, ST77XX_SWRESET); // 0x01 //  1: Software reset, 0 args, w/delay
    Delay_Ms(150);
    ST7735_Send(LCD_C, ST77XX_SLPOUT); // 0x11 //  2: Out of sleep mode, 0 args, w/delay
    Delay_Ms(255);

    ST7735_Send(LCD_C, ST77XX_INVON); // 0x21 // 13: Invert display, no args

    ST7735_Send(LCD_C, ST77XX_COLMOD); // 0x3A // 15: set color mode, 1 arg
    ST7735_Send(LCD_D, 0x05); // 16-bit/pixel     
    
    ST7735_Send(LCD_C, ST7735_GMCTRP1); // 0xE0 //  1: Gamma Adjustments (pos. polarity), 16 args 
    ST7735_Send(LCD_D, 0x02);
    ST7735_Send(LCD_D, 0x1C);
    ST7735_Send(LCD_D, 0x07);
    ST7735_Send(LCD_D, 0x12);
    ST7735_Send(LCD_D, 0x37);
    ST7735_Send(LCD_D, 0x32);
    ST7735_Send(LCD_D, 0x29);
    ST7735_Send(LCD_D, 0x2D);
    ST7735_Send(LCD_D, 0x29);
    ST7735_Send(LCD_D, 0x25);
    ST7735_Send(LCD_D, 0x2B);
    ST7735_Send(LCD_D, 0x39);
    ST7735_Send(LCD_D, 0x00);
    ST7735_Send(LCD_D, 0x01);
    ST7735_Send(LCD_D, 0x03);
    ST7735_Send(LCD_D, 0x10);     

    ST7735_Send(LCD_C, ST7735_GMCTRN1); // 0xE1 //  2: Gamma Adjustments (neg. polarity), 16 args   
    ST7735_Send(LCD_D, 0x03);
    ST7735_Send(LCD_D, 0x1D);
    ST7735_Send(LCD_D, 0x07);
    ST7735_Send(LCD_D, 0x06);
    ST7735_Send(LCD_D, 0x2E);
    ST7735_Send(LCD_D, 0x2C);
    ST7735_Send(LCD_D, 0x29);
    ST7735_Send(LCD_D, 0x2D);
    ST7735_Send(LCD_D, 0x2E);
    ST7735_Send(LCD_D, 0x2E);
    ST7735_Send(LCD_D, 0x37);
    ST7735_Send(LCD_D, 0x3F);
    ST7735_Send(LCD_D, 0x00);
    ST7735_Send(LCD_D, 0x00);
    ST7735_Send(LCD_D, 0x02);
    ST7735_Send(LCD_D, 0x10);     

    ST7735_Send(LCD_C, ST77XX_DISPON); // 0x29 //  4: Main screen turn on, no args w/delay
    Delay_Ms(100);

    GPIO_SetBits(GPIOA, DD_CS); // chip select disable
}


// Отправка данных
void ST7735_Send(uint8_t dc, uint8_t data)
{
    uint8_t temp = GPIO_ReadInputDataBit(GPIOA, DD_DC);

    if (temp != dc) {
        GPIO_WriteBit(GPIOA, DD_DC, !temp);
        // Delay_Us(100);
    }
        
    SPI_I2S_SendData(SPI1, data);
  
    uint8_t i = 0;
    while( (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) && (i <= 200) )
    {
        i++;
        // if(i > 200) while(1);
    }
}


// Очистка дисплея
void ST7735_Clear(void)
{
    ST7735_Fill(_BLACK, 0, 0, 0, 0);
}


// Заливка цветом
void ST7735_Fill(uint16_t color, uint8_t hl, uint8_t hr, uint8_t vt, uint8_t vb) // hl - horisontal left,  hr - horisontal right, vt - vertical top,  vb - vertical bottom
{
    GPIO_ResetBits(GPIOA, DD_CS); // chip select enable
        
    ST7735_SetColumnAndPage(ST7735_XSTART + ST7735_SHIFT + hl, ST7735_XSTART + ST7735_WIDTH + ST7735_SHIFT - hr, ST7735_YSTART + vt, ST7735_YSTART + ST7735_HEIGHT - vb);
    
    ST7735_Display(color);

    GPIO_SetBits(GPIOA, DD_CS); // chip select disable
}


// Установка координат
void ST7735_SetColumnAndPage(uint8_t caset1, uint8_t caset2, uint8_t raset1, uint8_t raset2)
{
    ST7735_Send(LCD_C, ST77XX_CASET); // 0x2A 
    ST7735_Send(LCD_D, 0x00); // x1 high - начало
    ST7735_Send(LCD_D, caset1); // x1 low   // + 26 shift
    ST7735_Send(LCD_D, 0x00); // x2 high - конец
    ST7735_Send(LCD_D, caset2); // x2 low   // 80 horizontal + 26 shift 

    ST7735_Send(LCD_C, ST77XX_RASET); // 0x2B 
    ST7735_Send(LCD_D, 0x00); // y1 high - начало
    ST7735_Send(LCD_D, raset1); // y1 low
    ST7735_Send(LCD_D, 0x00); // y2 high - конец
    ST7735_Send(LCD_D, raset2); // y2 low  // 160 vertical 
}


// Прорисовка дисплея
void ST7735_Display(uint16_t color) 
{
    uint8_t c1 = (uint8_t)((color & 0x00ff));
    uint8_t c2 = (color >> 8);

    ST7735_Send(LCD_C, ST77XX_RAMWR); // 0x2C 
    
    for (uint16_t i=0; i < (uint16_t)13040; i++)
    {
        ST7735_Send(LCD_D, c1);
        ST7735_Send(LCD_D, c2);
    }    
}


// tests
void TestScreen(void)
{    
    ST7735_Clear();    
    Delay_Ms(1000);

    ST7735_Fill(_WHITE, 5, 5, 10, 10);    
    Delay_Ms(1000);

    ST7735_Fill(_RED, 10, 10, 20, 20);    
    Delay_Ms(1000);

    ST7735_Fill(_GREEN, 15, 15, 30, 30);    
    Delay_Ms(1000);

    ST7735_Fill(_BLUE, 20, 20, 40, 40);    
    Delay_Ms(1000);

    ST7735_Fill(_RED_BLUE, 25, 25, 50, 50);    
    Delay_Ms(1000);

    ST7735_Fill(_BLUE_GREEN, 30, 30, 60, 60);    
    Delay_Ms(1000);

    ST7735_Fill(_RED_GREEN, 35, 35, 70, 70);
    Delay_Ms(1000);
}


