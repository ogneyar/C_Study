
#ifndef __ST7735_H_
#define __ST7735_H_

#include "defines.h"
#include "system.h"
#include "spi.h"


/* Список функций */
void ST7735_Init(void);
void ST7735_Send(uint8_t dc, uint8_t data);
void ST7735_Clear(void);
void ST7735_Fill(uint16_t color, uint8_t hl, uint8_t hr, uint8_t vt, uint8_t vb);
void ST7735_SetColumnAndPage(uint8_t caset1, uint8_t caset2, uint8_t raset1, uint8_t raset2);
void ST7735_Display(uint16_t color);


#define ST7735_MADCTL_BGR 0x08
#define ST7735_MADCTL_MH 0x04

#define ST7735_FRMCTR1 0xB1
#define ST7735_FRMCTR2 0xB2
#define ST7735_FRMCTR3 0xB3
#define ST7735_INVCTR 0xB4
#define ST7735_DISSET5 0xB6

#define ST7735_PWCTR1 0xC0
#define ST7735_PWCTR2 0xC1
#define ST7735_PWCTR3 0xC2
#define ST7735_PWCTR4 0xC3
#define ST7735_PWCTR5 0xC4
#define ST7735_VMCTR1 0xC5

#define ST7735_PWCTR6 0xFC

#define ST7735_GMCTRP1 0xE0
#define ST7735_GMCTRN1 0xE1

#define ST77XX_NOP 0x00
#define ST77XX_SWRESET 0x01
#define ST77XX_RDDID 0x04
#define ST77XX_RDDST 0x09

#define ST77XX_SLPIN 0x10
#define ST77XX_SLPOUT 0x11
#define ST77XX_PTLON 0x12
#define ST77XX_NORON 0x13

#define ST77XX_INVOFF 0x20
#define ST77XX_INVON 0x21
#define ST77XX_DISPOFF 0x28
#define ST77XX_DISPON 0x29
#define ST77XX_CASET 0x2A
#define ST77XX_RASET 0x2B
#define ST77XX_RAMWR 0x2C
#define ST77XX_RAMRD 0x2E

#define ST77XX_PTLAR 0x30
#define ST77XX_TEOFF 0x34
#define ST77XX_TEON 0x35
#define ST77XX_MADCTL 0x36
#define ST77XX_COLMOD 0x3A

#define ST77XX_MADCTL_MY 0x80
#define ST77XX_MADCTL_MX 0x40
#define ST77XX_MADCTL_MV 0x20
#define ST77XX_MADCTL_ML 0x10
#define ST77XX_MADCTL_RGB 0x00

// Some ready-made 16-bit ('565') color settings:
#define _BLACK 0x0000
#define _WHITE 0xFFFF
#define _RED 0xF800
#define _GREEN 0x001F
#define _BLUE 0x07E0
#define _RED_BLUE 0xFFE0    // розовый
#define _BLUE_GREEN 0x07FF  // голубой
#define _RED_GREEN 0xF81F   // жёлтый

#define LCD_D        1 // data
#define LCD_C        0 // command

#define ST7735_SHIFT  26
#define ST7735_XSTART 0
#define ST7735_YSTART 0
#define ST7735_WIDTH  80
#define ST7735_HEIGHT 160


// Инициализация дисплея
void ST7735_Init(void) 
{
    GPIOA->BCR |= DD_RES; // reset enable
    Delay_Ms(10);
    GPIOA->BSHR |= DD_RES; // reset disable
    Delay_Ms(10);

    GPIOA->BCR |= DD_CS; // chip select enable
    
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

    GPIOA->BSHR |= DD_CS; // chip select disable
}


// Отправка данных
void ST7735_Send(uint8_t dc, uint8_t data)
{
    uint8_t temp;

    if ((GPIOA->INDR & DD_DC) != 0) temp = SET;
    else temp = RESET;

    if (temp != dc) {
        // GPIOA->OUTDR ^= DD_DC;
        if (dc) GPIOA->BSHR |= DD_DC;
        else GPIOA->BCR |= DD_DC;
        Delay_Ms(10);
    }
            
    SPI1_SendData(data);
  
    uint8_t i = 0;
    while( (SPI1_GetFlagStatus(SPI_I2S_FLAG_TXE) == RESET) && (i <= 200) )
    {
        i++;
        if(i > 200) while(1);
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
    GPIOA->BCR |= DD_CS; // chip select enable
        
    ST7735_SetColumnAndPage(ST7735_XSTART + ST7735_SHIFT + hl, ST7735_XSTART + ST7735_WIDTH + ST7735_SHIFT - hr, ST7735_YSTART + vt, ST7735_YSTART + ST7735_HEIGHT - vb);
    
    ST7735_Display(color);

    GPIOA->BSHR |= DD_CS; // chip select disable
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



#endif /* __ST7735_H_ */
