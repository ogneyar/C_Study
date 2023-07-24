
#ifndef _TFT_SHIELD_3_2_H_
#define _TFT_SHIELD_3_2_H_

// #include "defines.h"
// #include "spi.h"
#include <ch32v30x.h>
#include "debug.h"
#include "spi.h"
#include "font.h"


#define PA1     1
#define PA2     2
#define PA3     3
#define PA4     4
#define PA5     5
#define PA6     6
#define PA7     7

// #define DD_CS 		(1 << PA2)
// #define DD_RES 		(1 << PA3)
// #define DD_DC 		(1 << PA4)
// #define DD_CLK 		(1 << PA5)
// #define DD_MISO 	(1 << PA6)
// #define DD_MOSI 	(1 << PA7)

#define WHITE 0xffff
#define BLACK 0x0000


uint8_t rotation = 1;
uint16_t _width = 240;
uint16_t _height = 320;
uint16_t cursor_x = 0, cursor_y = 0;
uint16_t textsize_x = 1, textsize_y = 1;
uint8_t wrap = 0;
uint16_t textcolor = WHITE, textbgcolor = BLACK, dotcolor = WHITE;


/* Список функций */
void Lcd_Init(void);
void Lcd_Write_Bus(unsigned char d);
void Lcd_Write_Com(unsigned char VH);
void Lcd_Write_Data(unsigned char VH);
void Lcd_Write_Com_Data(unsigned char com,unsigned char dat);
void Lcd_Address_Set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);
void Lcd_H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
void Lcd_V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
void Lcd_Rect(unsigned int x,unsigned int y,unsigned int w,unsigned int h,unsigned int c);
void Lcd_Rectf(unsigned int x,unsigned int y,unsigned int w,unsigned int h,unsigned int c);
int Lcd_RGB(int r,int g,int b);
void Lcd_Fill(unsigned int j);
void Lcd_Clear();
void Lcd_PushColors(uint16_t * block, int16_t size);
uint16_t Lcd_Color565(uint8_t r, uint8_t g, uint8_t b);
void Lcd_Dot(unsigned int x, unsigned int y, unsigned int c); 
void Lcd_Print(uint8_t ch, uint16_t color);
void Lcd_Print(const char *ch, uint16_t color);
void Lcd_Print(uint8_t ch);
void Lcd_Print(const char *ch);
void Lcd_DrawChar(uint16_t x, uint16_t y, unsigned char c, uint16_t color, uint16_t bg, uint16_t size_x, uint16_t size_y);
void Lcd_FillSquare(uint16_t color, uint16_t x, uint16_t y, uint16_t width, uint16_t height);
void Lcd_PrintNum(uint32_t number, uint16_t color);
void Lcd_PrintNum(uint32_t number);
void Lcd_SetCursor(uint16_t x, uint16_t y);
void Lcd_SetFontSize(uint8_t size);
void Lcd_SetFontSize(uint8_t sizeX, uint8_t sizeY);
void Lcd_SetFontColor(uint16_t color);
void Lcd_SetFontBgColor(uint16_t color);


/*
	LED  -- PA1
*/


