
#include "debug.h"
// #include <wchnet.h>
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

/*********************************************************************
 * MAC queue configuration
 */
#define ETH_TXBUFNB                   2    /* The number of descriptors sent by the MAC  */

#define ETH_RXBUFNB                   4    /* Number of MAC received descriptors  */

#ifndef ETH_MAX_PACKET_SIZE
#define ETH_RX_BUF_SZE                1520  /* MAC receive buffer length, an integer multiple of 4 */
#define ETH_TX_BUF_SZE                1520  /* MAC send buffer length, an integer multiple of 4 */
#else
#define ETH_RX_BUF_SZE                ETH_MAX_PACKET_SIZE
#define ETH_TX_BUF_SZE                ETH_MAX_PACKET_SIZE
#endif

#define define_O(a,b) \
  GPIO_InitStructure.GPIO_Pin = b;\
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;\
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;\
  GPIO_Init(a, &GPIO_InitStructure)

#define define_I(a,b) \
  GPIO_InitStructure.GPIO_Pin = b;\
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;\
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;\
  GPIO_Init(a, &GPIO_InitStructure)

#define GET_CHIP_VER()   ((*((volatile uint32_t *)0x1FFFF704) & 0xF0) >> 4)
#define CHIP_C_VER_NUM   2
#define PHY_PN_SWITCH_AUTO (2<<2)

ETH_DMADESCTypeDef *pDMARxSet;
ETH_DMADESCTypeDef *pDMATxSet;

u16 LastPhyStat = 0;
u32 LastQueryPhyTime = 0;

// ETH_InitTypeDef ETH_InitStructure;
// ETH_DMADESCTypeDef  DMARxDscrTab[ETH_RXBUFNB], DMATxDscrTab[ETH_TXBUFNB];
// u8 Rx_Buff[ETH_RXBUFNB][ETH_MAX_PACKET_SIZE], Tx_Buff[ETH_TXBUFNB][ETH_MAX_PACKET_SIZE];

uint16_t gPHYAddress;
uint32_t volatile LocalTime;
uint8_t volatile ChipVerNum;

u8 data[ETH_MAX_PACKET_SIZE];

u8 macAddr[6] = {MY_MAC_ADD_0,MY_MAC_ADD_1,MY_MAC_ADD_2,MY_MAC_ADD_3,MY_MAC_ADD_4,MY_MAC_ADD_5};

 __attribute__((__aligned__(4))) ETH_DMADESCTypeDef DMARxDscrTab[ETH_RXBUFNB];      /* MAC receive descriptor, 4-byte aligned*/
 __attribute__((__aligned__(4))) ETH_DMADESCTypeDef DMATxDscrTab[ETH_TXBUFNB];      /* MAC send descriptor, 4-byte aligned */

 __attribute__((__aligned__(4))) uint8_t  MACRxBuf[ETH_RXBUFNB*ETH_RX_BUF_SZE];     /* MAC receive buffer, 4-byte aligned */
 __attribute__((__aligned__(4))) uint8_t  MACTxBuf[ETH_TXBUFNB*ETH_TX_BUF_SZE];     /* MAC send buffer, 4-byte aligned */

 __attribute__((__aligned__(4))) ETH_DMADESCTypeDef DMARxDscrTab[ETH_RXBUFNB];      /* MAC receive descriptor, 4-byte aligned*/


void GPIO_Toggle_INIT(void);
void RecievePacket(void);
void Ethernet_Init( uint8_t *macAddr );
void Ethernet_Configuration( uint8_t *macAddr );
void Ethernet_MIIPinInit(void);
uint32_t Ethernet_RegInit( ETH_InitTypeDef* ETH_InitStruct, uint16_t PHYAddress );
void Ethernet_ETHIsr(void);


