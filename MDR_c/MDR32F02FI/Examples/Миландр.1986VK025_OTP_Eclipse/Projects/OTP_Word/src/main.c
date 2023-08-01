
#include "MLDR187_lib.h"
#include "MLDR187.h"

#include "MLDR187_gpio.h"
#include "MLDR187_rst_clk.h"
#include "MLDR187_otp.h"


// Demoboard LEDs definitions
#define	LED_PORT		MDR_PORTD
#define	LED_PORT_CLK	RST_CLK_PORTD
#define LED_PIN_1		PORT_Pin_0
#define LED_PIN_2		PORT_Pin_1
#define LED_PIN_3		PORT_Pin_2
#define LED_PIN_4		PORT_Pin_3


RST_CLK_CpuSpeed_InitTypeDef RST_CLK_CpuSpeed_InitStructure;
PORT_InitTypeDef PORT_InitStructure;


uint32_t DelayCnt = 0;


uint32_t WriteWord = 0xF1402D73;
volatile uint32_t ReadWord = 0;
uint32_t RW_OTP_Address = 0x000207D0;


void Setup_CPU_Clock(void);
void Setup_LED_Port(void);


uint32_t OTP_ReadWord(uint32_t addr);
void OTP_WriteWord(uint32_t addr, uint32_t otp_word);


/* Main function */
int main()
{
	// Setup MCU clock as HSE 8MHz
	Setup_CPU_Clock();
	// Setup OTP delays
	OTP_Init();
	// Setup demoboard LEDs for indication
    Setup_LED_Port();

    // Read word from OTP
    ReadWord = OTP_ReadWord(RW_OTP_Address);

    // if read word is not equal to target WriteWord - program OTP word
    if (ReadWord != WriteWord)
    {
    	OTP_WriteWord(RW_OTP_Address, WriteWord);
    }

    // Read word from OTP for validation
    ReadWord = OTP_ReadWord(RW_OTP_Address);

    // Indicate failed validation
    if(ReadWord != WriteWord)
    {
        while (1)
        {
			if (DelayCnt++ >= 0x00010000)
			{
				DelayCnt = 0;
				if (PORT_ReadPin(LED_PORT, LED_PIN_4) != 0)
					PORT_SetReset(LED_PORT, LED_PIN_3 | LED_PIN_4, RESET);
				else
					PORT_SetReset(LED_PORT, LED_PIN_3 | LED_PIN_4, SET);
			}
        }
    }


    // Successful validation
    while (1)
    {
        if (DelayCnt++ >= 0x0010000)
        {
            DelayCnt = 0;
            if (PORT_ReadPin(LED_PORT, LED_PIN_1) != 0)
                PORT_SetReset(LED_PORT, LED_PIN_1, RESET);
            else
                PORT_SetReset(LED_PORT, LED_PIN_1, SET);
        }
    }
}


void Setup_CPU_Clock()
{
    RST_CLK_CpuSpeed_InitTypeDef clkInit;

    clkInit.hseState = RST_CLK_HseOscillator;
    clkInit.hseSpeed = 8000000U;
    clkInit.cpuC1Src = RST_CLK_CpuC1SelHse;
    clkInit.pllState = RST_CLK_PllOff;
    //clkInit.pllMult = 6;
    clkInit.cpuC2Src = RST_CLK_CpuC2SelCpuC1;
    clkInit.cpuC3Div = RST_CLK_Div1;
    clkInit.hclkSrc = RST_CLK_HclkSelCpuC3;
    RST_CLK_SetupCpuSpeed(&clkInit);

    RST_CLK_SetupHsPeripheralClock(RST_CLK_Clk_PER1_C2, RST_CLK_ClkSrc_CPU_C1);
}


void Setup_LED_Port(void)
{
    PORT_InitStructure.PORT_OE    = PORT_OE_OUT;
    PORT_InitStructure.PORT_FUNC  = PORT_FUNC_PORT;
    PORT_InitStructure.PORT_MODE  = PORT_MODE_DIGITAL;
    PORT_InitStructure.PORT_SPEED = PORT_SPEED_SLOW_4mA;
    PORT_InitStructure.PORT_PULL_DOWN   = PORT_PULL_DOWN_OFF;
    PORT_Init(LED_PORT, (LED_PIN_1 | LED_PIN_2 | LED_PIN_3 | LED_PIN_4), &PORT_InitStructure);
    PORT_SetReset(LED_PORT, LED_PIN_1, SET);
    PORT_SetReset(LED_PORT, LED_PIN_2, SET);
    PORT_SetReset(LED_PORT, LED_PIN_3, SET);
    PORT_SetReset(LED_PORT, LED_PIN_4, SET);
}


uint32_t OTP_ReadWord(uint32_t addr_byte)
{
	uint32_t otp_word = 0;
	uint32_t addr_bit = 0;

	// align 4 bytes, mask 12 KB and convert to bit
	addr_bit = ((addr_byte & ~0x3) & 0x3FFF) * 8;

	// read 4 bytes as 32-bit word
	for(uint32_t i = 0; i < 4; i++)
	{
		// address should be in bits
		otp_word |= ((uint32_t)OTP_Read(addr_bit + i*8) << 8*i);
	}

	return otp_word;
}


void OTP_WriteWord(uint32_t addr_byte, uint32_t otp_word)
{
	uint8_t byteVal;
	uint32_t addr_bit = 0;

	// align 4 bytes, mask 12 KB and convert to bit
	addr_bit = ((addr_byte & ~0x3) & 0x3FFF) * 8;

	for(uint32_t i = 0; i < 4; i++)
	{
		// take 1 byte of word
		byteVal = (uint8_t)((otp_word >> 8*i) & 0xFF);
		// address should be in bits
		OTP_WriteByte(addr_bit + i*8, byteVal);
	}
}

