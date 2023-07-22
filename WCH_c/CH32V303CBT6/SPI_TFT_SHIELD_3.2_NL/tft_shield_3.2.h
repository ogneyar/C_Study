
#ifndef _TFT_SHIELD_3_2_H_
#define _TFT_SHIELD_3_2_H_

#include "defines.h"
#include "spi.h"


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

/*
	LED  -- PA1
*/


// Инициализация дисплея
void Lcd_Init(void)
{ 
    // включаем тактирование порта A
    RCC->APB2PCENR |= RCC_APB2PCENR_IOPAEN;    
    // PA1
    GPIOA->CFGLR &= ~(GPIO_Msk << PA1*4); // ~(0b1111);
    GPIOA->CFGLR |= (GPIO_Speed_50 << PA1*4); // 0b0011;
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
	Lcd_Write_Com(0x2c); 							 
}

//
void Lcd_H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c)             
{	
    unsigned int i,j;
    // digitalWrite(CS,LOW);
    GPIOA->BCR |= DD_CS;
    Lcd_Write_Com(0x02c); //write_memory_start
    l=l+x;
    Lcd_Address_Set(x,y,l,y);
    j=l*2;
    for(i=1;i<=j;i++)
    {
        Lcd_Write_Data(c);
    }
    // digitalWrite(CS,HIGH);
    GPIOA->BSHR |= DD_CS; 
}

//
void Lcd_V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c)
{	
    unsigned int i,j;
    // digitalWrite(CS,LOW);
    GPIOA->BCR |= DD_CS;
    Lcd_Write_Com(0x02c); //write_memory_start
    l=l+y;
    Lcd_Address_Set(x,y,x,l);
    j=l*2;
    for(i=1;i<=j;i++)
    { 
        Lcd_Write_Data(c);
    }
    // digitalWrite(CS,HIGH);
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
    unsigned int i;
    for(i=0;i<h;i++)
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
    unsigned int i,m;
    // digitalWrite(CS,LOW);
    GPIOA->BCR |= DD_CS;
    Lcd_Address_Set(0,0,240,320);
    for(i=0;i<240;i++)
        for(m=0;m<320;m++)
        {
        Lcd_Write_Data(j>>8);
        Lcd_Write_Data(j);
        }
    // digitalWrite(CS,HIGH);
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
    Lcd_Address_Set(x-1, y-1, x+1, y+1);
    for(uint32_t i = 0; i < 9; i++)
    {
        Lcd_Write_Data(c>>8);
        Lcd_Write_Data(c);
    }

    GPIOA->BSHR |= DD_CS;     
}


#endif  /* _TFT_SHIELD_3_2_H_ */
