
#include "xpt2046.h"


GPIO_TypeDef * _portXPT;
uint8_t _chipSelectPinXPT;


// Инициализация XPT2046
void XPT_Init(GPIO_TypeDef *port, uint8_t chipSelectPin)
{
    _portXPT = port;
    _chipSelectPinXPT = chipSelectPin;
    
    SPI1_Master_Init(0);

    SPI1->CTLR1 &= ~SPI_BaudRatePrescaler_2;
    SPI1->CTLR1 &= ~SPI_BaudRatePrescaler_8;
    SPI1->CTLR1 |= SPI_BaudRatePrescaler_128;
    
    if (port == GPIOA) RCC->APB2PCENR |= RCC_APB2Periph_GPIOA;
    
    // пока подразумевается PA1
    port->CFGLR &= ~(0b1111 << chipSelectPin*4); // GPIO_Msk
    port->CFGLR |= (0b0011 << chipSelectPin*4); // GPIO_Speed_50
    port->BSHR |= (1 << chipSelectPin);

    XPT_ChipSelectLow();

    SPI1_Send(0x80);
    SPI1_Send(0x00);
    SPI1_Send(0x00);
    
	Delay_Ms(10);

    XPT_ChipSelectHigh();    
} 


//
void XPT_ChipSelectLow(void)
{
    _portXPT->BCR |= (1 << _chipSelectPinXPT);
}


//
void XPT_ChipSelectHigh(void)
{
    _portXPT->BSHR |= (1 << _chipSelectPinXPT);
}


//
void XPT_GetTouch_xy(volatile uint16_t *x_kor,volatile uint16_t *y_kor)
{
	volatile uint16_t touch_x = 0;
	volatile uint16_t touch_y = 0;

    SPI1->CTLR1 &= ~SPI_BaudRatePrescaler_2;
    SPI1->CTLR1 &= ~SPI_BaudRatePrescaler_8;
    SPI1->CTLR1 |= SPI_BaudRatePrescaler_128;

    XPT_ChipSelectLow();

    SPI1_Send(chx);  //отправляем запрос координаты X
	Delay_Us(100);
	SPI1_Transmit(0x00); //отправляем пустой запрос
	Delay_Us(100);
	touch_x = SPI1_Transmit(0x00); //получаем старшие 8 бит
	touch_x <<= 8;
	Delay_Us(100);
    touch_x |= SPI1_Receive(); //получаем младшие 4 бита
	touch_x >>= 3;

	Delay_Us(100);

	SPI1_Send(chy); //отправляем запрос координаты Y
	Delay_Us(100);
	SPI1_Transmit(0x00); //отправляем пустой запрос
	Delay_Us(100);
	touch_y = SPI1_Transmit(0x00); //получаем старшие 8 бит
	touch_y <<= 8;
	Delay_Us(100);
    touch_y |= SPI1_Receive(); //получаем младшие 4 бита
	touch_y >>= 3;
	Delay_Us(100);
	
#ifndef calibr
	touch_x -= Xmin;
	// touch_x = 240 - touch_x/((Xmax-Xmin)/240);
	touch_x = touch_x/((Xmax-Xmin)/240);
	touch_y -= Ymin;
	touch_y = 320 - touch_y/((Ymax-Ymin)/320);  
#endif

	*x_kor = touch_x;
	*y_kor = touch_y; 

    // printf("touch_x: %d\r\n", touch_x);
    // printf("touch_x hex: 0x%x\r\n", touch_x);
    // printf("touch_y: %d\r\n", touch_y);
    // printf("touch_y hex: 0x%x\r\n", touch_y);
    

    XPT_ChipSelectHigh();
}

