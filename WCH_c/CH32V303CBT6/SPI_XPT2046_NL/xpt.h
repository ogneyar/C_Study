
#ifndef __XPT_H_
#define __XPT_H_

#include "xpt_defines.h"

#define	chy 	0x90 // координатные оси дисплея и тачскрина поменяны местами(там где у дисплея Х у тачсрина Y)
#define	chx 	0xD0 // за основу возьмём оси дисплея


void XPT_Init(GPIO_TypeDef *port, uint8_t chipSelectPin);
void XPT_ChipSelectLow(void);
void XPT_ChipSelectHigh(void);
void XPT_GetTouch_xy(volatile uint16_t *x_kor,volatile uint16_t *y_kor);


// Инициализация XPT2046
void XPT_Init(GPIO_TypeDef *port, uint8_t chipSelectPin)
{
    _port = port;
    _chipSelectPin = chipSelectPin;
    
    SPI1_Master_Init(0);

    SPI1->CTLR1 &= ~SPI_BaudRatePrescaler_2;
    SPI1->CTLR1 &= ~SPI_BaudRatePrescaler_8;
    SPI1->CTLR1 |= SPI_BaudRatePrescaler_256;
    
    if (port == GPIOA) RCC->APB2PCENR |= RCC_APB2PCENR_IOPAEN;
    
    // пока подразумевается PA1
    port->CFGLR &= ~(GPIO_Msk << chipSelectPin*4); // ~(0b1111);
    port->CFGLR |= (GPIO_Speed_50 << chipSelectPin*4); // 0b0011;
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
    _port->BCR |= (1 << _chipSelectPin);
}


//
void XPT_ChipSelectHigh(void)
{
    _port->BSHR |= (1 << _chipSelectPin);
}


//
void XPT_GetTouch_xy(volatile uint16_t *x_kor,volatile uint16_t *y_kor)
{
	volatile uint16_t touch_x = 0;
	volatile uint16_t touch_y = 0;

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
	
	*x_kor = touch_x;
	*y_kor = touch_y; 

    // printf("touch_x: %d\r\n", touch_x);
    // printf("touch_x hex: 0x%x\r\n", touch_x);
    // printf("touch_y: %d\r\n", touch_y);
    // printf("touch_y hex: 0x%x\r\n", touch_y);

    XPT_ChipSelectHigh();
}



#endif /* __XPT_H_ */
