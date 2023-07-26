
#include "debug.h"
#include "tm1637.h"

// #define __DEBUG_TAINER_ 

typedef uint8_t bool;
typedef uint8_t boolean;
#define true 1
#define false 0


bool update = false;        // флаг для обновления
bool beepFlag = false;      // флаг для сигнала
bool setTimeFlag = false;   // флаг установки тайнера

uint8_t segment1 = 0;
uint8_t segment2 = 0;
uint8_t segment3 = 0;
uint8_t segment4 = 0;
uint8_t segment5 = 0; // сегменты секунд не отображаются
uint8_t segment6 = 5; // но считаются

uint8_t points = 0;

bool flagButton1 = false;
bool flagButton2 = false;
bool flagButton3 = false;

uint8_t h, m, s;
uint8_t hour1, hour2;
uint8_t min1, min2;
uint8_t height_segments = 57;

#define button1Pin GPIO_Pin_4 // PC4 - кнопка настроек
#define button2Pin GPIO_Pin_6 // PC6 - кнопка часов
#define button3Pin GPIO_Pin_7 // PC7 - кнопка минут


// список функций
void TIM2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void TIM2_Init(u16 period);
void buttonInit(void);
void showTime(void);
void beep(void);
void blinkPoints(void);
bool setTime(void);


// основная функция
int main(void)
{
    Delay_Init();
    USART_Printf_Init(115200);
    printf("SystemClk:%d\r\n", SystemCoreClock);
    printf("Tainer on TM1637\r\n");

    TIM2_Init(1000); // прерывание таймера раз в секунду

    tm1637_init(GPIOC, GPIO_Pin_2, GPIO_Pin_1);  // по умолчанию CLK = GPIO_Pin_2, а DIO = GPIO_Pin_1
    tm1637_clear();
    tm1637_brightness(1);  // яркость, 0 - 7 (минимум - максимум)
    tm1637_point(points);
    tm1637_display(0, 8, 0, 8);

    buttonInit();

    while(1)
    {     
        // update = ! setTime(); // если была нажата 1 кнопка, то тайнер останавливается, до следующего нажатия этой же кнопки

        showTime();        

#ifdef __DEBUG_TAINER_
        printf("point: %d\r\n", points);
#endif
    }
}

//
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_CC4) != RESET)
    {        
#ifdef __DEBUG_TAINER_
        printf("Run at TIM2\r\n");
#endif
        update = 1;
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC4);     /* Clear Flag */
    }

}

//
void TIM2_Init(u16 period)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure={0};

    RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM2, ENABLE );
   
    TIM_TimeBaseInitStructure.TIM_Period = period;
    TIM_TimeBaseInitStructure.TIM_Prescaler = (SystemCoreClock/1000)-1;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit( TIM2, &TIM_TimeBaseInitStructure);

    TIM_Cmd( TIM2, ENABLE );

    NVIC_EnableIRQ(TIM2_IRQn);
    TIM_ITConfig(TIM2, TIM_IT_CC4, ENABLE);
}

//
void buttonInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure={0};
    GPIO_InitStructure.GPIO_Pin = button1Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = button2Pin;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = button3Pin;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    digitalWrite(button1Pin, 1);
    digitalWrite(button2Pin, 1);
    digitalWrite(button3Pin, 1);
}

//
void showTime(void)
{  
    if (update)
    {    
        if (segment1 || segment2 || segment3 || segment4 || segment5 || segment6)
        {
            tm1637_point(points = !points);

            if (segment1 || segment2) tm1637_display(segment1, segment2, segment3, segment4); // часы и минуты
            else tm1637_display(segment3, segment4, segment5, segment6); // минуты и секунды

            if (segment6) segment6--;
            else
            {
                if (segment5)
                {
                    segment5--;
                    segment6 = 9;
                }
                else
                {
                    if (segment4)
                    {
                        segment4--;
                        segment5 = 5;
                        segment6 = 9;
                    }
                    else
                    {
                        if (segment3)
                        {
                            segment3--;
                            segment4 = 9;
                            segment5 = 5;
                            segment6 = 9;
                        }
                        else
                        {
                            if (segment2)
                            {
                                segment2--;
                                segment3 = 5;
                                segment4 = 9;
                                segment5 = 5;
                                segment6 = 9;
                            }
                            else
                            {
                                segment1--;
                                segment2 = 9;
                                segment3 = 5;
                                segment4 = 9;
                                segment5 = 5;
                                segment6 = 9;
                            }
                        }
                    }
                }
            }          
        }
        else {
            // tm1637_point(0);
            tm1637_display(0, 0, 0, 0);
            beep();
        }  
        
        update = false;

    }
}

