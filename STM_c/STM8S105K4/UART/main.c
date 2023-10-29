
// НЕ РАБОТАЕТ!!!

#include <stdint.h>
#include <stdio.h>

//константы для CLK
#define CLK_DIVR    (*(volatile uint8_t *)0x50c6)
#define CLK_PCKENR1    (*(volatile uint8_t *)0x50c7)
//константы для UART
#define UART1_SR    (*(volatile uint8_t *)0x5230)
#define UART1_DR    (*(volatile uint8_t *)0x5231)
#define UART1_BRR1    (*(volatile uint8_t *)0x5232)
#define UART1_BRR2    (*(volatile uint8_t *)0x5233)
#define UART1_CR2    (*(volatile uint8_t *)0x5235)
#define UART1_CR3    (*(volatile uint8_t *)0x5236)
#define UART_CR2_TEN (1 << 3)
#define UART_CR3_STOP2 (1 << 5)
#define UART_CR3_STOP1 (1 << 4)
#define UART_SR_TXE (1 << 7)

void fputc(char c) //вывод символа в UART
{
    while(!(UART1_SR & UART_SR_TXE));
    UART1_DR = c;
}



void main(void)
{
    CLK_DIVR = 0x00; //установка тактовой частоты 16 MГц
    CLK_PCKENR1 = 0xFF; //включение периферии
    UART1_CR2 = UART_CR2_TEN; //разрешение TX и RX
    UART1_CR3 &= ~(UART_CR3_STOP1 | UART_CR3_STOP2); //1 стоп-бит
    UART1_BRR2 = 0x03; UART1_BRR1 = 0x68; //9600 бод

    while(1)
    {
        printf("Hello,world!\r\n"); //вывод строки в UART
    }
}
