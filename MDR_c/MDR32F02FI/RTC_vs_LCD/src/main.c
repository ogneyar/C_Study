

/*
 * This example shows how to implement an alarm clock
 * Every second it prints current time to LCD
 * At startup it sets an alarm after 10 seconds
 * When alarm is triggered, you need to press a button to postpone it
 */


/* Includes ------------------------------------------------------------------*/
#include <stdio.h>

#include <MLDR187_bkp.h>
#include <MLDR187_it.h>
#include <MLDR187_gpio.h>

#include <init.h>
#include <stdbool.h>

#include "MLDR187_lib.h"
#include "MLDR187.h"
#include "MLDR187_rst_clk.h"
#include "MLDR187_eeprom.h"
#include "MLDR187_uart.h"

#include "interrupt.h"
#include "lcd.h"
#include "text.h"
#include "Logo.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define	LED_PORT 		MDR_PORTD
#define	LED_PORT_CLK 	RST_CLK_PORTD
#define LED_PIN_0		PORT_Pin_0
#define LED_PIN_1		PORT_Pin_1
#define LED_PIN_2		PORT_Pin_2
#define LED_PIN_3		PORT_Pin_3

#define	KEY_PORT 		MDR_PORTA
#define	KEY_PORT_CLK 	RST_CLK_PORTA
#define KEY_PIN_0		PORT_Pin_0
#define KEY_PIN_1		PORT_Pin_1
#define KEY_PIN_2		PORT_Pin_2
#define KEY_PIN_3		PORT_Pin_3

#define UART_MODULE     MDR_UART1
#define UART_PORT       MDR_PORTB
#define UART_PORT_CLK   RST_CLK_PORTB
#define UART_TX_PIN     PORT_Pin_0
#define UART_RX_PIN     PORT_Pin_1

#define ALARM_POST_MINUTES 0
#define ALARM_POST_SECONDS 10

#define LSE_FREQUENCY 32768


/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
PORT_InitTypeDef        PORT_InitStructure;
UART_InitTypeDef        UART_InitStructure;
RST_CLK_CpuSpeed_InitTypeDef    RST_CLK_CpuSpeed_InitStructure;

uint32_t DelayCnt = 0;
uint8_t TickTock = 0;

