
#ifndef __RTC_H_
#define __RTC_H_

#include "stdio.h"
#include <stdint.h>
#include "defines.h"
#include "rcc.h"
#include "bkp.h"
#include "irqn.h"

#define RTC_IRQn 19

#define RTC_LSI 1
// #define RTC_LSE 1

typedef struct
{
    vu8 hour;
    vu8 min;
    vu8 sec;

    vu16 w_year;
    vu8  w_month;
    vu8  w_date;
    vu8  week;
} _calendar_obj;

_calendar_obj calendar;

u8 const table_week[12] = {0, 3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5};
const u8 mon_table[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};


// Список функций
u8 RTC_Init(void);
void RTC_WaitForLastTask(void);
void RTC_EnterConfigMode(void);
void RTC_ExitConfigMode(void);
u8 Is_Leap_Year(u16 year);
u8 RTC_Set(u16 syear, u8 smon, u8 sday, u8 hour, u8 min, u8 sec);
u8 RTC_Alarm_Set(u16 syear, u8 smon, u8 sday, u8 hour, u8 min, u8 sec);
u8 RTC_Get(void);
u8 RTC_Get_Week(u16 year, u8 month, u8 day);
uint32_t RTC_GetCounter(void);


// Инициализация RTC
u8 RTC_Init(void)
{
    u8 temp = 0;

	// RCC APB1 Periph Clock enable
    RCC->APB1PCENR |= RCC_APB1PCENR_PWREN;
    RCC->APB1PCENR |= RCC_APB1PCENR_BKPEN;

    // PWR Backup Access enable
	PWR->CTLR |= (1 << 8);

#ifndef RTC_LSE // если используется внутренний часовой кварц (LSI)

    // RCC LSI enable
	RCC->RSTSCKR |= (1 << 0);
    while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET && temp < 250)
    {
        temp++;
        Delay_Ms(20);
    }
    if(temp >= 250)
        return 1;
	// RTC Source LSI
	RCC->BDCTLR |= RCC_RTCCLKSource_LSI;

#else // если используется внешний часовой кварц (LSE)

    RCC_LSEConfig(RCC_LSE_ON);
    while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET && temp < 250)
    {
        temp++;
        Delay_Ms(20);
    }
    if(temp >= 250)
        return 1;
	// RTC Source LSE
	RCC->BDCTLR |= RCC_RTCCLKSource_LSE;

#endif

    // RTC CLK enable
	RCC->BDCTLR |= (1 << 15);
    RTC_WaitForLastTask();	
    // RTC Wait For Synchro
    RTC->CTLRL &= (uint16_t)~RTC_FLAG_RSF;
    while((RTC->CTLRL & RTC_FLAG_RSF) == (uint16_t)RESET) ;

    // RTC Interrupt Alarm enable
	RTC->CTLRH |= RTC_IT_ALR;
    // RTC Interrupt Second enable
	RTC->CTLRH |= RTC_IT_SEC;
    RTC_WaitForLastTask();

    uint16_t bkp_data = 0XA1AA;

    // когда надо установить время
    // ---------------------------
    if (BKP->DATAR1 != bkp_data)
    {
        RTC_EnterConfigMode();
        // RTC Set Prescaler = 32767
		RTC->PSCRH = (32767 & PRLH_MSB_MASK) >> 16;
		RTC->PSCRL = (32767 & RTC_LSB_MASK);
        RTC_WaitForLastTask();
        RTC_Set(2023, 7, 21, 13, 47, 0); // Setup Time
        RTC_ExitConfigMode();
        
        BKP_Write(BKP_DR1, bkp_data);
    }
    // ---------------------------

    uint8_t NVIC_IRQChannelSubPriority = 0;
    NVIC->IPRIOR[(uint32_t)(RTC_IRQn)] = (NVIC_IRQChannelSubPriority << 4);
    NVIC->IENR[((uint32_t)(RTC_IRQn) >> 5)] = (1 << ((uint32_t)(RTC_IRQn) & 0x1F));

    RTC_Get();

    return 0;
}


// ожидание выполнения последней задачи
void RTC_WaitForLastTask(void)
{
    while((RTC->CTLRL & RTC_FLAG_RTOFF) == (uint16_t)RESET) ;
}


//
void RTC_EnterConfigMode(void)
{
    RTC->CTLRL |= RTC_CTLRL_CNF;
}


//
void RTC_ExitConfigMode(void)
{
    RTC->CTLRL &= (uint16_t) ~((uint16_t)RTC_CTLRL_CNF);
}

//
u8 Is_Leap_Year(u16 year)
{
    if(year % 4 == 0)
    {
        if(year % 100 == 0)
        {
            if(year % 400 == 0)
                return 1;
            else
                return 0;
        }
        else
            return 1;
    }
    else
        return 0;
}


