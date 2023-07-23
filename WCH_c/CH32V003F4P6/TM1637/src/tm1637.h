
#ifndef __TM1637_H_
#define __TM1637_H_

#include "stdio.h"
#include "debug.h"
#include <ch32v00x.h>

#define INPUT 0
#define OUTPUT 1
#define LOW 0
#define HIGH 1

#define ADDR_AUTO 0x40
#define ADDR_FIXED 0x44
#define STARTADDR 0xc0


GPIO_TypeDef *_GPIOx;
uint8_t digit[10] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6f };  // символы с 0 по 9
uint8_t Cmd_DispCtrl = 0;  //0x88;
uint8_t _DispCLKpin = 5;
uint8_t _DispDIOpin = 6;
uint8_t points = 0;


// Список функций
void pinMode(uint8_t pin, uint8_t state);
void digitalWrite(uint8_t pin, uint8_t state);
uint8_t digitalRead(uint8_t pin);
void tm1637_init(GPIO_TypeDef *GPIOx, uint8_t clk_pin, uint8_t dio_pin);
void tm1637_start(void);
void tm1637_stop(void);
int tm1637_writeByte(uint8_t wr_data);
void tm1637_display(uint8_t n0, uint8_t n1, uint8_t n2, uint8_t n3);
void tm1637_brightness(uint8_t bri);
void tm1637_point(uint8_t pointFlag);
void tm1637_sendOne(uint8_t number, uint8_t data);
void tm1637_sendAll(uint8_t data[]);
void tm1637_clear(void);



// 
void pinMode(uint8_t pin, uint8_t state)
{
    uint8_t RCC_APB2Periph_GPIOx = RCC_APB2Periph_GPIOA;
    if (_GPIOx == GPIOC) RCC_APB2Periph_GPIOx = RCC_APB2Periph_GPIOC;
    if (_GPIOx == GPIOD) RCC_APB2Periph_GPIOx = RCC_APB2Periph_GPIOD;
    
    GPIO_InitTypeDef GPIO_InitStructure={0};

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOx, ENABLE );

    GPIO_InitStructure.GPIO_Pin = pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    if (state) {    
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    }else {
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    }

    GPIO_Init( _GPIOx, &GPIO_InitStructure );
}


// 
void digitalWrite(uint8_t pin, uint8_t state)
{
    if (state) _GPIOx->BSHR |= pin;
    else _GPIOx->BCR |= pin;
    // Delay_Us(50);
}


// 
uint8_t digitalRead(uint8_t pin)
{
    // Delay_Us(50);
    return GPIO_ReadInputDataBit(_GPIOx, pin);
}

// 
void tm1637_init(GPIO_TypeDef *GPIOx, uint8_t clk_pin, uint8_t dio_pin)
{
    _GPIOx = GPIOx;
    _DispCLKpin = clk_pin;
    _DispDIOpin = dio_pin;
    pinMode(_DispCLKpin, OUTPUT);
    pinMode(_DispDIOpin, OUTPUT);
}


//send start signal
void tm1637_start(void)
{
    digitalWrite(_DispCLKpin, HIGH);
    digitalWrite(_DispDIOpin, HIGH);
    digitalWrite(_DispDIOpin, LOW);
    digitalWrite(_DispCLKpin, LOW);
}


//End of transmission
void tm1637_stop(void)
{
    digitalWrite(_DispCLKpin, LOW);
    digitalWrite(_DispDIOpin, LOW);
    digitalWrite(_DispCLKpin, HIGH);
    digitalWrite(_DispDIOpin, HIGH);
}

// 
int tm1637_writeByte(uint8_t wr_data)
{
    uint8_t i;
    for (i = 0; i < 8; i++)  //sent 8bit data
    {
        digitalWrite(_DispCLKpin, LOW);
        if (wr_data & 0x01) digitalWrite(_DispDIOpin, HIGH);  //LSB first
        else digitalWrite(_DispDIOpin, LOW);
        wr_data >>= 1;
        digitalWrite(_DispCLKpin, HIGH);
    }
    digitalWrite(_DispCLKpin, LOW);  //wait for the ACK
    digitalWrite(_DispDIOpin, HIGH);
    digitalWrite(_DispCLKpin, HIGH);
    pinMode(_DispDIOpin, INPUT);

    Delay_Us(50);
    uint8_t ack = digitalRead(_DispDIOpin);
    if (ack == 0) {
        pinMode(_DispDIOpin, OUTPUT);
        digitalWrite(_DispDIOpin, LOW);
    }
    Delay_Us(50);
    pinMode(_DispDIOpin, OUTPUT);
    Delay_Us(50);

    return ack;
}


//
void tm1637_display(uint8_t n0, uint8_t n1, uint8_t n2, uint8_t n3)
{
    tm1637_sendOne(1, digit[n0]);
    tm1637_sendOne(2, digit[n1]);
    tm1637_sendOne(3, digit[n2]);
    tm1637_sendOne(4, digit[n3]);
}


// установка яркости экрана
void tm1637_brightness(uint8_t bri)
{
    Cmd_DispCtrl = 0x88 + bri;
}


// показывать ли точки
void tm1637_point(uint8_t pointFlag)
{
    points = pointFlag;
}


// функция отправки данных на дисплей
void tm1637_sendOne(uint8_t number, uint8_t data)
{
    tm1637_start();
    tm1637_writeByte(ADDR_FIXED);              //
    tm1637_stop();                             //
    tm1637_start();                            //
    tm1637_writeByte(number - 1 | STARTADDR);  //
    if (points) tm1637_writeByte(data | 0x80);
    else tm1637_writeByte(data);     //
    tm1637_stop();                   //
    tm1637_start();                  //
    tm1637_writeByte(Cmd_DispCtrl);  //
    tm1637_stop();                   //
}


// отображение массива данных
void tm1637_sendAll(uint8_t data[])
{
    tm1637_start();
    tm1637_writeByte(ADDR_AUTO);  //
    tm1637_stop();                //
    tm1637_start();               //
    tm1637_writeByte(STARTADDR);  //
    for (uint8_t i = 0; i < 4; i++) {
        if (points) tm1637_writeByte(data[i] | 0x80);
        else tm1637_writeByte(data[i]);  //
    }
    tm1637_stop();                   //
    tm1637_start();                  //
    tm1637_writeByte(Cmd_DispCtrl);  //
    tm1637_stop();                   //
}


// очистка экрана
void tm1637_clear(void)
{
    points = 0;
    tm1637_sendOne(1, 0x00);
    tm1637_sendOne(2, 0x00);
    tm1637_sendOne(3, 0x00);
    tm1637_sendOne(4, 0x00);
}



#endif /* __TM1637_H_ */