//
void beep(void)
{
    beepFlag = true;

    while(beepFlag)
    {
        blinkPoints();
#ifdef __DEBUG_TAINER_
        printf("beep\r\n");
#endif
    }
    
}

//
void blinkPoints(void)
{
    for (uint8_t i = 0; i < 10; i++)
    {
        Delay_Ms(100);
        tm1637_point(points = !points);
        if ( ! points) tm1637_display(0, 0, 0, 0);
        else tm1637_clearWithOutPoints();
    }
}

//
bool setTime(void)
{  
    boolean button1 = !digitalRead(button1Pin);
    if (button1 == true && flagButton1 == false) {
        flagButton1 = true;
#ifdef __DEBUG_TAINER_
        printf("Button1 pressed\r\n");
#endif
        Delay_Ms(10);

        /*
            настройка тайнера
        */

        

        setTimeFlag = ! setTimeFlag;
        
        tm1637_clear();
        Delay_Ms(500);
        tm1637_point(points = 0);
        tm1637_display(segment1, segment2, segment3, segment4);

        if (setTimeFlag)
        {
        }
        
    }
    if (button1 == false && flagButton1 == true) {
        flagButton1 = false;
#ifdef __DEBUG_TAINER_
        printf("Button1 released\r\n");
#endif
        Delay_Ms(10);

        /*
            записываем время
        */

    }

    boolean button2 = !digitalRead(button2Pin);
    if (button2 == true && flagButton2 == false) {
        flagButton2 = true;
#ifdef __DEBUG_TAINER_
        printf("Button2 pressed\r\n");
#endif
        Delay_Ms(10);

    }
    if (button2 == false && flagButton2 == true) {
        flagButton2 = false;
#ifdef __DEBUG_TAINER_
        printf("Button2 released\r\n");
#endif
        
        /*
        меняем часы
        */
        if (flagButton1) // если нажата кнопка настроек
        {
            // if (update) printf(h);
            hour1 = (h >> 4);
            hour2 = (h & 0x0f);

            if (hour2 == 9)
            {
                hour2 = 0;
                if (hour1 == 1) hour1 = 2;
                else hour1 = 1;
            }
            else 
            {
                if (hour1 == 2 && hour2 == 3) 
                {
                hour1 = 0;
                hour2 = 0;
                }
                else hour2 += 1;
            }
            
            h = (hour1 << 4) | (hour2);

            if (segment1 != (h >> 4))
            {
                // st7735_setCursor(10, height_segments);
                // st7735_printNum(segment1, textbgcolor);
                // segment1 = (h >> 4);
                // st7735_setCursor(10, height_segments);
                // st7735_printNum(segment1);
            }
            
            if (segment2 != (h & 0x0F))
            {
                // st7735_setCursor(45, height_segments);
                // st7735_printNum(segment2, textbgcolor);
                // segment2 = (h & 0x0F);
                // st7735_setCursor(45, height_segments);
                // st7735_printNum(segment2);
            }

        }

        Delay_Ms(10);
    }
    
    boolean button3 = !digitalRead(button3Pin);
    if (button3 == true && flagButton3 == false) {
        flagButton3 = true;
#ifdef __DEBUG_TAINER_
        printf("Button3 pressed\r\n");
#endif
        Delay_Ms(10);
    }
    if (button3 == false && flagButton3 == true) {
        flagButton3 = false;
#ifdef __DEBUG_TAINER_
        printf("Button3 released\r\n");
#endif

        /*
        меняем минуты
        */
        if (flagButton1) // если нажата кнопка настроек
        {
            // if (update) Serial.println(m);
            min1 = (m >> 4);
            min2 = (m & 0x0f);
            
            if (min2 == 9)
            {
                min2 = 0;
                if (min1 == 5) min1 = 0;
                else min1 += 1;
            }
            else 
            {
                min2 += 1;
            }
            
            m = (min1 << 4) | (min2);
            
            if (segment3 != (m >> 4))
            {
                // st7735_setCursor(95, height_segments);
                // st7735_printNum(segment3, textbgcolor);
                // segment3 = (m >> 4);
                // st7735_setCursor(95, height_segments);
                // st7735_printNum(segment3);
            }
            
            if (segment4 != (m & 0x0F))
            {
                // st7735_setCursor(130, height_segments);
                // st7735_printNum(segment4, textbgcolor);
                // segment4 = (m & 0x0F);
                // st7735_setCursor(130, height_segments);
                // st7735_printNum(segment4);
            }
        }

        Delay_Ms(10);
    }

    // return flagButton1;
    return setTimeFlag;
}