//
u8 RTC_Set(u16 syear, u8 smon, u8 sday, u8 hour, u8 min, u8 sec)
{
    u16 t;
    u32 seccount = 0;
    if(syear < 1970 || syear > 2099)
        return 1;
    for(t = 1970; t < syear; t++)
    {
        if(Is_Leap_Year(t))
            seccount += 31622400;
        else
            seccount += 31536000;
    }
    smon -= 1;
    for(t = 0; t < smon; t++)
    {
        seccount += (u32)mon_table[t] * 86400;
        if(Is_Leap_Year(syear) && t == 1)
            seccount += 86400;
    }
    seccount += (u32)(sday - 1) * 86400;
    seccount += (u32)hour * 3600;
    seccount += (u32)min * 60;
    seccount += sec;

	// RCC APB1 Periph Clock enable
    RCC->APB1PCENR |= RCC_APB1PCENR_PWREN;
    RCC->APB1PCENR |= RCC_APB1PCENR_BKPEN;
    // PWR Backup Access enable
	PWR->CTLR |= (1 << 8);

    // RTC Set Counter = seccount    
    RTC_EnterConfigMode();
    RTC->CNTH = seccount >> 16;
    RTC->CNTL = (seccount & RTC_LSB_MASK);
    RTC_ExitConfigMode();

    RTC_WaitForLastTask();
    return 0;
}

//
u8 RTC_Alarm_Set(u16 syear, u8 smon, u8 sday, u8 hour, u8 min, u8 sec)
{
    u16 t;
    u32 seccount = 0;
    if(syear < 1970 || syear > 2099)
        return 1;
    for(t = 1970; t < syear; t++)
    {
        if(Is_Leap_Year(t))
            seccount += 31622400;
        else
            seccount += 31536000;
    }
    smon -= 1;
    for(t = 0; t < smon; t++)
    {
        seccount += (u32)mon_table[t] * 86400;
        if(Is_Leap_Year(syear) && t == 1)
            seccount += 86400;
    }
    seccount += (u32)(sday - 1) * 86400;
    seccount += (u32)hour * 3600;
    seccount += (u32)min * 60;
    seccount += sec;

	// RCC APB1 Periph Clock enable
    RCC->APB1PCENR |= RCC_APB1PCENR_PWREN;
    RCC->APB1PCENR |= RCC_APB1PCENR_BKPEN;
    // PWR Backup Access enable
	PWR->CTLR |= (1 << 8);

    // RTC Set Alarm = seccount
    RTC_EnterConfigMode();
    RTC->ALRMH = seccount >> 16;
    RTC->ALRML = (seccount & RTC_LSB_MASK);
    RTC_ExitConfigMode();

    RTC_WaitForLastTask();

    return 0;
}

//
u8 RTC_Get(void)
{
    static u16 daycnt = 0;
    u32        timecount = 0;
    u32        temp = 0;
    u16        temp1 = 0;
    timecount = RTC_GetCounter();
    temp = timecount / 86400;
    if(daycnt != temp)
    {
        daycnt = temp;
        temp1 = 1970;
        while(temp >= 365)
        {
            if(Is_Leap_Year(temp1))
            {
                if(temp >= 366)
                    temp -= 366;
                else
                {
                    temp1++;
                    break;
                }
            }
            else
                temp -= 365;
            temp1++;
        }
        calendar.w_year = temp1;
        temp1 = 0;
        while(temp >= 28)
        {
            if(Is_Leap_Year(calendar.w_year) && temp1 == 1)
            {
                if(temp >= 29)
                    temp -= 29;
                else
                    break;
            }
            else
            {
                if(temp >= mon_table[temp1])
                    temp -= mon_table[temp1];
                else
                    break;
            }
            temp1++;
        }
        calendar.w_month = temp1 + 1;
        calendar.w_date = temp + 1;
    }
    temp = timecount % 86400;
    calendar.hour = temp / 3600;
    calendar.min = (temp % 3600) / 60;
    calendar.sec = (temp % 3600) % 60;
    calendar.week = RTC_Get_Week(calendar.w_year, calendar.w_month, calendar.w_date);
    return 0;
}

//
u8 RTC_Get_Week(u16 year, u8 month, u8 day)
{
    u16 temp2;
    u8  yearH, yearL;

    yearH = year / 100;
    yearL = year % 100;
    if(yearH > 19)
        yearL += 100;
    temp2 = yearL + yearL / 4;
    temp2 = temp2 % 7;
    temp2 = temp2 + day + table_week[month - 1];
    if(yearL % 4 == 0 && month < 3)
        temp2--;
    return (temp2 % 7);
}

//
uint8_t RTC_GetITStatus(uint16_t RTC_IT)
{
    uint8_t bitstatus = RESET;

    bitstatus = (uint8_t)(RTC->CTLRL & RTC_IT);
    if(((RTC->CTLRH & RTC_IT) != (uint16_t)RESET) && (bitstatus != (uint16_t)RESET))
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }
    return bitstatus;
}

//
uint32_t RTC_GetCounter(void)
{
    uint16_t high1 = 0, high2 = 0, low = 0;

    high1 = RTC->CNTH;
    low = RTC->CNTL;
    high2 = RTC->CNTH;

    if(high1 != high2)
    {
        return (((uint32_t)high2 << 16) | RTC->CNTL);
    }
    else
    {
        return (((uint32_t)high1 << 16) | low);
    }
}




#endif /* __RTC_H_ */
