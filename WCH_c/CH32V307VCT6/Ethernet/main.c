
#include "debug.h"
#include <ch32v30x.h>
#include <ch32v30x_eth.h>

//PHY_ADDRESS определяется подключением ног PHYAD[0:4] у RTL8201 см. стр. 19 datasheet
#define PHY_ADDRESS 0x01F /* PHY_ADDRESS TE-STM32F107 Board */
#define MII_MODE          /*MII mode or RMII mode*/

#define MY_MAC_ADD_0 0x33
#define MY_MAC_ADD_1 0x35
#define MY_MAC_ADD_2 0x43
#define MY_MAC_ADD_3 0x38
#define MY_MAC_ADD_4 0xF3
#define MY_MAC_ADD_5 0xA3

#define ETH_RXBUFNB 8
#define ETH_TXBUFNB 8

ETH_InitTypeDef ETH_InitStructure;
ETH_DMADESCTypeDef  DMARxDscrTab[ETH_RXBUFNB], DMATxDscrTab[ETH_TXBUFNB];
u8 Rx_Buff[ETH_RXBUFNB][ETH_MAX_PACKET_SIZE], Tx_Buff[ETH_TXBUFNB][ETH_MAX_PACKET_SIZE];


void GPIO_Toggle_INIT(void);
void Ethernet_Init(void);
void RecievePacket(void);


int main(void)
{
    // SysTick_Config(SystemCoreClock/1000);
    // RCC_Configuration();
    // NVIC_Configuration();
    // GPIO_PinRemapConfig(GPIO_Remap_ETH, DISABLE);
    // GPIO_ETH_MediaInterfaceConfig(GPIO_ETH_MediaInterface_MII);
    // RCC->CFGR |= RCC_CFGR_MCO_HSE;
    // GPIO_Configuration();


    u8 i = 0;
    // NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    SystemCoreClockUpdate();
    // Delay_Init();
    USART_Printf_Init(115200);	
    printf("SystemClk:%d\r\n", SystemCoreClock);
    printf("GPIO Toggle TEST\r\n");
    GPIO_Toggle_INIT();

    Ethernet_Init();

    while(1)
    {
        RecievePacket();
        // Delay_Ms(500);
        // GPIO_WriteBit(GPIOA, GPIO_Pin_0, (i == 0) ? (i = Bit_SET) : (i = Bit_RESET));
    }
}


void GPIO_Toggle_INIT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}



void Ethernet_Init(void)
{
    // сброс настроек в начальное состояние Ethernet
    ETH_DeInit();

    // сбрасываем Ethernet
    ETH_SoftwareReset();

    // ждём подтверждения, что Ethernet сбросился
    while(ETH_GetSoftwareResetStatus()==SET);
    
    // конфигурируем Ethernet
    ETH_StructInit(&ETH_InitStructure);

    // Устанавливаем параметры ETH_InitStructure
    ETH_InitStructure.ETH_AutoNegotiation = ETH_AutoNegotiation_Enable;
    ETH_InitStructure.ETH_Speed = ETH_Speed_100M;
    ETH_InitStructure.ETH_LoopbackMode = ETH_LoopbackMode_Disable;
    ETH_InitStructure.ETH_Mode = ETH_Mode_FullDuplex;
    ETH_InitStructure.ETH_RetryTransmission = ETH_RetryTransmission_Disable;
    ETH_InitStructure.ETH_AutomaticPadCRCStrip = ETH_AutomaticPadCRCStrip_Disable;
    // Принимать всё подряд
    // ETH_InitStructure.ETH_ReceiveAll = ETH_ReceiveAll_Enable;
    ETH_InitStructure.ETH_BroadcastFramesReception = ETH_BroadcastFramesReception_Enable;
    ETH_InitStructure.ETH_PromiscuousMode = ETH_PromiscuousMode_Disable;
    ETH_InitStructure.ETH_MulticastFramesFilter = ETH_MulticastFramesFilter_Perfect;
    ETH_InitStructure.ETH_UnicastFramesFilter = ETH_UnicastFramesFilter_Perfect;

    // производим инициализацию параметров
    int Value = ETH_Init(&ETH_InitStructure, PHY_ADDRESS);

    
    /* Initialize Tx Descriptors list: Chain Mode */
    ETH_DMATxDescChainInit(DMATxDscrTab, &Tx_Buff[0][0], ETH_TXBUFNB);
    /* Initialize Rx Descriptors list: Chain Mode  */
    ETH_DMARxDescChainInit(DMARxDscrTab, &Rx_Buff[0][0], ETH_RXBUFNB);

    //Разрешаем прием
    DMARxDscrTab -> Status = ETH_DMARxDesc_OWN;

    /* Enable MAC and DMA transmission and reception */
    ETH_Start(); 

}


void RecievePacket(void)
{
    if ((DMARxDscrTab->Status & ETH_DMARxDesc_OWN)==0)
    {   
        //Сначала отключаем прием
        ETH_DMAReceptionCmd(DISABLE);
        for (u8 i=0; i<43; i++)
        {      
            data[i]=Rx_Buff[0][i];
        }
        //отдаем дескриптор в руки DMA Ethernet
        DMARxDscrTab -> Status = ETH_DMARxDesc_OWN;
        ETH_DMAReceptionCmd(ENABLE);
   }
}

