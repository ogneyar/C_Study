
//
#include "debug.h"
#include <ch32v00x.h>

#define I2C_BaudRate  80000

#define Size          7
#define TxAdderss     0x3c

u8 TxData[Size] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06};

void GPIOD_Pin0_Init(void);
void IIC_Init(u32 bound, u16 address);
void IIC_Scaner(void);


int main(void)
{
    u8 i = 0;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    Delay_Init();
    USART_Printf_Init(115200);
    printf("SystemClk:%d\r\n", SystemCoreClock);

    printf("I2C Scaner run\r\n");
    GPIOD_Pin0_Init();


    IIC_Init(I2C_BaudRate, TxAdderss);

    IIC_Scaner();

    while(1)
    {
        Delay_Ms(250);
        GPIO_WriteBit(GPIOD, GPIO_Pin_0, (i == 0) ? (i = Bit_SET) : (i = Bit_RESET));
    }
}

// инициализация PD0
void GPIOD_Pin0_Init(void)
{
    // включаем тактирование порта D
    // RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    RCC->APB2PCENR |= RCC_APB2Periph_GPIOD;
    
    GPIO_InitTypeDef GPIO_Init_Str = {0};
    GPIO_Init_Str.GPIO_Pin = GPIO_Pin_0;
    GPIO_Init_Str.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init_Str.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_Init_Str);
}

// инициализация I2C
void IIC_Init(u32 bound, u16 address)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    I2C_InitTypeDef  I2C_InitTSturcture = {0};
    // NVIC_InitTypeDef NVIC_InitStructure = {0};

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE );
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_I2C1, ENABLE );

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( GPIOC, &GPIO_InitStructure );

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( GPIOC, &GPIO_InitStructure );

    I2C_InitTSturcture.I2C_ClockSpeed = bound;
    I2C_InitTSturcture.I2C_Mode = I2C_Mode_I2C;
    I2C_InitTSturcture.I2C_DutyCycle = I2C_DutyCycle_16_9;
    I2C_InitTSturcture.I2C_OwnAddress1 = address;
    I2C_InitTSturcture.I2C_Ack = I2C_Ack_Enable;
    I2C_InitTSturcture.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_Init(I2C1, &I2C_InitTSturcture);

    I2C_Cmd(I2C1, ENABLE);
    I2C_CalculatePEC(I2C1, ENABLE);

    I2C_AcknowledgeConfig(I2C1, ENABLE);
}

//
void IIC_Scaner(void)
{

}


void I2C1_ER_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

/*********************************************************************
 * @fn      I2C1_ER_IRQHandler
 *
 * @brief   This function IIC PEC error exception.
 *
 * @return  none
 */
void I2C1_ER_IRQHandler(void )
{
    if( I2C_GetITStatus( I2C1, I2C_IT_PECERR ) != RESET )
    {
        printf( "PECEER\r\n" );
        I2C_ClearITPendingBit( I2C1, I2C_IT_PECERR );
    }
}
