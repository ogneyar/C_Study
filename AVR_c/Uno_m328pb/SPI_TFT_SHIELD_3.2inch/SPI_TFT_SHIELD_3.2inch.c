

// НЕ РАБОТАЕТ!!!



#include <avr/io.h>
#ifndef _AVR_IOXXX_H_
#include <avr/iom328pb.h>
#endif 
#include <util/delay.h>

#include "../libs/tft_shield_3.2.h"
#include "../libs/spi.h"
#include "../libs/defines.h"


#define INPUT   0    
#define OUTPUT  1    
#define LOW     0    
#define HIGH    1    

// #define LED   7    
#define CS    10        
#define RS    9       
#define RESET 8

/* Список функций */
void Lcd_Write_Bus(unsigned char d);
void Lcd_Write_Com(unsigned char VH);
void Lcd_Write_Data(unsigned char VH);
void Lcd_Write_Com_Data(unsigned char com,unsigned char dat);
void Lcd_Address_Set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);
void Lcd_Init(void);
void Lcd_H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
void Lcd_V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
void Lcd_Rect(unsigned int x,unsigned int y,unsigned int w,unsigned int h,unsigned int c);
void Lcd_Rectf(unsigned int x,unsigned int y,unsigned int w,unsigned int h,unsigned int c);
int Lcd_RGB(int r,int g,int b);
void Lcd_Fill(unsigned int j);
void Lcd_Clear();



void pinMode(uint8_t pin, uint8_t mode) {
    if (mode) DDRB |= (1 << (pin-8));
    else DDRB &= ~(1 << (pin-8));
}

void digitalWrite(uint8_t pin, uint8_t mode) {
    if (mode) PORTB |= (1 << (pin-8));
    else PORTB &= ~(1 << (pin-8));
}

int main(void) 
{
    // display_init(address);
    
    SPI_Init(0);
    // pinMode(LED, OUTPUT);
    pinMode(RS,OUTPUT);
    pinMode(RESET,OUTPUT);
    pinMode(CS,OUTPUT);

    // digitalWrite(LED, HIGH);
    digitalWrite(RS, HIGH);
    digitalWrite(RESET, HIGH);
    digitalWrite(CS, HIGH);

    Lcd_Init();

    while(1) {
        
        Lcd_Fill(0xf800);
        Lcd_Fill(0x07E0);
        Lcd_Fill(0x001F);
        Lcd_Clear(); 
        // for(int i=0;i<500;i++)
        // {
        //     Rect(random(300),random(300),random(300),random(300),random(65535)); // rectangle at x, y, with, hight, color
        // }
        _delay_ms(1000);
    }

    return 0;  
}


//
void Lcd_Write_Bus(unsigned char d)
{
    SPI_Transmit(d);
}

//
void Lcd_Write_Com(unsigned char VH)  
{   
    // *(portOutputRegister(digitalPinToPort(RS))) &=  ~digitalPinToBitMask(RS);//LCD_RS=0;
    digitalWrite(RS, LOW);
    Lcd_Write_Bus(VH);
}

//
void Lcd_Write_Data(unsigned char VH)
{
    // *(portOutputRegister(digitalPinToPort(RS)))|=  digitalPinToBitMask(RS);//LCD_RS=1;
    digitalWrite(RS, HIGH);
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

// Инициализация дисплея
void Lcd_Init(void)
{
    digitalWrite(RESET, HIGH);
    _delay_ms(5); 
    digitalWrite(RESET, LOW);
    _delay_ms(15);
    digitalWrite(RESET, HIGH);
    _delay_ms(15);

    digitalWrite(CS,LOW);  //CS

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
    _delay_ms(120); 
				
    Lcd_Write_Com(0x29);    //Display on 
    Lcd_Write_Com(0x2c); 
    digitalWrite(CS,HIGH);
}

//
void Lcd_H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c)             
{	
    unsigned int i,j;
    digitalWrite(CS,LOW);
    Lcd_Write_Com(0x02c); //write_memory_start
    l=l+x;
    Lcd_Address_Set(x,y,l,y);
    j=l*2;
    for(i=1;i<=j;i++)
    {
        Lcd_Write_Data(c);
    }
    digitalWrite(CS,HIGH);   
}

//
void Lcd_V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c)
{	
    unsigned int i,j;
    digitalWrite(CS,LOW);
    Lcd_Write_Com(0x02c); //write_memory_start
    l=l+y;
    Lcd_Address_Set(x,y,x,l);
    j=l*2;
    for(i=1;i<=j;i++)
    { 
        Lcd_Write_Data(c);
    }
    digitalWrite(CS,HIGH);   
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
    digitalWrite(CS,LOW);
    Lcd_Address_Set(0,0,240,320);
    for(i=0;i<240;i++)
        for(m=0;m<320;m++)
        {
        Lcd_Write_Data(j>>8);
        Lcd_Write_Data(j);
        }
    digitalWrite(CS,HIGH);   
}

//
void Lcd_Clear()
{	
    Lcd_Fill(0);
}

