
/*
 *@Note
 BKP routine:
 Intrusion detection pin (PC13), when an intrusion detection event occurs, all data backup register contents are cleared,
   And trigger the intrusion detection interrupt.

*/

#include "debug.h"
#include <ch32v30x.h>

#define LED_R       3 // PB3
#define LED_G       4 // PB4
#define LED_B       5 // PB5

#define TAMPER_IRQn 18


// Список функций
void GPIOB_Leds_Init(void);
void Leds_Blink(uint16_t delay);
void BKP_Tamper_Init(void);
static void TAMPER_NVIC_Config(void);


// Основная функция
int main(void)
{
	// NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	// SystemCoreClockUpdate();
    Delay_Init();
	USART_Printf_Init(115200);
	printf("SystemClk:%d\r\n",SystemCoreClock);
	printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );

    GPIOB_Leds_Init();

	BKP_Tamper_Init();

	while(1)
	{
		Leds_Blink(500);
	}
}


// Инициализация PA2, PB3, PB4, PB5
void GPIOB_Leds_Init(void)
{
    // включаем тактирование порта B
    RCC->APB2PCENR |= RCC_APB2Periph_GPIOB;
    // PB3
    GPIOB->CFGLR &= ~(0b1111 << LED_R*4); // GPIO_Msk
    GPIOB->CFGLR |= (0b0011 << LED_R*4); // GPIO_Speed_50
    // PB4
    GPIOB->CFGLR &= ~(0b1111 << LED_G*4); // GPIO_Msk
    GPIOB->CFGLR |= (0b0011 << LED_G*4); // GPIO_Speed_50
    GPIOB->BSHR |= (1 << LED_G);
    // PB5
    GPIOB->CFGLR &= ~(0b1111 << LED_B*4); // GPIO_Msk
    GPIOB->CFGLR |= (0b0011 << LED_B*4); // GPIO_Speed_50
}

// мигание светодиодов
void Leds_Blink(uint16_t delay)
{    
    GPIOB->OUTDR ^= (1 << LED_R);
    GPIOB->OUTDR ^= (1 << LED_G);
    GPIOB->OUTDR ^= (1 << LED_B);
    Delay_Ms(delay);
}

// инициализация BKP
void BKP_Tamper_Init(void)
{
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE );

	BKP_TamperPinCmd( DISABLE );
    PWR_BackupAccessCmd( ENABLE );
    BKP_ClearFlag();

	BKP_WriteBackupRegister( BKP_DR1, 0x9880 );
	BKP_WriteBackupRegister( BKP_DR2, 0x5678 );
	BKP_WriteBackupRegister( BKP_DR3, 0xABCD );
	BKP_WriteBackupRegister( BKP_DR4, 0x3456 );

	printf( "BKP_DR1: 0x%04x\r\n", BKP->DATAR1 );
	printf( "BKP_DR2: 0x%04x\r\n", BKP->DATAR2 );
	printf( "BKP_DR3: 0x%04x\r\n", BKP->DATAR3 );
    printf( "BKP_DR4: 0x%04x\r\n", BKP->DATAR4 );

	BKP_TamperPinLevelConfig( BKP_TamperPinLevel_High );  //TPAL:0-PC13 set input-pull-down
	// BKP_TamperPinLevelConfig( BKP_TamperPinLevel_Low );	 //TPAL:1-PC13 input-pull-up

	TAMPER_NVIC_Config();

	BKP_ITConfig( ENABLE );
	BKP_TamperPinCmd( ENABLE );
}

// настройка контроллера прерываний
static void TAMPER_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure={0};
	NVIC_InitStructure.NVIC_IRQChannel = TAMPER_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init( &NVIC_InitStructure );
}
