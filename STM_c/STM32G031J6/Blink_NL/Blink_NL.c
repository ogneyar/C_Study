//
#include "Blink_NL.h"

#define PERIPH_BASE             (0x40000000UL)  /*!< Peripheral base address */
#define AHBPERIPH_BASE          (PERIPH_BASE + 0x00020000UL)
#define RCC_BASE                (AHBPERIPH_BASE + 0x00001000UL)

#define RCC_IOPENR              (*(uint32_t*)(RCC_BASE + 0x34))
#define RCC_IOPENR_GPIOBEN_Pos  (1U)
#define RCC_IOPENR_GPIOBEN_Msk  (0x1UL << RCC_IOPENR_GPIOBEN_Pos)      /*!< 0x00000002 */
#define RCC_IOPENR_GPIOBEN      RCC_IOPENR_GPIOBEN_Msk

#define IOPORT_BASE             (0x50000000UL)  /*!< IOPORT base address */
#define GPIOB_BASE              (IOPORT_BASE + 0x00000400UL)

#define GPIOB_MODER             (*(uint32_t*)(GPIOB_BASE + 0x00))
#define GPIO_MODER_MODE7_Pos    (14U)
#define GPIO_MODER_MODE7_Msk    (0x3UL << GPIO_MODER_MODE7_Pos)          /*!< 0x0000C000 */
#define GPIO_MODER_MODE7        GPIO_MODER_MODE7_Msk
#define GPIO_MODER_MODE7_0      (0x1UL << GPIO_MODER_MODE7_Pos)          /*!< 0x00004000 */
#define GPIO_MODER_MODE7_1      (0x2UL << GPIO_MODER_MODE7_Pos)          /*!< 0x00008000 */

#define GPIOB_ODR               (*(uint32_t*)(GPIOB_BASE + 0x14))
#define GPIO_ODR_OD7_Pos        (7U)
#define GPIO_ODR_OD7_Msk        (0x1UL << GPIO_ODR_OD7_Pos)              /*!< 0x00000080 */
#define GPIO_ODR_OD7            GPIO_ODR_OD7_Msk

#define GPIOB_BSRR              (*(uint32_t*)(GPIOB_BASE + 0x18))
#define GPIO_BSRR_BS7_Pos       (7U)
#define GPIO_BSRR_BS7_Msk       (0x1UL << GPIO_BSRR_BS7_Pos)             /*!< 0x00000080 */
#define GPIO_BSRR_BS7           GPIO_BSRR_BS7_Msk
#define GPIO_BSRR_BR7_Pos       (23U)
#define GPIO_BSRR_BR7_Msk       (0x1UL << GPIO_BSRR_BR7_Pos)             /*!< 0x00800000 */
#define GPIO_BSRR_BR7           GPIO_BSRR_BR7_Msk

#define GPIOB_BRR               (*(uint32_t*)(GPIOB_BASE + 0x28))
#define GPIO_BRR_BR7_Pos        (7U)
#define GPIO_BRR_BR7_Msk        (0x1UL << GPIO_BRR_BR7_Pos)              /*!< 0x00000080 */
#define GPIO_BRR_BR7            GPIO_BRR_BR7_Msk

#define GPIO_Pin7               GPIO_BSRR_BS7
  

// all functions
void GPIO_Init(void);
void delay(uint16_t inter);

static uint16_t interval = 100;


int main(void) 
{
    /* Initialize all configured peripherals */
	GPIO_Init();
	
    while (1)
    {
        // Переключаем пин 7 на порте B
        GPIOB_ODR ^= GPIO_ODR_OD7;
        delay(interval);

        // либо так
        // GPIOB_BSRR |= GPIO_BSRR_BS7; // set
        // delay(interval);
        // GPIOB_BSRR |= GPIO_BSRR_BR7;  // reset
        // delay(interval);

        // либо вот так
        // GPIOB_BSRR |= (uint32_t)GPIO_Pin7; // set
        // delay(interval);
        // GPIOB_BRR |= (uint32_t)GPIO_Pin7;  // reset
        // delay(interval);
    }	
}


// PB7
void GPIO_Init(void) 
{
    //Разрешаем тактирование GPIOB
	RCC_IOPENR |= RCC_IOPENR_GPIOBEN; // Input Output Port B Enable
	
	// включаем порт на выход
    GPIOB_MODER &= ~GPIO_MODER_MODE7;	/* clear mode for PB7 */
	GPIOB_MODER |= GPIO_MODER_MODE7_0; // 01: General purpose output mode (PB7)
}

// delay
void delay(uint16_t inter) {
	for(uint32_t i = 0; i < (uint32_t)(inter<<10); i++) __asm("nop");
}
