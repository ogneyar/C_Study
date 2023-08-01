//
#include <stdint.h>

#define MDR_RST_CLK    				(0x40020000)
#define MDR_RST_CLK_PER_CLOCK   	(*(uint32_t*)(MDR_RST_CLK + 0x1c))

#define PER_CLOCK_PCLK_EN_PORTB		(1u << 22u)
#define PER_CLOCK_PCLK_EN_PORTC		(1u << 23u)

#define MDR_PORTx_RXTX              (0x00)
#define MDR_PORTx_OE                (0x04)
#define MDR_PORTx_ANALOG            (0x0c)
#define MDR_PORTx_PWR               (0x18)

#define MDR_PORTB                   (0x400b0000)
#define MDR_PORTB_ANALOG            (*(uint32_t*)(MDR_PORTB + MDR_PORTx_ANALOG))
#define MDR_PORTB_RXTX            	(*(uint32_t*)(MDR_PORTB + MDR_PORTx_RXTX))
#define MDR_PORTB_OE            	(*(uint32_t*)(MDR_PORTB + MDR_PORTx_OE))
#define MDR_PORTB_PWR            	(*(uint32_t*)(MDR_PORTB + MDR_PORTx_PWR))

#define MDR_PORTC                   (0x400b8000)
#define MDR_PORTC_ANALOG            (*(uint32_t*)(MDR_PORTC + MDR_PORTx_ANALOG))
#define MDR_PORTC_RXTX            	(*(uint32_t*)(MDR_PORTC + MDR_PORTx_RXTX))
#define MDR_PORTC_OE            	(*(uint32_t*)(MDR_PORTC + MDR_PORTx_OE))
#define MDR_PORTC_PWR            	(*(uint32_t*)(MDR_PORTC + MDR_PORTx_PWR))
 
#define PB1 						1
#define PB2 						2
#define PC2 						2
#define LED_HL1 					(1 << PB1) // red
#define LED_HL2 					(1 << PB2) // green
#define LED_HL3 					(1 << PC2) // blue

uint16_t interval = 100;

void GPIO_Init(void);
void delay(uint16_t inter);


__attribute__((noreturn))
void Reset_Handler(){
    
	GPIO_Init();
	
	while(1)
	{	
		MDR_PORTB_RXTX ^= LED_HL1;
		MDR_PORTB_RXTX ^= LED_HL2;	
		MDR_PORTC_RXTX ^= LED_HL3;
		
		delay(interval);
	}
}
 
// Объявим тип - указатель на прерывание
typedef void (*isr_routine)(void);
 
// Опишем структуру таблицы векторов прерываний
typedef struct  {
    const uint32_t * stack_top;
    const isr_routine reset;
} ISR_VECTOR_t;
 
//Получим адрес указателья на стек из скрипта линковки
extern const uint32_t __StackTop;
 
//Укажем линковщику, что эту константу нужно положить в секцию .isr_vector
__attribute__((section(".isr_vector"), __unused__))
const ISR_VECTOR_t  isr_vector = {
    .stack_top = &__StackTop,
    .reset  = &Reset_Handler,
};

//
void GPIO_Init(void) 
{
	MDR_RST_CLK_PER_CLOCK |= PER_CLOCK_PCLK_EN_PORTB | PER_CLOCK_PCLK_EN_PORTC;
	
	MDR_PORTB_ANALOG |= LED_HL1 | LED_HL2;
	MDR_PORTB_OE |= LED_HL1 | LED_HL2; 
	MDR_PORTB_PWR |= (0x1<<(PB1*2)) | (0x1<<(PB2*2)); // 01 - lower
	
	MDR_PORTC_ANALOG |= LED_HL3;
	MDR_PORTC_OE |= LED_HL3; 
	MDR_PORTC_PWR |= (0x1<<(PC2*2)); // 01 - lower
		
	MDR_PORTB_RXTX |= LED_HL1;
	MDR_PORTB_RXTX &= ~LED_HL2;	
	MDR_PORTC_RXTX |= LED_HL3;
}	

// delay
void delay(uint16_t inter)
{
	for(uint32_t i = 0; i < (uint32_t)(inter<<9); i++) __asm("nop");
}
