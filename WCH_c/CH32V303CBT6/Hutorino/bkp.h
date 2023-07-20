
#ifndef __EXTI_H_
#define __EXTI_H_

#include "stdio.h"
#include <stdint.h>
#include "defines.h"
#include "irqn.h"


// Список функций
void BKP_Tamper_Init(void);
void BKP_Write(uint16_t BKP_DR, uint16_t Data);
uint8_t BKP_GetITStatus(void);


// инициализация BKP
void BKP_Tamper_Init(void)
{
	// RCC APB1 Periph Clock enable
    RCC->APB1PCENR |= RCC_APB1PCENR_PWREN;
    RCC->APB1PCENR |= RCC_APB1PCENR_BKPEN;

	// BKP Tamper Pin ConTRoL enable
	BKP->TPCTLR |= (1 << 0);
    // PWR Backup Access enable
	PWR->CTLR |= (1 << 8);
    // BKP Clears Tamper Pin Event pending flag.
    BKP->TPCSR |= BKP_CTE;

	BKP_Write(BKP_DR1, 0x9880);
	BKP_Write(BKP_DR2, 0x5678);
	BKP_Write(BKP_DR3, 0xABCD);
	BKP_Write(BKP_DR4, 0x3456);

	printf("BKP_DR1: 0x%04x\r\n", BKP->DATAR1);
	printf("BKP_DR2: 0x%04x\r\n", BKP->DATAR2);
	printf("BKP_DR3: 0x%04x\r\n", BKP->DATAR3);
    printf("BKP_DR4: 0x%04x\r\n", BKP->DATAR4);

	// BKP Tamper Pin Level High
	BKP->TPCTLR &= ~(1 << 1); //TPAL:0-PC13 set input-pull-down
	// BKP Tamper Pin Level Low
	// BKP->TPCTLR |= (1 << 1); //TPAL:1-PC13 input-pull-up

	// настройка контроллера прерываний
	uint8_t NVIC_IRQChannelPreemptionPriority = 1;
	NVIC->IPRIOR[(uint32_t)(TAMPER_IRQn)] = (NVIC_IRQChannelPreemptionPriority << 4);
	NVIC->IENR[((uint32_t)(TAMPER_IRQn) >> 5)] = (1 << ((uint32_t)(TAMPER_IRQn) & 0x1F));

	// BKP Interrupt enable
	BKP->TPCSR |= (1 << 2);

	// BKP Tamper Pin enable
	BKP->TPCTLR |= (1 << 0);
}

// запись данных в BKP
void BKP_Write(uint16_t BKP_DR, uint16_t Data)
{
    *(__IO uint32_t *)((uint32_t)BKP_BASE + BKP_DR) = Data;
}

// получение статуса
uint8_t BKP_GetITStatus(void)
{
    if(BKP->TPCSR & (1 << 9))
    {
        return SET;
    }
    else
    {
        return RESET;
    }
}


#endif /* __EXTI_H_ */
