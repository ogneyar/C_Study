
/* Includes ------------------------------------------------------------------*/
#include "MLDR187.h"
#include "bootloader_config.h"

#define UART_CLK				8e6 // HSI 8 MHz

#define BAUD_RATE				9600

#define BR_DEV					30 // Baudrate Deviation [%]
#define WO_ERR_CNT				5

#define CMD_SYNC				'\0'
#define CMD_CR					'\r'
#define CMD_LOAD				'L'
#define CMD_VFY					'Y'
#define CMD_RUN					'R'
#define CMD_BAUD				'B'

#define REPLY_ERR				'E'
#define ERR_CHN					'i'
#define ERR_CMD					'c'
#define ERR_BAUD				'b'

#define REPLY_OK				'K'


#define BAUD_RATE_MIN			(BAUD_RATE * (1.0 - BR_DEV/100.0))
#define BAUD_RATE_MAX			(BAUD_RATE * (1.0 + BR_DEV/100.0))

#define GetDiv(clk, baud)		((float)clk/(16*baud))
#define GetIntDiv(baud)			((uint32_t)(64*GetDiv(UART_CLK, baud) + 0.5))

#define DIV_MIN					GetIntDiv(BAUD_RATE_MAX)
#define DIV_MAX					GetIntDiv(BAUD_RATE_MIN)
#define DIV_RANGE				(DIV_MAX - DIV_MIN)
#define DIV_DLT					(DIV_RANGE/BR_DEV/4 ? DIV_RANGE/BR_DEV/4 : 1)

#if USE_BOOTLOADER_VERSION
// BootloaderVersion is located at 0x0002_3FDF (OTP memory)
__attribute__((section(".boot_version"))) const uint8_t BootloaderVersion = BOOTLOADER_VERSION;
#endif

typedef enum
{
  mode_MCU = 0,
  mode_UART0,
  mode_UART1,
  mode_RAM
} BootModes;


typedef volatile unsigned char	vu8;
typedef volatile unsigned short	vu16;
typedef volatile unsigned long	vu32;


void UartModeInit(void);
void UartSetBaud(uint32_t divider);
void UartSendByte(uint32_t ch);
uint32_t UartReceiveByte(void);
uint32_t UartSyncErr(void);
uint32_t UartReceiveParam(void);
void UartWaitBusy(void);
void AF_Run(void *entry_point);
void InitPort(MDR_GPIO_TypeDef * Port);


/* Main function */
int main(void)
{
	uint32_t i;
	uint32_t Mode;
	uint32_t BKP_LDO;
	uint32_t tmp;

	MDR_RST_CLK->PER2_CLOCK = RST_CLK_PER2_CLOCK_RST_CLKEN | RST_CLK_PER2_CLOCK_BKPEN | RST_CLK_PER2_CLOCK_PORTCEN;

	BKP_LDO = MDR_BKP->LDO;
	if ((BKP_LDO & BKP_LDO_FPOR) == BKP_LDO_FPOR)
	{
		Mode = (BKP_LDO >> 27) & 3;
	}
	else
	{
		InitPort(MDR_PORTC);
		for ( i = 0; i < 100; i ++ )
		{
			tmp = *(vu32 *)0;
		}

		Mode = 0;
		Mode = (MDR_PORTC->RXTX) & 0x3;
		MDR_PORTC->PULL   = 0;
		MDR_PORTC->ANALOG = 0;
		MDR_PORTC->PWR    = 0;

		MDR_BKP->WPR = 0x8555AAA1;
		MDR_BKP->LDO = (BKP_LDO & (~(0xF<<26))) | (Mode<<27) | (1<<25);
		MDR_BKP->WPR = 0;
	}

	switch ( Mode )
	{
		case mode_MCU:
		{
			MDR_RST_CLK->PER2_CLOCK = RST_CLK_PER2_CLOCK_RST_CLKEN;
			asm("lui t0,0x10000");
			asm("jr  t0");
			break;        // unreachable code
		}

		case mode_RAM:
		{
			MDR_RST_CLK->PER2_CLOCK = RST_CLK_PER2_CLOCK_RST_CLKEN;
			asm("lui t0,0x80000");
			asm("jr  t0");
			break;        // unreachable code
		}

		case mode_UART0:
		case mode_UART1:
		{
			uint32_t divisor;
			uint32_t div_lo;
			uint32_t err;
			uint32_t sym;

			UartModeInit();

			do
			{
				div_lo = divisor = DIV_MIN;
				for ( ; (divisor < DIV_MAX) && (divisor - div_lo < DIV_DLT * WO_ERR_CNT); )
				{
					UartSetBaud(divisor);
					UartReceiveByte();
					divisor += DIV_DLT;
					if ( UartSyncErr() )
					{
						div_lo = divisor;
					}
				}
			} while ( divisor - div_lo < DIV_DLT*WO_ERR_CNT );

			for ( ; divisor < DIV_MAX; divisor += DIV_DLT )
			{
				UartSetBaud(divisor);
				UartReceiveByte();
				if ( UartSyncErr() )
				{
					break;
				}
			}

			divisor = (div_lo + divisor) >> 1;
			UartSetBaud(divisor);
			for ( i = 0; i < 16; i++ )
			{
				UartReceiveByte();
			}
			sym = CMD_CR;

			for (;;)
			{
				err = 0;
				if ( sym >> 8 )
				{
					err = ERR_CHN;
				}

				if ( !err )
				{
					switch ( sym )
					{
						case CMD_SYNC:
						{
							break;
						}

						case CMD_CR:
						{
							UartSendByte(CMD_CR);
							UartSendByte(0x0A);
							UartSendByte('>');
							break;
						}

						case CMD_BAUD:
							tmp = UartReceiveParam();
							if ( tmp == ~0UL )
							{
								err = ERR_CHN;
								break;
							}

							if ( !tmp )
							{
								err = ERR_BAUD;
								break;
							}

							tmp = ((uint32_t)(divisor * BAUD_RATE)) / tmp;

							if ( (tmp < 64) || (tmp >= (1<<22)) )
							{
								err = ERR_BAUD;
								break;
							}

							UartSendByte(CMD_BAUD);
							UartSetBaud(tmp);
							break;

						case CMD_LOAD:
						case CMD_VFY:
						{
							uint32_t addr, size;
							addr = UartReceiveParam();
							size = UartReceiveParam();
							if ( (addr == ~0UL) || (size == ~0UL) )
							{
								err = ERR_CHN;
								break;
							}
							UartSendByte(sym);

							size += addr;
							for ( ; addr < size; addr++ )
							{
								if ( sym == CMD_VFY )
								{
									UartSendByte(*(uint8_t *)addr);
								}
								else
								{
									uint32_t tmp = UartReceiveByte();
									if ( tmp >> 8 )
									{
										err = ERR_CHN;
										break;
									}
									*(uint8_t *)addr = tmp;
								}
							}

							if ( err )
							{
								break;
							}
							UartSendByte(REPLY_OK);
							break;
						}

						case CMD_RUN:
						{
							uint32_t tmp = UartReceiveParam();
							if ( tmp == ~0UL )
							{
								err = ERR_CHN;
								break;
							}
							UartSendByte(CMD_RUN);
							UartWaitBusy();

							AF_Run((void *)tmp);
							break;
						}

						default:
						{
							err = ERR_CMD;
						}
					}
				}

				if ( err )
				{
					UartSendByte(REPLY_ERR);
					UartSendByte(err);
				}

				sym = UartReceiveByte();
			}
		}
	}
	while (1);
}


