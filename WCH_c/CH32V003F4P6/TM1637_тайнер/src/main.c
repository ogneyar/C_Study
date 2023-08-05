
#include "debug.h"
#include "tm1637.h"
#include <ch32v00x.h>

// #define __DEBUG_TAINER_ 

typedef uint8_t bool;
typedef uint8_t boolean;
#define true 1
#define false 0


bool update = false;        // флаг для обновления
bool tainerStop = true;     // флаг для остановки тайнера
bool beepFlag = false;      // флаг для включения сигнала
bool beepPeriodFlag = false;      // флаг для периодического прерывания сигнала
bool setTimeFlag = false;   // флаг установки тайнера
bool one_second = false;   // в режиме настроек тиканье тайнера

uint8_t segment1 = 0; // сегмент десятков часов
uint8_t segment2 = 0; // сегмент часов
uint8_t segment3 = 0; // сегмент десятков минут
uint8_t segment4 = 0; // сегмент минут
uint8_t segment5 = 0; // сегмент десятков секунд
uint8_t segment6 = 0; // сегмент секунд

uint8_t points = 0;

bool flagButton1 = false;
bool flagButton2 = false;
bool flagButton3 = false;

uint32_t buttonHourPressValue = 0; // продолжительность нажатия кнопки часов
uint32_t buttonMinPressValue = 0; // продолжительность нажатия кнопки минут

uint32_t maxValuePressButton = 0x00008000;

#define button1Pin GPIO_Pin_4 // PC4 - кнопка настроек
#define button2Pin GPIO_Pin_6 // PC6 - кнопка часов
#define button3Pin GPIO_Pin_7 // PC7 - кнопка минут


// список функций
void EXTI7_0_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void EXTI4_Init(void);
void TIM1_PWMOut_Init(u16 arr, u16 psc, u16 ccp);
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

    EXTI4_Init(); // прерывание по нажатию на PC4 (кнопка настроек)
    // TIM1_PWMOut_Init( 10, 800-1, 5 ); // ШИМ на биппер (TIM_CH2(PA1))
    TIM2_Init(1000); // прерывание таймера раз в секунду

    tm1637_init(GPIOC, GPIO_Pin_2, GPIO_Pin_1);  // по умолчанию CLK = GPIO_Pin_2, а DIO = GPIO_Pin_1
    tm1637_clear();
    tm1637_brightness(1);  // яркость, 0 - 7 (минимум - максимум)
    tm1637_point(points);
    tm1637_display(8, 8, 8, 8);

    buttonInit();

    while(1)
    {     
        showTime();      

        setTime();  
    }
}


// This function handles EXTI Handler.
void EXTI7_0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line4)!=RESET)
	{
#ifdef __DEBUG_TAINER_
		printf("Run at EXTI\r\n");
#endif
        if (beepFlag)
        {
            beepFlag = false;
            tainerStop = true;
            TIM_Cmd( TIM1, DISABLE );
            RCC_APB2PeriphClockCmd( RCC_APB2Periph_TIM1, DISABLE );
        }
		EXTI_ClearITPendingBit(EXTI_Line4);     /* Clear Flag */
	}
}


// Initializes EXTI collection.
void EXTI4_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    EXTI_InitTypeDef EXTI_InitStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* GPIOC ----> EXTI_Line4 */
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource4);
    EXTI_InitStructure.EXTI_Line = EXTI_Line4;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI7_0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


/*********************************************************************
 * @fn      TIM1_OutCompare_Init
 *
 * @brief   Initializes TIM1 output compare.
 *
 * @param   arr - the period value.
 *          psc - the prescaler value.
 *          ccp - the pulse value.
 *
 * @return  none
 */