// Инициализация дисплея
void Lcd_Init(void)
{ 
    SPI1_Master_Init(0);    
  
    SPI1->CTLR1 &= ~SPI_BaudRatePrescaler_8;
    SPI1->CTLR1 &= ~SPI_BaudRatePrescaler_128;
    SPI1->CTLR1 |= SPI_BaudRatePrescaler_2;

    /* LED экрана теперь подключен напрямую к питанию */

    // включаем тактирование порта A
    RCC->APB2PCENR |= RCC_APB2Periph_GPIOA; //RCC_APB2PCENR_IOPAEN;    
    // PA1
    GPIOA->CFGLR &= ~(0b1111 << PA1*4); // GPIO_Msk
    GPIOA->CFGLR |= (0b0011 << PA1*4); // GPIO_Speed_50
    GPIOA->BSHR |= (1 << PA1);
	
    // digitalWrite(RESET, HIGH);
    GPIOA->BSHR |= DD_RES;
    Delay_Ms(5); 
    // digitalWrite(RESET, LOW);
    GPIOA->BCR |= DD_RES;
    Delay_Ms(15);
    // digitalWrite(RESET, HIGH);
    GPIOA->BSHR |= DD_RES;
    Delay_Ms(15);

    // digitalWrite(CS, LOW);
    GPIOA->BCR |= DD_CS;

    Lcd_Write_Com(0xCB);  
    Lcd_Write_Data(0x39); 
    Lcd_Write_Data(0x2C); 
    Lcd_Write_Data(0x00); 
    Lcd_Write_Data(0x34); 
    Lcd_Write_Data(0x02); 

    Lcd_Write_Com(0xCF);  
    Lcd_Write_Data(0x00); 
    Lcd_Write_Data(0XC1); 
    Lcd_Write_Data(0X30); 

    Lcd_Write_Com(0xE8);  
    Lcd_Write_Data(0x85); 
    Lcd_Write_Data(0x00); 
    Lcd_Write_Data(0x78); 

    Lcd_Write_Com(0xEA);  
    Lcd_Write_Data(0x00); 
    Lcd_Write_Data(0x00); 
 
    Lcd_Write_Com(0xED);  
    Lcd_Write_Data(0x64); 
    Lcd_Write_Data(0x03); 
    Lcd_Write_Data(0X12); 
    Lcd_Write_Data(0X81); 

    Lcd_Write_Com(0xF7);  
    Lcd_Write_Data(0x20); 
  
    Lcd_Write_Com(0xC0);    //Power control 
    Lcd_Write_Data(0x23);   //VRH[5:0] 
 
    Lcd_Write_Com(0xC1);    //Power control 
    Lcd_Write_Data(0x10);   //SAP[2:0];BT[3:0] 

    Lcd_Write_Com(0xC5);    //VCM control 
    Lcd_Write_Data(0x3e);   //Contrast
    Lcd_Write_Data(0x28); 
 
    Lcd_Write_Com(0xC7);    //VCM control2 
    Lcd_Write_Data(0x86);   //--
 
    Lcd_Write_Com(0x36);    // Memory Access Control 
    Lcd_Write_Data(0x48);   

    Lcd_Write_Com(0x3A);    
    Lcd_Write_Data(0x55); 

    Lcd_Write_Com(0xB1);    
    Lcd_Write_Data(0x00);  
    Lcd_Write_Data(0x18); 
 
    Lcd_Write_Com(0xB6);    // Display Function Control 
    Lcd_Write_Data(0x08); 
    Lcd_Write_Data(0x82);
    Lcd_Write_Data(0x27);  

    Lcd_Write_Com(0x11);    //Exit Sleep 
    Delay_Ms(120); 
				
    Lcd_Write_Com(0x29);    //Display on 
    Lcd_Write_Com(0x2c); 

    // digitalWrite(CS,HIGH);
    GPIOA->BSHR |= DD_CS;
}

//
void Lcd_Write_Bus(unsigned char d)
{
    SPI1_SendData(d);
}

//
void Lcd_Write_Com(unsigned char VH)  
{   
    // digitalWrite(DC, LOW);
    if ( GPIOA->INDR & DD_DC ) GPIOA->BCR |= DD_DC;
    // Delay_Ms(1); 
    Lcd_Write_Bus(VH);
}

//
void Lcd_Write_Data(unsigned char VH)
{
    // digitalWrite(DC, HIGH);
    if ( ! (GPIOA->INDR & DD_DC) ) GPIOA->BSHR |= DD_DC;
    // Delay_Ms(1); 
    Lcd_Write_Bus(VH);
}

//
void Lcd_Write_Com_Data(unsigned char com,unsigned char dat)
{
    Lcd_Write_Com(com);
    Lcd_Write_Data(dat);
}

//
void Lcd_Address_Set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{
    SPI1->CTLR1 &= ~SPI_BaudRatePrescaler_8;
    SPI1->CTLR1 &= ~SPI_BaudRatePrescaler_128;
    SPI1->CTLR1 |= SPI_BaudRatePrescaler_2;

    Lcd_Write_Com(0x2a);
	Lcd_Write_Data(x1>>8);
	Lcd_Write_Data(x1);
	Lcd_Write_Data(x2>>8);
	Lcd_Write_Data(x2);
    Lcd_Write_Com(0x2b);
	Lcd_Write_Data(y1>>8);
	Lcd_Write_Data(y1);
	Lcd_Write_Data(y2>>8);
	Lcd_Write_Data(y2);

	Lcd_Write_Com(0x2c); //write_memory_start					 
}

//
void Lcd_H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c)             
{	
    SPI1->CTLR1 &= ~SPI_BaudRatePrescaler_8;
    SPI1->CTLR1 &= ~SPI_BaudRatePrescaler_128;
    SPI1->CTLR1 |= SPI_BaudRatePrescaler_2;

    uint32_t j;
    
    GPIOA->BCR |= DD_CS;

    // Lcd_Write_Com(0x02c); //write_memory_start
    l=l+x;
    Lcd_Address_Set(x, y, l, y);
    j=l*2;
    for(uint32_t i = 1; i <= j; i++)
    {
        Lcd_Write_Data(c);
    }
    
    GPIOA->BSHR |= DD_CS; 
}

