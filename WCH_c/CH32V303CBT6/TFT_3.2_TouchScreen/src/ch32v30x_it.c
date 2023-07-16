
#include <ch32v30x.h>
#include "ch32v30x_it.h"

#include "xpt2046.h"
#include "tft_shield_3_2.h"

// volatile uint8_t status = 0;
volatile uint16_t x_kor, y_kor;

void NMI_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void HardFault_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void EXTI1_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

/*********************************************************************
 * @fn      NMI_Handler
 *
 * @brief   This function handles NMI exception.
 *
 * @return  none
 */
void NMI_Handler(void)
{
}

/*********************************************************************
 * @fn      HardFault_Handler
 *
 * @brief   This function handles Hard Fault exception.
 *
 * @return  none
 */
void HardFault_Handler(void)
{
  while (1)
  {
  }
}


/*********************************************************************
 * @fn      EXTI1_IRQHandler
 *
 * @brief   This function handles EXTI1 Handler.
 *
 * @return  none
 */
void EXTI1_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line1)!=RESET)
    {
        // status = 1;
        XPT_GetTouch_xy(&x_kor, &y_kor);
        if (x_kor <= 240 && y_kor <= 320) {
#ifdef __DEBUG__
        	printf("Run at EXTI\r\n");
            printf("x_kor: %d\r\n", x_kor);
            printf("y_kor: %d\r\n\r\n", y_kor);
#endif /* __DEBUG__ */
			Lcd_Dot(x_kor, y_kor, 0xf000);
        }
      EXTI_ClearITPendingBit(EXTI_Line1);     /* Clear Flag */
    }
}