void TIM1_PWMOut_Init(u16 arr, u16 psc, u16 ccp)
{
    GPIO_InitTypeDef GPIO_InitStructure={0};
    TIM_OCInitTypeDef TIM_OCInitStructure={0};
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure={0};

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_TIM1, ENABLE );

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( GPIOA, &GPIO_InitStructure );

    TIM_TimeBaseInitStructure.TIM_Period = arr;
    TIM_TimeBaseInitStructure.TIM_Prescaler = psc;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit( TIM1, &TIM_TimeBaseInitStructure );

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;

    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = ccp;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC2Init( TIM1, &TIM_OCInitStructure );

    TIM_CtrlPWMOutputs(TIM1, ENABLE );
    TIM_OC2PreloadConfig( TIM1, TIM_OCPreload_Disable );
    TIM_ARRPreloadConfig( TIM1, ENABLE );
    // TIM_Cmd( TIM1, ENABLE );
}


//
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_CC4) != RESET)
    {        
#ifdef __DEBUG_TAINER_
        printf("Run at TIM2\r\n");
#endif
        if ( ! tainerStop ) update = true;
        if (setTimeFlag) one_second = true; // когда в режиме настроек 
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

    // if (tainerStop)
    // {
    //     // sleep
    //     if (points)
    //     {            
    //         Delay_Ms(300);
    //         tm1637_point(points = !points);
    //         tm1637_display(0, 0, 0, 0);
    //     }
    // }
}

//
void beep(void)
{
    beepFlag = true;
    beepPeriodFlag = true;
    
    TIM1_PWMOut_Init( 10, 800-1, 5 ); // ШИМ на биппер (TIM_CH2(PA1))

    while(beepFlag)
    {
        if (beepPeriodFlag) {
            RCC_APB2PeriphClockCmd( RCC_APB2Periph_TIM1, ENABLE );
            TIM_Cmd( TIM1, ENABLE );
        }
        else
        {
            RCC_APB2PeriphClockCmd( RCC_APB2Periph_TIM1, DISABLE );
            TIM_Cmd( TIM1, DISABLE );
        }
        beepPeriodFlag = ! beepPeriodFlag;

        blinkPoints();
#ifdef __DEBUG_TAINER_
        printf("beep\r\n");
#endif
    }
    
}

//
void blinkPoints(void)
{
    points = true;
    for (uint8_t i = 0; i < 10; i++)
    {
        tm1637_point(points);
        if ( ! points) tm1637_display(0, 0, 0, 0);
        else tm1637_clearWithOutPoints();        
        points = ! points;
        Delay_Ms(100);
    }
}