//
void Lcd_V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c)
{	
    SPI1->CTLR1 &= ~SPI_BaudRatePrescaler_8;
    SPI1->CTLR1 &= ~SPI_BaudRatePrescaler_128;
    SPI1->CTLR1 |= SPI_BaudRatePrescaler_2;

    uint32_t j;
    
    GPIOA->BCR |= DD_CS;

    // Lcd_Write_Com(0x02c); //write_memory_start
    l=l+y;
    Lcd_Address_Set(x, y, x, l);
    j=l*2;
    for(uint32_t i = 1; i <= j; i++)
    { 
        Lcd_Write_Data(c);
    }
    
    GPIOA->BSHR |= DD_CS;
}

//
void Lcd_Rect(unsigned int x,unsigned int y,unsigned int w,unsigned int h,unsigned int c)
{
    Lcd_H_line(x  , y  , w, c);
    Lcd_H_line(x  , y+h, w, c);
    Lcd_V_line(x  , y  , h, c);
    Lcd_V_line(x+w, y  , h, c);
}

//
void Lcd_Rectf(unsigned int x,unsigned int y,unsigned int w,unsigned int h,unsigned int c)
{
    for(uint32_t i = 0; i < h; i++)
    {
        Lcd_H_line(x  , y  , w, c);
        Lcd_H_line(x  , y+i, w, c);
    }
}

//
int Lcd_RGB(int r,int g,int b)
{
    return (int)((r << 16) | (g << 8) | b);
}

//
void Lcd_Fill(unsigned int j)
{	
    GPIOA->BCR |= DD_CS;
    Lcd_Address_Set(0, 0, _width, _height);
    for(uint32_t i = 0; i < _width; i++)
        for(uint32_t m = 0; m < _height; m++)
        {
            Lcd_Write_Data(j>>8);
            Lcd_Write_Data(j);
        }
    GPIOA->BSHR |= DD_CS;
}

//
void Lcd_Clear()
{	
    Lcd_Fill(0);
}

//
void Lcd_PushColors(uint16_t * block, int16_t size)
{
    GPIOA->BCR |= DD_CS;
    for(uint32_t i = 0; i < size; i++)
    {
        Lcd_Write_Data(block[i] >> 8);        
        Lcd_Write_Data(block[i]);
    }
    GPIOA->BSHR |= DD_CS;
}

//
uint16_t Lcd_Color565(uint8_t r, uint8_t g, uint8_t b)
{ 
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3); 
}


//
void Lcd_Dot(unsigned int x, unsigned int y, unsigned int c)          
{	
    SPI1->CTLR1 &= ~SPI_BaudRatePrescaler_8;
    SPI1->CTLR1 &= ~SPI_BaudRatePrescaler_128;
    SPI1->CTLR1 |= SPI_BaudRatePrescaler_2;

    GPIOA->BCR |= DD_CS;

    Lcd_Write_Com(0x02c); //write_memory_start  

    // Lcd_Address_Set(x-1, y-1, x+1, y+1);
    Lcd_Address_Set(x, y, x, y);

    Lcd_Write_Data(c>>8);
    Lcd_Write_Data(c);

    GPIOA->BSHR |= DD_CS;     
}

//
void Lcd_Print(uint8_t ch, uint16_t color)
{
    if (ch == '\n') {               // Newline?
        cursor_x = 0;         // Reset x to zero,
        cursor_y += textsize_y * 8; // advance y one line
    } else if (ch != '\r') {        // Ignore carriage returns
        if (wrap && ((cursor_x + textsize_x * 6) > _width)) { // Off right?
            cursor_x = 0;     // Reset x to zero,
            cursor_y += textsize_y * 8; // advance y one line
        }
        // Lcd_Address_Set(0, 0, _width, _height);
        Lcd_DrawChar(cursor_x, cursor_y, ch, color, textbgcolor, textsize_x, textsize_y);
        cursor_x += textsize_x * 6; // Advance x one char
    }
}

//
void Lcd_Print(const char *ch, uint16_t color)
{
    const char * ptrCh = ch;
    while(*ptrCh != '\0') {
        Lcd_Print(*ptrCh, color);
        ptrCh++;
    }
}

//
void Lcd_Print(uint8_t ch)
{
    Lcd_Print(ch, textcolor);
}

//
void Lcd_Print(const char *ch)
{
    Lcd_Print(ch, textcolor);
}


