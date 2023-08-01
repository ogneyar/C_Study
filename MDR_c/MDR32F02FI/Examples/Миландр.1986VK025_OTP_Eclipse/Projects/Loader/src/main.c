/**
  ******************************************************************************
  * @file    Template/src/main.c
  * @author  Milandr Application Team
  * @version V1.0.2
  * @date    28/12/2021
  * @brief   Main program body.
  ******************************************************************************
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, MILANDR SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  */

/* Includes ------------------------------------------------------------------*/
#include "MLDR187_lib.h"
#include "MLDR187.h"
#include "BootloaderCode.h"
#include "MLDR187_rst_clk.h"
#include "MLDR187_otp.h"

void Setup_CPU_Clock(void);

uint16_t c;
uint8_t  d;
uint8_t  d_read;
uint32_t addr;
uint16_t num_section;
uint16_t addr_byte;
uint16_t start_section;
uint16_t end_section;


/* Main function */
int main()
{
	// Setup MCU clock as HSE 8MHz
	Setup_CPU_Clock();
	// Setup OTP delays
	OTP_Init();

	// Program
	for(num_section = 0, c = 0; num_section < data_sections; num_section++)
	{
		start_section = data_address[num_section] & 0x3FFF;
		end_section = start_section + data_length_of_sections[num_section];

		for (addr_byte = start_section; addr_byte < end_section; addr_byte++, c++)
		{
			d = data[c];
			addr = (addr_byte * 8);
			OTP_WriteByte(addr, d);
		}
	}

	// Verify
	for(num_section = 0, c = 0; num_section < data_sections; num_section++)
	{
		start_section = data_address[num_section] & 0x3FFF;
		end_section = start_section + data_length_of_sections[num_section];

		for (addr_byte = start_section; addr_byte < end_section; addr_byte++, c++)
		{
			d = data[c];
			addr = (addr_byte * 8);
			d_read = OTP_Read(addr);
			if (d_read != d)
			{
				asm("EBREAK");
				while(1);
			}
		}
	}

	asm("EBREAK");
	while (1);
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