int main(void)
{
    u8 i = 0;
    // NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    SystemCoreClockUpdate();
    Delay_Init();
    USART_Printf_Init(115200);
    printf("TcpServer Test\r\n");  	
    printf("SystemClk:%d\r\n", SystemCoreClock);
    // printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );
    // printf("net version:%x\n", WCHNET_GetVer());
    GPIO_Toggle_INIT();

    Ethernet_Init(&macAddr);

    while(1)
    {
        // RecievePacket();
        Delay_Ms(500);
        GPIO_WriteBit(GPIOA, GPIO_Pin_0, (i == 0) ? (i = Bit_SET) : (i = Bit_RESET));
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


void RecievePacket(void)
{
    if ((DMARxDscrTab->Status & ETH_DMARxDesc_OWN)==0)
    {   
        //Сначала отключаем прием
        ETH_DMAReceptionCmd(DISABLE);
        for (u8 i = 0; i < (ETH_RXBUFNB*ETH_RX_BUF_SZE); i++)
        {      
            data[i]=MACRxBuf[i];
        }
        //отдаем дескриптор в руки DMA Ethernet
        DMARxDscrTab -> Status = ETH_DMARxDesc_OWN;
        ETH_DMAReceptionCmd(ENABLE);
   }
}

//из файла NetLib/eth_driver.c
void Ethernet_Init( uint8_t *macAddr )
{
    Delay_Ms(100);
    Ethernet_Configuration( macAddr );
    ETH_DMATxDescChainInit(DMATxDscrTab, MACTxBuf, ETH_TXBUFNB);
    ETH_DMARxDescChainInit(DMARxDscrTab, MACRxBuf, ETH_RXBUFNB);
    pDMARxSet = DMARxDscrTab;
    pDMATxSet = DMATxDscrTab;
    NVIC_EnableIRQ(ETH_IRQn);
}

void ETH_IRQHandler(void)
{
    Ethernet_ETHIsr();
}


void Ethernet_Configuration( uint8_t *macAddr )
{
    ETH_InitTypeDef ETH_InitStructure;
    uint16_t timeout = 10000;

    /* Enable Ethernet MAC clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ETH_MAC|RCC_AHBPeriph_ETH_MAC_Tx|RCC_AHBPeriph_ETH_MAC_Rx,ENABLE);

    gPHYAddress = PHY_ADDRESS;

    /*  Enable MII GPIO */
    Ethernet_MIIPinInit();

    /* Reset ETHERNET on AHB Bus */
    ETH_DeInit();

    /* Software reset */
    ETH_SoftwareReset();

    /* Wait for software reset */
    do{
        Delay_Us(10);
        if( !--timeout )  break;
    }while(ETH->DMABMR & ETH_DMABMR_SR);

    ChipVerNum = GET_CHIP_VER();

    /* ETHERNET Configuration */
    /* Call ETH_StructInit if you don't like to configure all ETH_InitStructure parameter */
    ETH_StructInit(&ETH_InitStructure);
    /* Fill ETH_InitStructure parameters */
    /*------------------------   MAC   -----------------------------------*/
    ETH_InitStructure.ETH_Mode = ETH_Mode_FullDuplex;    
    ETH_InitStructure.ETH_Speed = ETH_Speed_100M;
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
    Ethernet_RegInit( &ETH_InitStructure, gPHYAddress );

    /* Enable the Ethernet Rx Interrupt */
    ETH_DMAITConfig( ETH_DMA_IT_NIS | ETH_DMA_IT_R | ETH_DMA_IT_T, ENABLE );
    
}


void Ethernet_MIIPinInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);

    define_O(GPIOA,GPIO_Pin_2);                                                         /* MDIO */
    define_O(GPIOC,GPIO_Pin_1);                                                         /* MDC */

    define_I(GPIOC,GPIO_Pin_3);                                                         /* txclk */
    define_O(GPIOB,GPIO_Pin_11);                                                        /* txen */
    define_O(GPIOB,GPIO_Pin_12);                                                        /* txd0 */
    define_O(GPIOB,GPIO_Pin_13);                                                        /* txd1 */
    define_O(GPIOC,GPIO_Pin_2);                                                         /* txd2 */
    define_O(GPIOB,GPIO_Pin_8);                                                         /* txd3 */
    /* RX */
    define_I(GPIOA,GPIO_Pin_1);                                                         /* PA1 RXC */
    define_I(GPIOA,GPIO_Pin_7);                                                         /* PA7 RXDV */
    define_I(GPIOC,GPIO_Pin_4);                                                         /* RXD0 */
    define_I(GPIOC,GPIO_Pin_5);                                                         /* RXD1 */
    define_I(GPIOB,GPIO_Pin_0);                                                         /* RXD2 */
    define_I(GPIOB,GPIO_Pin_1);                                                         /* RXD3 */
    define_I(GPIOB,GPIO_Pin_10);                                                        /* RXER */

    define_O(GPIOA,GPIO_Pin_0);                                                         /* PA0 */
    define_O(GPIOA,GPIO_Pin_3);                                                         /* PA3 */
}


