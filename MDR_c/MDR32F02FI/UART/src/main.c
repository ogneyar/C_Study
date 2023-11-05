//
// Created by panukov.a on 05.03.2021.
//

#include <MLDR187_bkp.h>
// #include <MLDR187_iwdg.h>
#include <stdlib.h>

#include "wait.h"
#include <stdio.h>
#include <math.h>
#include <MLDR187_uart.h>
#include <MLDR187_gpio.h>
#include "MLDR187_rst_clk.h"

/* Private define ------------------------------------------------------------*/
#define TRIMMING    1.55
#define TEST_N      10
#define TEST_PERIOD 1
#define	LED_PORT		MDR_PORTD
#define	LED_PORT_CLK	RST_CLK_PORTD
#define LED_PIN_0		PORT_Pin_0
#define LED_PIN_1		PORT_Pin_1
#define LED_PIN_2		PORT_Pin_2
#define LED_PIN_3		PORT_Pin_3

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
PORT_InitTypeDef PORT_InitStructure;
uint32_t DelayCnt = 0;
uint8_t fast = 1, slow = 0;

void init_clock();
void init_led();
void init_uart();
void blink(uint8_t);
void error();


int main() {
    init_clock();
    init_led();
    init_uart();

    printf("Hell o!\n");    
   
    blink(slow);

    return 0;
}


void init_clock() {
    ErrorStatus erCode;
    RST_CLK_CpuSpeed_InitTypeDef clkInit;

    clkInit.hseState = RST_CLK_HseOscillator;
    clkInit.hseSpeed = 8000000U;
    clkInit.cpuC1Src = RST_CLK_CpuC1SelHse;
    clkInit.pllState = RST_CLK_PllFromHse;
    clkInit.pllMult = 6;
    clkInit.cpuC2Src = RST_CLK_CpuC2SelPllCpu;
    clkInit.cpuC3Div = RST_CLK_Div1;
    clkInit.hclkSrc = RST_CLK_HclkSelCpuC3;
    erCode = RST_CLK_SetupCpuSpeed(&clkInit);
    if (erCode != SUCCESS) {
        error();
    }

    erCode = RST_CLK_SetupHsPeripheralClock(RST_CLK_Clk_PER1_C2, RST_CLK_ClkSrc_CPU_C1);
    if (erCode != SUCCESS) {
        error();
    }

    BKP_InitTypeDef bkpInit;
    BKP_StructInitDefault(&bkpInit);
    bkpInit.RTCsrc = bkpRtc_HSI;
    BKP_Init(&bkpInit);

    BKP_FreqGenCmd(bkpLsi, ENABLE, DISABLE);
}


void init_led()
{ 
    PORT_InitStructure.PORT_OE    = PORT_OE_OUT;
    PORT_InitStructure.PORT_FUNC  = PORT_FUNC_PORT;
    PORT_InitStructure.PORT_MODE  = PORT_MODE_DIGITAL;
    PORT_InitStructure.PORT_SPEED = PORT_SPEED_SLOW_4mA;
    PORT_InitStructure.PORT_PULL_DOWN   = PORT_PULL_DOWN_OFF;
    PORT_Init(LED_PORT, (LED_PIN_0 | LED_PIN_1 | LED_PIN_2 | LED_PIN_3), &PORT_InitStructure);
    PORT_SetReset(LED_PORT, LED_PIN_0, RESET);
    PORT_SetReset(LED_PORT, LED_PIN_1, SET);
    PORT_SetReset(LED_PORT, LED_PIN_2, RESET);
    PORT_SetReset(LED_PORT, LED_PIN_3, SET);

}


void init_uart()
{
    UART_InitTypeDef initStruct;
    UART_StructInitDefault(&initStruct);
    initStruct.baudRate = 115200;
    initStruct.fifoEn = DISABLE;
    UART_Init(MDR_UART1, &initStruct);

    PORT_InitTypeDef PORT_InitStructure;

    PORT_InitStructure.PORT_OE    = PORT_OE_OUT;
    PORT_InitStructure.PORT_FUNC  = PORT_FUNC_MAIN;
    PORT_InitStructure.PORT_MODE  = PORT_MODE_DIGITAL;
    PORT_InitStructure.PORT_SPEED = PORT_SPEED_FAST_2mA;
    PORT_InitStructure.PORT_PULL_DOWN   = PORT_PULL_DOWN_OFF;
    PORT_Init(MDR_PORTB, (PORT_Pin_0), &PORT_InitStructure); // UART1_TX

    PORT_InitStructure.PORT_OE    = PORT_OE_IN;
    PORT_Init(MDR_PORTB, (PORT_Pin_1), &PORT_InitStructure); // UART1_RX

    UART_Cmd(MDR_UART1, ENABLE);
}


void blink(uint8_t fast) {
    uint32_t delay = 0x00100000;
    if (fast) delay = 0x00010000;
    while (1)
    {
        if (DelayCnt++ >= delay)
        {
            DelayCnt = 0;
            if (PORT_ReadPin(LED_PORT, LED_PIN_0) != RESET)
            {
                PORT_SetReset(LED_PORT, LED_PIN_0, RESET);
                PORT_SetReset(LED_PORT, LED_PIN_1, SET);
                PORT_SetReset(LED_PORT, LED_PIN_2, RESET);
                PORT_SetReset(LED_PORT, LED_PIN_3, SET);
            }
            else
            {
                PORT_SetReset(LED_PORT, LED_PIN_0, SET);
                PORT_SetReset(LED_PORT, LED_PIN_1, RESET);
                PORT_SetReset(LED_PORT, LED_PIN_2, SET);
                PORT_SetReset(LED_PORT, LED_PIN_3, RESET);
            }
        }
    }
}

void error() {
    blink(fast);
}