char Logotip1[]	= 	{
             0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0x0F,  //левая сторона логотипа
			 0x07,0xC7,0xE7,0xE7,0xE7,0xE7,0xE7,0xE7,
			 0x67,0x27,0x27,0x67,0xE7,0xE7,0xE7,0xE7,
			 0x67,0x27,0x27,0x67,0xE7,0xE7,0xC7,0x07,
			 0x0F,0xFF,0xFF,0xFF,0x00,0x00,0x00,0xF8,
			 0x80,0xC0,0x60,0xC0,0x80,0xF8,0x00,0xF8,
			 0x80,0xC0,0x60,0xC0,0x80,0xf8,0x00,0xF8,

			 0x60,0x60,0x60,0x60,0x7F,0x7F,0x7F,0x00,
			 0x00,0x3F,0x1F,0x8F,0xC7,0xE3,0xF1,0xF8,
			 0xFC,0xFE,0xF8,0x00,0x01,0x0F,0x83,0xE0,
			 0xF0,0xFC,0xFC,0xC0,0x01,0x0F,0x7F,0x00,
			 0x00,0x7F,0x7F,0x7F,0x60,0x60,0x60,0x60,
			 0x01,0x00,0xE0,0xE0,0xE1,0xC0,0x80,0x80,
			 0xC1,0xE0,0xE0,0xE0,0x01,0x00,0xE0,0xE0,

			 0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x00,
			 0x00,0x66,0x67,0x67,0x67,0x67,0x67,0x67,
			 0x67,0x67,0x67,0x66,0x66,0x67,0x67,0x67,
			 0x67,0x67,0x67,0x67,0x66,0x66,0x66,0x60,
			 0x00,0x66,0x66,0x66,0x66,0x66,0x66,0x66,
			 0x00,0x00,0x7F,0x7F,0x00,0x01,0x07,0x07,
			 0x01,0x00,0x7F,0x7F,0x00,0x00,0x7F,0x7F,

			 0x00,0x00,0x00,0x00,0xFE,0xFE,0xFE,0xF0,
			 0xE0,0xE2,0xE6,0xE6,0xE6,0xE6,0xE6,0xE6,
			 0xE6,0xE6,0xE6,0xE6,0xE6,0xE6,0xE6,0xE6,
			 0xE6,0xE6,0xE6,0xE6,0xE6,0xE6,0xE2,0xE0,
			 0xF0,0xFE,0xFE,0xFE,0x00,0x00,0x00,0x00,
			 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

			 0x80,0xC0,0x60,0xC0,0x80,0xF8,0x00,0x00, //правая сторона логотипа
			 0x00,0xF8,0x30,0x60,0x30,0xF8,0x00,0xF8,
			 0x00,0xF8,0x00,0x00,0x00,0x00,0xE0,0xB8,
			 0x88,0xB8,0xE0,0x00,0xF8,0x30,0x60,0xC0,
			 0xF8,0x00,0xF8,0x08,0x98,0xF0,0x00,0xF8,
			 0x48,0xC8,0xB0,0x00,0x00,0x00,0xF8,0x48,
			 0xC8,0xB0,0x00,0xF8,0x80,0x00,0x00,0xF8,

			 0x01,0x00,0x80,0xC0,0xE1,0xE0,0x00,0x01,
			 0x00,0x81,0xC0,0xE0,0x60,0x61,0xE0,0xE1,
			 0x00,0x01,0x01,0x01,0xC1,0xE0,0xE1,0xC0,
			 0x00,0x00,0x01,0x00,0xE1,0xE0,0x00,0x00,
			 0x01,0x00,0xE1,0xE1,0x01,0x00,0x00,0x01,
			 0x80,0xC0,0xE1,0xE0,0xE1,0xE0,0x01,0x00,
			 0x00,0xE1,0xE0,0x60,0x61,0xE1,0xC1,0x81,

			 0x0E,0x07,0x03,0x01,0x7F,0x7F,0x00,0x00,
			 0x7F,0x7F,0x01,0x00,0x00,0x00,0x7F,0x7F,
			 0x00,0x00,0x78,0x7F,0x07,0x31,0x31,0x3F,
			 0x7F,0x78,0x00,0x00,0x7F,0x7F,0x00,0x06,
			 0x06,0x06,0x7F,0x7F,0x00,0x00,0x60,0x6F,
			 0x6F,0x63,0x60,0x60,0x7F,0x7F,0x60,0x00,
			 0x00,0x7F,0x7F,0x00,0x0C,0x0E,0x07,0x03,

			 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

struct {
    MDR_GPIO_TypeDef* port; 
    uint16_t pin; 
    bool state;
}

LED_PINS[] = {
    {MDR_PORTD, pin0, false},
    {MDR_PORTD, pin1, false}
};

RTC_DateTime INIT_DATE = {
    .year = 21, .month = 3, .day = 22,
    .hours = 14, .minutes = 13, .seconds = 0
};


bool alarm_triggered = false;


/* Private function prototypes -----------------------------------------------*/
void BACKUP_IRQHandler(void);
void SysTick_Handler(void);

void LCD_printNum(uint8_t x, uint8_t y, uint32_t number);

void Led_Init(void);
void Btn_Init(void);
void Itr_Init(void);

void setAlarm(uint32_t minutes_after, uint32_t seconds_after);
void toggle_led(int i);
void set_led(int i, BitStatus state);
void init_leds();



/* Main Function */
int main()
{    
    // from RTC
    init_clock();
    init_uart();
    // init_leds();
    init_bkp(&INIT_DATE);

    setAlarm(ALARM_POST_MINUTES, ALARM_POST_SECONDS);


    // from LCD
    Itr_Init();
    Led_Init();
    Btn_Init();

    LCD_INIT();
    LCD_CLS();
    LCD_CurrentMethod = MET_AND;
    LCD_PUT_PICT(8, 0, 112, 32, Logotip);
    LCD_PUTS(54, 28, "K1986BK025");

    printf("K1986BK025 Init end!\n");


    for (;;);
}



void BACKUP_IRQHandler(void)
{
    uint32_t bkp_flags = BKP_GetFlags();

    if (bkp_flags & bkpAlarmAFlag) {
        // Alarm triggered

        // Disable alarm. It will be set again when button is pressed
        BKP_SetItNewState(bkpAlarmAIE, DISABLE);
        BKP_SetAlarmNewState(bkpAlarmAEN, DISABLE);

        printf(" !!! WAKE UP !!! \n");
        
        LCD_PUTS(20, 40, "!!! WAKE UP !!!");

        alarm_triggered = true;
        set_led(0, RESET);
        set_led(1, SET);
    }

    if ((MDR_BKP->TAFCR & BKP_RTC_TAFCR_TSIE) && ((BKP_GetTamperEventState() & bkpEventWakeup1) != 0)) {
        // Button pressed

        printf("Button pressed\n");

        LCD_PUTS(20, 40, "               ");

        // Disable button until new tick
        BKP_SetItNewState(bkpTSIE, DISABLE);

        if (alarm_triggered) {
            // Exit "alarm triggered" state (led now blinks green again)
            alarm_triggered = false;
            set_led(0, SET);
            set_led(1, RESET);

            // Set new alarm
            setAlarm(ALARM_POST_MINUTES, ALARM_POST_SECONDS);
        }
    }

    if (bkp_flags & bkpSecCntEnFlag) {
        // Every second tick

        // Reset flags and enable button event
        BKP_SetItNewState(bkpSecondIE | bkpTSIE, ENABLE);

        // Print current time
        RTC_DateTime dateTime;
        BKP_GetDateTimeBCD(&dateTime);
        BKP_ConvertToBINFormat(&dateTime);

        printf("%.2u:%.2u:%.2u %.2u.%.2u.20%.2u",
               dateTime.hours, dateTime.minutes, dateTime.seconds,
               dateTime.day, dateTime.month, dateTime.year
        );

        LCD_PUTS(40, 50, "          ");
        LCD_printNum(40, 50, dateTime.hours);
        LCD_PUTS(52, 50, ":");
        LCD_printNum(58, 50, dateTime.minutes);
        LCD_PUTS(70, 50, ":");
        LCD_printNum(76, 50, dateTime.seconds);

        // Blink led and print alarm message
        if (alarm_triggered) {
            printf(" ... please wake up and press the button ...");
            toggle_led(1);
        } else {
            toggle_led(0);
        }

        printf("\n");

    }
}


/* --- */
void SysTick_Handler(void)
{
    // Set up time for next interrupt 
    MDR_CLIC_MTIMECMP_Reg = MDR_CLIC_MTIME_Reg + 32768;
    
    if (TickTock)
    {
        TickTock = 0;
    }
    else
    {
        TickTock = 1;
    }

}


void LCD_printNum(uint8_t x, uint8_t y, uint32_t number)
{
    if (number <= 9) { 
        LCD_PUTC(x, y, (uint8_t)(number + 0x30));
    }else if (number > 9 && number < 100) {
        LCD_PUTC(x, y, (uint8_t)(number/10 + 0x30));
        LCD_PUTC(x + 6, y, (uint8_t)(number - ((number/10) * 10) + 0x30));
    }else if (number > 99 && number < 1000) {
        uint8_t temp = number / 100;
        LCD_PUTC(x, y, (uint8_t)(temp + 0x30));
        uint8_t temp2 = ( number - temp*100 )  / 10;
        LCD_PUTC(x + 6, y, (uint8_t)(temp2 + 0x30));
        uint8_t temp3 = number - temp*100 - temp2*10;
        LCD_PUTC(x + 12, y, (uint8_t)(temp3 + 0x30));
    }else if (number > 999 && number < 10000) {
        uint8_t temp = number / 1000;
        LCD_PUTC(x, y, (uint8_t)(temp + 0x30));
        uint8_t temp2 = ( number - temp*1000 )  / 100;
        LCD_PUTC(x + 6, y, (uint8_t)(temp2 + 0x30));
        uint8_t temp3 = (number - temp*1000 - temp2*100) / 10;
        LCD_PUTC(x + 12, y, (uint8_t)(temp3 + 0x30));
        uint8_t temp4 = number - temp*1000 - temp2*100 - temp3*10;
        LCD_PUTC(x + 18, y, (uint8_t)(temp4 + 0x30));
    }
}



/* --- */
void Led_Init(void)
{
    PORT_InitStructure.PORT_OE    = PORT_OE_OUT;
    PORT_InitStructure.PORT_FUNC  = PORT_FUNC_PORT;
    PORT_InitStructure.PORT_MODE  = PORT_MODE_DIGITAL;
    PORT_InitStructure.PORT_SPEED = PORT_SPEED_SLOW_4mA;
    PORT_InitStructure.PORT_PULL_DOWN   = PORT_PULL_DOWN_OFF;
    PORT_Init(LED_PORT, (LED_PIN_0 | LED_PIN_1 | LED_PIN_2 | LED_PIN_3), &PORT_InitStructure);
    PORT_SetReset(LED_PORT, LED_PIN_0, SET);
    PORT_SetReset(LED_PORT, LED_PIN_1, SET);
    PORT_SetReset(LED_PORT, LED_PIN_2, SET);
    PORT_SetReset(LED_PORT, LED_PIN_3, SET);
}

/* --- */
void Btn_Init(void)
{
    PORT_InitStructure.PORT_OE    = PORT_OE_IN;
    PORT_InitStructure.PORT_FUNC  = PORT_FUNC_PORT;
    PORT_InitStructure.PORT_MODE  = PORT_MODE_DIGITAL;
    PORT_InitStructure.PORT_SPEED = PORT_SPEED_SLOW_4mA;
    PORT_InitStructure.PORT_PULL_DOWN   = PORT_PULL_DOWN_OFF;
    PORT_Init(KEY_PORT, (KEY_PIN_0 | KEY_PIN_1 | KEY_PIN_2 | KEY_PIN_3), &PORT_InitStructure);
}

/* --- */
void Itr_Init(void)
{
    /* Disable SysTick interrupt */
    disable_SysTick_Handler();
    /* Safely set up SysTick cmp value */
    MDR_CLIC_MSIP_Reg = 0;
    MDR_CLIC_MTIMECMP_Reg = MDR_CLIC_MTIMECMP_Reg + 32768;
    /* Enable SysTick interrupt */
    enable_SysTick_Handler();
    RST_CLK_EnablePeripheralClock(RST_CLK_BKP, RST_CLK_Div1);
    MDR_BKP->WPR = 0x8555AAA1;
    MDR_BKP->CLK |= 0x00000003;
}




void setAlarm(uint32_t minutes_after, uint32_t seconds_after)
{
    RTC_DateTime dateTime;
    BKP_GetDateTimeBCD(&dateTime);
    BKP_ConvertToBINFormat(&dateTime);

    dateTime.seconds = dateTime.seconds + seconds_after;
    dateTime.minutes += dateTime.seconds / 60;
    dateTime.seconds %= 60;
    dateTime.minutes = dateTime.minutes + minutes_after;
    dateTime.hours += dateTime.minutes / 60;
    dateTime.minutes %= 60;

    BKP_AlarmInitTypeDef bkpAlarm = {
            .enHours = ENABLE,
            .enWeekDay = DISABLE,
            .enDateEn = DISABLE,
            .enMinutes = ENABLE,
            .enSeconds = ENABLE
    };

    BKP_ConvertToBCDFormat(&dateTime);
    bkpAlarm.minutes = dateTime.minutes;
    bkpAlarm.seconds = dateTime.seconds;
    bkpAlarm.hours = dateTime.hours;
    bkpAlarm.day = dateTime.day;

    BKP_SetAlarm(bkpAlarmAEN, &bkpAlarm);
    BKP_SetAlarmNewState(bkpAlarmAEN, ENABLE);

    // Print alarm time
    BKP_ConvertToBINFormat(&dateTime);
    printf("Alarm set to --> %.2u:%.2u:%.2u\n",
           dateTime.hours, dateTime.minutes, dateTime.seconds
    );
}


void toggle_led(int i) {
    LED_PINS[i].state = !LED_PINS[i].state;
    PORT_SetReset(LED_PINS[i].port, LED_PINS[i].pin, !LED_PINS[i].state);
}


void set_led(int i, BitStatus state) {
    LED_PINS[i].state = state;
    PORT_SetReset(LED_PINS[i].port, LED_PINS[i].pin, !state);
}


void init_leds() {
    for (int i = 0; i < (int)(sizeof(LED_PINS) / sizeof(LED_PINS[0])); i++) {
        PORT_InitTypeDef portInit = {
                .PORT_MODE = PORT_MODE_DIGITAL,
                .PORT_OE = PORT_OE_OUT,
                .PORT_FUNC = PORT_FUNC_PORT,
                .PORT_SPEED = PORT_SPEED_SLOW_4mA,
                .PORT_PULL_DOWN = PORT_PULL_DOWN_OFF
        };

        PORT_Init(LED_PINS[i].port, LED_PINS[i].pin, &portInit);
        PORT_SetReset(LED_PINS[i].port, LED_PINS[i].pin, !LED_PINS[i].state);
    }
}


void init_bkp(RTC_DateTime * dateTime) {
    BKP_InitTypeDef bkpInit;
    BKP_StructInitDefault(&bkpInit);

    // Enable peripheral clock to BKP
    RST_CLK_EnablePeripheralClock(RST_CLK_BKP, RST_CLK_Div1);
    BKP_FreqGenCmd(bkpLse, ENABLE, ENABLE);
    bkpInit.RTCsrc = bkpRtc_LSE;

    // As BKP is not reset on main core reset, we need to clear and disable all
    // BKP related interrupts that occurred before to avoid undefined behaviour
    BKP_SetItNewState(bkpAlarmBIE | bkpAlarmAIE | bkpWutfIE | bkpSecondIE | bkpOwerflowIE | bkpTSIE | bkpTAMPIE, DISABLE);
    PLIC_SetMinPermPriority(0);
    PLIC_SetPriority(BACKUP_IRQn, 1);
    PLIC_DisableIRQ(BACKUP_IRQn);
    PLIC_ReleaseIRQ(BACKUP_IRQn);
    enable_irq_extm();

    uint32_t prescaler = LSE_FREQUENCY;

    // RTC time can only be changed when RTC is disabled
    bkpInit.RTCenable = DISABLE;
    BKP_Init(&bkpInit);

// установка времени компиляции
// ----------------------------
    // Set date and time
    // BKP_ConvertToBCDFormat(dateTime);
    // BKP_SetDateTimeBCD(dateTime, SET, SET);
// ----------------------------

    // In this example we use WAKEUP1 as a button.
    // Here we initialize it
    BKP_TampEventInitTypeDef tampInit;
    tampInit.enable = ENABLE;
    tampInit.interrupt = ENABLE;
    tampInit.wakeup1 = BKP_FALLING_LOW;
    BKP_InitExternalEvent(&tampInit);

    // Run the clock
    bkpInit.RTCenable = ENABLE;
    bkpInit.RTCprediv = prescaler;
    BKP_Init(&bkpInit);

    // Enable interrupts
    BKP_SetItNewState(bkpSecondIE | bkpAlarmAIE | bkpTSIE, ENABLE);
    PLIC_EnableIRQ(BACKUP_IRQn);
}