uint32_t Ethernet_RegInit( ETH_InitTypeDef* ETH_InitStruct, uint16_t PHYAddress )
{
    uint32_t tmpreg = 0;

    /*---------------------- Physical layer configuration -------------------*/
    /* Set the SMI interface clock, set as the main frequency divided by 42  */
    tmpreg = ETH->MACMIIAR;
    tmpreg &= MACMIIAR_CR_MASK;
    tmpreg |= (uint32_t)ETH_MACMIIAR_CR_Div42;
    ETH->MACMIIAR = (uint32_t)tmpreg;

    /*------------------------ MAC register configuration  ----------------------- --------------------*/
    tmpreg = ETH->MACCR;
    tmpreg &= MACCR_CLEAR_MASK;
    tmpreg |= (uint32_t)(ETH_InitStruct->ETH_AutoNegotiation |
                  ETH_InitStruct->ETH_Watchdog |
                  ETH_InitStruct->ETH_Jabber |
                  ETH_InitStruct->ETH_InterFrameGap |
                  ETH_InitStruct->ETH_CarrierSense |
                  ETH_InitStruct->ETH_Speed |
                  ETH_InitStruct->ETH_ReceiveOwn |
                  ETH_InitStruct->ETH_LoopbackMode |
                  ETH_InitStruct->ETH_Mode |
                  ETH_InitStruct->ETH_ChecksumOffload |
                  ETH_InitStruct->ETH_RetryTransmission |
                  ETH_InitStruct->ETH_AutomaticPadCRCStrip |
                  ETH_InitStruct->ETH_BackOffLimit |
                  ETH_InitStruct->ETH_DeferralCheck);
    /* Write MAC Control Register */
    ETH->MACCR = (uint32_t)tmpreg;
    ETH->MACFFR = (uint32_t)(ETH_InitStruct->ETH_ReceiveAll |
                          ETH_InitStruct->ETH_SourceAddrFilter |
                          ETH_InitStruct->ETH_PassControlFrames |
                          ETH_InitStruct->ETH_BroadcastFramesReception |
                          ETH_InitStruct->ETH_DestinationAddrFilter |
                          ETH_InitStruct->ETH_PromiscuousMode |
                          ETH_InitStruct->ETH_MulticastFramesFilter |
                          ETH_InitStruct->ETH_UnicastFramesFilter);
    /*--------------- ETHERNET MACHTHR and MACHTLR Configuration ---------------*/
    /* Write to ETHERNET MACHTHR */
    ETH->MACHTHR = (uint32_t)ETH_InitStruct->ETH_HashTableHigh;
    /* Write to ETHERNET MACHTLR */
    ETH->MACHTLR = (uint32_t)ETH_InitStruct->ETH_HashTableLow;
    /*----------------------- ETHERNET MACFCR Configuration --------------------*/
    /* Get the ETHERNET MACFCR value */
    tmpreg = ETH->MACFCR;
    /* Clear xx bits */
    tmpreg &= MACFCR_CLEAR_MASK;
    tmpreg |= (uint32_t)((ETH_InitStruct->ETH_PauseTime << 16) |
                     ETH_InitStruct->ETH_ZeroQuantaPause |
                     ETH_InitStruct->ETH_PauseLowThreshold |
                     ETH_InitStruct->ETH_UnicastPauseFrameDetect |
                     ETH_InitStruct->ETH_ReceiveFlowControl |
                     ETH_InitStruct->ETH_TransmitFlowControl);
    ETH->MACFCR = (uint32_t)tmpreg;

    ETH->MACVLANTR = (uint32_t)(ETH_InitStruct->ETH_VLANTagComparison |
                               ETH_InitStruct->ETH_VLANTagIdentifier);

    tmpreg = ETH->DMAOMR;
    tmpreg &= DMAOMR_CLEAR_MASK;
    tmpreg |= (uint32_t)(ETH_InitStruct->ETH_DropTCPIPChecksumErrorFrame |
                    ETH_InitStruct->ETH_ReceiveStoreForward |
                    ETH_InitStruct->ETH_FlushReceivedFrame |
                    ETH_InitStruct->ETH_TransmitStoreForward |
                    ETH_InitStruct->ETH_TransmitThresholdControl |
                    ETH_InitStruct->ETH_ForwardErrorFrames |
                    ETH_InitStruct->ETH_ForwardUndersizedGoodFrames |
                    ETH_InitStruct->ETH_ReceiveThresholdControl |
                    ETH_InitStruct->ETH_SecondFrameOperate);
    ETH->DMAOMR = (uint32_t)tmpreg;

    /* Reset the physical layer */
    ETH_WritePHYRegister(PHYAddress, PHY_BCR, PHY_Reset);
    ETH_WritePHYRegister(gPHYAddress, PHY_MDIX, PHY_PN_SWITCH_AUTO);
    return ETH_SUCCESS;
}



