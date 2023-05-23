
#include "debug.h"
#include <ch32v30x.h>
#include <ch32v30x_eth.h>


#define USE_10M_BASE                            1  /* Internal 10M PHY */
#define USE_MAC_MII                             2

#define PHY_ADDRESS 1
#define PHY_MODE    USE_MAC_MII

#define MY_MAC_ADD_0 0x33
#define MY_MAC_ADD_1 0x35
#define MY_MAC_ADD_2 0x43
#define MY_MAC_ADD_3 0x38
#define MY_MAC_ADD_4 0xF3
#define MY_MAC_ADD_5 0xA3

#define ETH_RXBUFNB 1
#define ETH_TXBUFNB 1

ETH_InitTypeDef ETH_InitStructure;
ETH_DMADESCTypeDef  DMARxDscrTab[ETH_RXBUFNB], DMATxDscrTab[ETH_TXBUFNB];
u8 Rx_Buff[ETH_RXBUFNB][ETH_MAX_PACKET_SIZE], Tx_Buff[ETH_TXBUFNB][ETH_MAX_PACKET_SIZE];
u8 data[ETH_MAX_PACKET_SIZE];

u8 macAddr[6] = {MY_MAC_ADD_0,MY_MAC_ADD_1,MY_MAC_ADD_2,MY_MAC_ADD_3,MY_MAC_ADD_4,MY_MAC_ADD_5};

 __attribute__((__aligned__(4))) ETH_DMADESCTypeDef DMARxDscrTab[ETH_RXBUFNB];      /* MAC receive descriptor, 4-byte aligned*/
 __attribute__((__aligned__(4))) ETH_DMADESCTypeDef DMATxDscrTab[ETH_TXBUFNB];      /* MAC send descriptor, 4-byte aligned */

 __attribute__((__aligned__(4))) uint8_t  MACRxBuf[ETH_RXBUFNB*ETH_MAX_PACKET_SIZE];     /* MAC receive buffer, 4-byte aligned */
 __attribute__((__aligned__(4))) uint8_t  MACTxBuf[ETH_TXBUFNB*ETH_MAX_PACKET_SIZE];     /* MAC send buffer, 4-byte aligned */

void GPIO_Toggle_INIT(void);
void Ethernet_Init(void);
void RecievePacket(void);
void ETH_Init( uint8_t *macAddr );
void ETH_Configuration( uint8_t *macAddr );


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
    // int Value = ETH_Init(&ETH_InitStructure, PHY_ADDRESS);

    ETH_Init(&macAddr);

    
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

//из файла NetLib/eth_driver.c
void ETH_Init( uint8_t *macAddr )
{
    Delay_Ms(100);
    ETH_Configuration( macAddr );
    ETH_DMATxDescChainInit(DMATxDscrTab, MACTxBuf, ETH_TXBUFNB);
    ETH_DMARxDescChainInit(DMARxDscrTab, MACRxBuf, ETH_RXBUFNB);
    // pDMARxSet = DMARxDscrTab;
    // pDMATxSet = DMATxDscrTab;
    // NVIC_EnableIRQ(ETH_IRQn);
}

// ETH_IRQHandler() {}


void ETH_Configuration( uint8_t *macAddr )
{
    ETH_InitTypeDef ETH_InitStructure;
    uint16_t timeout = 10000;

    /* Enable Ethernet MAC clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ETH_MAC|RCC_AHBPeriph_ETH_MAC_Tx|RCC_AHBPeriph_ETH_MAC_Rx,ENABLE);

    u8 gPHYAddress = PHY_ADDRESS;
#if( PHY_MODE ==  USE_10M_BASE )
    ETH_SetClock( );
    /* Enable internal 10BASE-T PHY*/
    EXTEN->EXTEN_CTR |= EXTEN_ETH_10M_EN;    /* Enable 10M Ethernet physical layer   */
#elif( PHY_MODE ==  USE_MAC_MII)
    /*  Enable MII GPIO */
    ETH_MIIPinInit( );
#endif
    /* Reset ETHERNET on AHB Bus */
    ETH_DeInit();

    /* Software reset */
    ETH_SoftwareReset();

    /* Wait for software reset */
    do{
        Delay_Us(10);
        if( !--timeout )  break;
    }while(ETH->DMABMR & ETH_DMABMR_SR);

    // ChipVerNum = GET_CHIP_VER();
    /* ETHERNET Configuration */
    /* Call ETH_StructInit if you don't like to configure all ETH_InitStructure parameter */
    ETH_StructInit(&ETH_InitStructure);
    /* Fill ETH_InitStructure parameters */
    /*------------------------   MAC   -----------------------------------*/
    ETH_InitStructure.ETH_Mode = ETH_Mode_FullDuplex;
#if( PHY_MODE ==  USE_10M_BASE )
    ETH_InitStructure.ETH_Speed = ETH_Speed_10M;
#else
    ETH_InitStructure.ETH_Speed = ETH_Speed_100M;
#endif
    ETH_InitStructure.ETH_AutoNegotiation = ETH_AutoNegotiation_Enable  ;
    ETH_InitStructure.ETH_LoopbackMode = ETH_LoopbackMode_Disable;
    ETH_InitStructure.ETH_RetryTransmission = ETH_RetryTransmission_Disable;
    ETH_InitStructure.ETH_AutomaticPadCRCStrip = ETH_AutomaticPadCRCStrip_Disable;
    ETH_InitStructure.ETH_ReceiveAll = ETH_ReceiveAll_Enable;
    ETH_InitStructure.ETH_BroadcastFramesReception = ETH_BroadcastFramesReception_Enable;
    ETH_InitStructure.ETH_PromiscuousMode = ETH_PromiscuousMode_Enable;
    ETH_InitStructure.ETH_MulticastFramesFilter = ETH_MulticastFramesFilter_Perfect;
    ETH_InitStructure.ETH_UnicastFramesFilter = ETH_UnicastFramesFilter_Perfect;
    /*------------------------   DMA   -----------------------------------*/
    /* When we use the Checksum offload feature, we need to enable the Store and Forward mode:
    the store and forward guarantee that a whole frame is stored in the FIFO, so the MAC can insert/verify the checksum,
    if the checksum is OK the DMA can handle the frame otherwise the frame is dropped */
    ETH_InitStructure.ETH_DropTCPIPChecksumErrorFrame = ETH_DropTCPIPChecksumErrorFrame_Enable;
    ETH_InitStructure.ETH_ReceiveStoreForward = ETH_ReceiveStoreForward_Enable;
    ETH_InitStructure.ETH_TransmitStoreForward = ETH_TransmitStoreForward_Enable;
    ETH_InitStructure.ETH_ForwardErrorFrames = ETH_ForwardErrorFrames_Enable;
    ETH_InitStructure.ETH_ForwardUndersizedGoodFrames = ETH_ForwardUndersizedGoodFrames_Enable;
    ETH_InitStructure.ETH_SecondFrameOperate = ETH_SecondFrameOperate_Disable;
    /* Configure Ethernet */
    ETH_RegInit( &ETH_InitStructure, gPHYAddress );
#if( PHY_MODE ==  USE_10M_BASE )
    /* Enable the Ethernet Rx Interrupt */
    ETH_DMAITConfig( ETH_DMA_IT_NIS |\
      ETH_DMA_IT_R |\
      ETH_DMA_IT_T |\
      ETH_DMA_IT_PHYLINK,\
      ENABLE );
#else
    /* Enable the Ethernet Rx Interrupt */
    ETH_DMAITConfig( ETH_DMA_IT_NIS | ETH_DMA_IT_R | ETH_DMA_IT_T, ENABLE );
#endif
}