// -------------------------
// установка времени тайнера
bool setTime(void)
{  
    boolean button1 = !digitalRead(button1Pin);
    if (button1 == true && flagButton1 == false)
    {
        flagButton1 = true;
#ifdef __DEBUG_TAINER_
        printf("Button1 pressed\r\n");
#endif
        Delay_Ms(10);

        /*
            кнопка настроек нажата
            настройка тайнера
        */

        setTimeFlag = ! setTimeFlag;

        if (setTimeFlag)
        {
            /*
                при первом нажатии
                включаем точки
            */        

            tainerStop = true;
            points = true;
            tm1637_point(points);
            tm1637_display(segment1, segment2, segment3, segment4);
        }

        if ( ! setTimeFlag )
        {
            /*
                при повторном нажатии
                записываем время
            */        

            if ( ! segment1 && ! segment2 && ! segment3 && ! segment4)
            {
                segment5 = 0;
                segment6 = 0;
            }

            if (segment1 || segment2 || segment3 || segment4 || segment5 || segment6)
                tainerStop = false;

            for(u8 i = 0; i < 2; i++)
            {
                tm1637_clear();
                Delay_Ms(250);
                tm1637_point(points = 0);
                tm1637_display(segment1, segment2, segment3, segment4);
                Delay_Ms(250);
            }
        }
        
    }

    if (button1 == false && flagButton1 == true)
    {
        flagButton1 = false;
#ifdef __DEBUG_TAINER_
        printf("Button1 released\r\n");
#endif
        Delay_Ms(10);

        /*
            кнопка настроек отпущена
            ничего не делаем
        */

    }

    boolean button2 = !digitalRead(button2Pin);
    if (button2 == true && flagButton2 == false) {
        flagButton2 = true;
#ifdef __DEBUG_TAINER_
        printf("Button2 pressed\r\n");
#endif
        Delay_Ms(10);
        
        /*
            кнопка часов нажата
            ничего не делаем
        */

    }
    
    if (button2 == true && flagButton2 == true)
    {
#ifdef __DEBUG_TAINER_
        printf("Button2 pressed long\r\n");
#endif
        // Delay_Ms(10);
        
        /*
            кнопка часов удерживается
            меняем часы
        */

        if (setTimeFlag) // если нажата кнопка настроек
        {            
            if (buttonHourPressValue > maxValuePressButton)
            {
                if (segment2 < 2) segment2++;
                else segment2 = 0;
                tm1637_point(points = 1);
                tm1637_display(segment1, segment2, segment3, segment4);
                // buttonHourPressValue = 0;
                Delay_Ms(100);
            }
            else
            {
                buttonHourPressValue++;
            }
        }

    }

    if (button2 == false && flagButton2 == true)
    {
        flagButton2 = false;
#ifdef __DEBUG_TAINER_
        printf("Button2 released\r\n");
#endif
        Delay_Ms(10);
        
        /*
            кнопка часов отпущена
        */
       
        if (setTimeFlag) // если нажата кнопка настроек
        {
            if (buttonHourPressValue <= maxValuePressButton)
            {
                if (segment2 < 2) segment2++;
                else segment2 = 0;
                tm1637_point(points = 1);
                tm1637_display(segment1, segment2, segment3, segment4);
            }
        }    

        buttonHourPressValue = 0;    

    }
    
    boolean button3 = !digitalRead(button3Pin);
    if (button3 == true && flagButton3 == false)
    {
        flagButton3 = true;
#ifdef __DEBUG_TAINER_
        printf("Button3 pressed\r\n");
#endif
        Delay_Ms(10);
        
        /*
            кнопка минут нажата
            ничего не делаем
        */

    }
    
    if (button3 == true && flagButton3 == true)
    {
#ifdef __DEBUG_TAINER_
        printf("Button3 pressed long\r\n");
#endif
        // Delay_Ms(10);
        
        /*
            кнопка минут удерживается
            меняем минуты
        */

        if (setTimeFlag) // если нажата кнопка настроек
        {            
            if (buttonMinPressValue > maxValuePressButton)
            {
                if (segment4 < 9) segment4++;
                else 
                {
                    segment4 = 0;
                    if (segment3 < 5) segment3++;
                    else segment3 = 0;
                }
                tm1637_point(points = 1);
                tm1637_display(segment1, segment2, segment3, segment4);
                // buttonMinPressValue = 0;
                Delay_Ms(100);
            }
            else
            {
                buttonMinPressValue++;
            }
        }

    }

    if (button3 == false && flagButton3 == true)
    {
        flagButton3 = false;
#ifdef __DEBUG_TAINER_
        printf("Button3 released\r\n");
#endif
        Delay_Ms(10);

        /*
            кнопка минут отпущена
            меняем минуты
        */

        if (setTimeFlag) // если нажата кнопка настроек
        {
            if (segment4 < 9) segment4++;
            else 
            {
                segment4 = 0;
                if (segment3 < 5) segment3++;
                else segment3 = 0;
            }
            tm1637_point(points = 1);
            tm1637_display(segment1, segment2, segment3, segment4);
        }
        
        buttonMinPressValue = 0;

    }

    if (one_second)
    {
        tm1637_clear();
        Delay_Ms(100);
        tm1637_point(points = 1);
        tm1637_display(segment1, segment2, segment3, segment4);
        one_second = false;
    }


    return setTimeFlag;
}