void Ethernet_ETHIsr(void)
{
    uint32_t int_sta;

    int_sta = ETH->DMASR;
    if( int_sta & ETH_DMA_IT_NIS )
    {
        if( int_sta & ETH_DMA_IT_R )
        {
            if( ChipVerNum < CHIP_C_VER_NUM )
            {
                if ((int_sta & ETH_DMA_IT_RBU) != (u32)RESET)
                {
                    /* Clear RBUS ETHERNET DMA flag */
                    ETH->DMASR = ETH_DMA_IT_RBU;

                    ((ETH_DMADESCTypeDef *)(((ETH_DMADESCTypeDef *)(ETH->DMACHRDR))->Buffer2NextDescAddr))->Status = ETH_DMARxDesc_OWN;

                    /* Resume DMA reception */
                    ETH->DMARPDR = 0;
                }
            }
            ETH_DMAClearITPendingBit(ETH_DMA_IT_R);
            /* Check if the descriptor is owned by the ETHERNET DMA (when set) or CPU (when reset) */
            if((DMARxDescToGet->Status & ETH_DMARxDesc_OWN) != (u32)RESET)
            {
                /***/
            }
            else
            {
                /* Update the ETHERNET DMA global Rx descriptor with next Rx descriptor */
                /* Chained Mode */
                /* Selects the next DMA Rx descriptor list for next buffer to read */
                DMARxDescToGet = (ETH_DMADESCTypeDef*) (DMARxDescToGet->Buffer2NextDescAddr);
            }
        }
        if( int_sta & ETH_DMA_IT_T )
        {
            ETH_DMAClearITPendingBit(ETH_DMA_IT_T);
            if( (pDMATxSet->Status&ETH_DMATxDesc_ES) )
            {
                /***/
            }
        }
        if( int_sta & ETH_DMA_IT_PHYLINK)
        {
            Ethernet_PHYLink( );
            ETH_DMAClearITPendingBit(ETH_DMA_IT_PHYLINK);
        }
        ETH_DMAClearITPendingBit(ETH_DMA_IT_NIS);
    }
}



void ETH_PHYLink( void )
{
    u32 phy_stat;

    phy_stat = ETH_ReadPHYRegister( PHY_ADDRESS, PHY_BSR );
    LastPhyStat = phy_stat;
    // WCHNET_PhyStatus( phy_stat );
    if( (phy_stat&PHY_Linked_Status) && (phy_stat&PHY_AutoNego_Complete) )
    {
        phy_stat = ETH_ReadPHYRegister( PHY_ADDRESS, PHY_BCR );
        /* PHY negotiation result */
        if(phy_stat&(1<<13))                                    /* 100M */
        {
            ETH->MACCR &= ~(ETH_Speed_100M|ETH_Speed_1000M);
            ETH->MACCR |= ETH_Speed_100M;
        }
        else                                                    /* 10M */
        {
            ETH->MACCR &= ~(ETH_Speed_100M|ETH_Speed_1000M);
        }
        if(phy_stat&(1<<8))                                     /* full duplex */
        {
            ETH->MACCR |= ETH_Mode_FullDuplex;
        }
        else                                                    /* half duplex */
        {
            ETH->MACCR &= ~ETH_Mode_FullDuplex;
        }
        ETH_Start( );
    }
}