void UartModeInit(void)
{
	MDR_RST_CLK->PER2_CLOCK = RST_CLK_PER2_CLOCK_UART1EN | RST_CLK_PER2_CLOCK_PORTBEN | RST_CLK_PER2_CLOCK_EEPROMEN | RST_CLK_PER2_CLOCK_RST_CLKEN;
	MDR_RST_CLK->UART_CLOCK = RST_CLK_UART_CLOCK_UART1CLKEN;

	MDR_PORTB->OE = 0;
	MDR_PORTB->FUNC = (GPIO_PORTx_FUNC_MAIN << GPIO_PORTx_FUNC_PIN_0_Pos) | (GPIO_PORTx_FUNC_MAIN << GPIO_PORTx_FUNC_PIN_1_Pos);
	MDR_PORTB->ANALOG = (GPIO_PORTx_ANALOG_DIGITAL << PORT_Pin_0_Pos) | (GPIO_PORTx_ANALOG_DIGITAL << PORT_Pin_1_Pos);
	MDR_PORTB->PULL = 0;
	MDR_PORTB->PWR = 2 << 0;
	MDR_PORTB->OE = GPIO_PORTx_OE_OUTPUT << PORT_Pin_0_Pos;
}


void UartSetBaud(uint32_t divider)
{
	while (MDR_UART1->FR & UART_FR_BUSY);
	MDR_UART1->CR = 0;
	MDR_UART1->IBRD = divider >> 6;
	MDR_UART1->FBRD = divider & 0x3F;
	MDR_UART1->LCR_H = (3 << UART_LCR_H_WLEN1_Pos);
	MDR_UART1->CR = (UART_CR_UARTEN | UART_CR_TXE | UART_CR_RXE);
}


void UartSendByte(uint32_t ch)
{
	uint32_t tmp;

	while ( (tmp = MDR_UART1->FR) & UART_FR_TXFF )
	{
		if ( tmp & UART_FR_RXFF )
		{
			MDR_UART1->DR;
		}
	}
    MDR_UART1->DR = ch;
}



uint32_t UartReceiveByte(void)
{
	while(MDR_UART1->FR & UART_FR_RXFE);
	return (MDR_UART1->DR << 20) >> 20;
}


uint32_t UartSyncErr(void)
{
	uint32_t i;
	for ( i = 0; i < 16; i++ )
	{
		while ( (MDR_UART1->FR & UART_FR_RXFE) );
		if ( (MDR_UART1->DR & 0xFFF) != CMD_SYNC )
		{
			return 1;
		}
	}
	return 0;
}


uint32_t UartReceiveParam(void)
{
	uint32_t tmp, res;
	for ( tmp = 0; tmp < 0x4000; tmp+=0x1000 )
	{
		while ( MDR_UART1->FR & UART_FR_RXFE );
		tmp |= MDR_UART1->DR & 0xFFF;
		res = (res >> 8) | (tmp << 24);
		tmp &= ~0xFF;
	}
	return (tmp & 0xF00) ? ~0UL : res;
}

void UartWaitBusy(void)
{
	while (MDR_UART1->FR & UART_FR_BUSY);
}

void AF_Run(void *entry_point)
{
	asm("mv t0,%[src]" : : [src] "r" (entry_point));
	asm("jr  t0");
}

void InitPort(MDR_GPIO_TypeDef * Port)
{
	Port->OE     = 0;
	Port->FUNC   = 0;
	Port->ANALOG = (1 << PORT_Pin_0_Pos) | (1 << PORT_Pin_1_Pos);
	Port->PULL   = (1 << PORT_Pin_0_Pos) | (1 << PORT_Pin_1_Pos);
	Port->PWR    = 0;
}