//
void Lcd_DrawChar(uint16_t x, uint16_t y, unsigned char c, uint16_t color, uint16_t bg, uint16_t size_x, uint16_t size_y)
{
    const GFXfont *gfxFont = &font;

    c -= (uint8_t)gfxFont->first;

    GFXglyph glyph = read_glyph(gfxFont, c);
    uint8_t *bitmap = read_bitmap(gfxFont);

    uint16_t bo = glyph.bitmapOffset;
    uint8_t w = glyph.width, h = glyph.height;

    // добавил эти строки чтобы координаты символа x = 0, y = 0, вмещали символ полностью на экране 
    x += w;
    y += (size_y * h);

    int8_t xo = glyph.xOffset,
           yo = glyph.yOffset;
    uint8_t xx, yy, bits = 0, bit = 0;
    uint16_t xo16 = 0, yo16 = 0;

    if (size_x > 1 || size_y > 1) {
        xo16 = xo;
        yo16 = yo;
    }

    for (yy = 0; yy < h; yy++) {
        for (xx = 0; xx < w; xx++) {
            if (!(bit++ & 7)) {
                bits = bitmap[bo++];
            }
            if (bits & 0x80) {
                if (size_x == 1 && size_y == 1) {
                    Lcd_Dot(x + xo + xx, y + yo + yy, color);
                } else {
                    Lcd_FillSquare(color, x + (xo16 + xx) * size_x, y + (yo16 + yy) * size_y, size_x, size_y);
                }
            }
            bits <<= 1;
        }
    }
}

//
void Lcd_FillSquare(uint16_t color, uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    SPI1->CTLR1 &= ~SPI_BaudRatePrescaler_8;
    SPI1->CTLR1 &= ~SPI_BaudRatePrescaler_128;
    SPI1->CTLR1 |= SPI_BaudRatePrescaler_2;

    GPIOA->BCR |= DD_CS;

    Lcd_Write_Com(0x02c); //write_memory_start    
    // Lcd_Address_Set(x, y, x + width, y + height);
    Lcd_Address_Set(x, y - height, x + width, y);

    for(uint32_t i = 0; i < ((width+1) * (height+1)); i++)
    {
        Lcd_Write_Data(color >> 8);
        Lcd_Write_Data(color);
    }

    GPIOA->BSHR |= DD_CS;   

}


//
void Lcd_PrintNum(uint32_t number, uint16_t color)
{
    if (number <= 9) { 
        Lcd_Print(number + 0x30, color);
    }else if (number > 9 && number < 100) {
        Lcd_Print(number/10 + 0x30, color);
        Lcd_Print( number - ((number/10) * 10) + 0x30, color);
    }else if (number > 99 && number < 1000) {
        uint8_t temp = number / 100;
        Lcd_Print(temp + 0x30, color);
        uint8_t temp2 = ( number - temp*100 )  / 10;
        Lcd_Print(temp2 + 0x30, color);
        uint8_t temp3 = number - temp*100 - temp2*10;
        Lcd_Print(temp3 + 0x30, color);
    }else if (number > 999 && number < 10000) {
        uint8_t temp = number / 1000;
        Lcd_Print(temp + 0x30, color);
        uint8_t temp2 = ( number - temp*1000 )  / 100;
        Lcd_Print(temp2 + 0x30, color);
        uint8_t temp3 = (number - temp*1000 - temp2*100) / 10;
        Lcd_Print(temp3 + 0x30, color);
        uint8_t temp4 = number - temp*1000 - temp2*100 - temp3*10;
        Lcd_Print(temp4 + 0x30, color);
    }
}

//
void Lcd_PrintNum(uint32_t number)
{  
    Lcd_PrintNum(number, textcolor);
}

//
void Lcd_SetCursor(uint16_t x, uint16_t y)
{
    cursor_x = x; // x+5;
    cursor_y = y; // y+10;
}

//
void Lcd_SetFontSize(uint8_t size)
{  
    textsize_x = size;
    textsize_y = size;
}

//
void Lcd_SetFontSize(uint8_t sizeX, uint8_t sizeY)
{  
    if (rotation == 1) {
        textsize_x = sizeY;
        textsize_y = sizeX;
    }else {    
        textsize_x = sizeX;
        textsize_y = sizeY;
    }
}

//
void Lcd_SetFontColor(uint16_t color)
{
    textcolor = color;
}

//
void Lcd_SetFontBgColor(uint16_t color)
{
    textbgcolor = color;
}




#endif  /* _TFT_SHIELD_3_2_H_ */
