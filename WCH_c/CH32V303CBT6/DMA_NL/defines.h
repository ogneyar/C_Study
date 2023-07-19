
#ifndef __DEFINES_H_
#define __DEFINES_H_

#define __I     volatile    // defines 'read only' permissions
#define __O     volatile    // defines 'write only' permissions
#define __IO    volatile    // defines 'read/write' permissions

typedef uint64_t  u64;
typedef uint32_t  u32;
typedef uint16_t  u16;
typedef uint8_t   u8;

// memory mapped structure for Program Fast Interrupt Controller (PFIC)
typedef struct{
    __I  uint32_t ISR[8];
    __I  uint32_t IPR[8];
    __IO uint32_t ITHRESDR;
    __IO uint32_t RESERVED;
    __IO uint32_t CFGR;
    __I  uint32_t GISR;
    __IO uint8_t VTFIDR[4];
    uint8_t RESERVED0[12];
    __IO uint32_t VTFADDR[4];
    uint8_t RESERVED1[0x90];
    __O  uint32_t IENR[8];
    uint8_t RESERVED2[0x60];
    __O  uint32_t IRER[8];
    uint8_t RESERVED3[0x60];
    __O  uint32_t IPSR[8];
    uint8_t RESERVED4[0x60];
    __O  uint32_t IPRR[8];
    uint8_t RESERVED5[0x60];
    __IO uint32_t IACTR[8];
    uint8_t RESERVED6[0xE0];
    __IO uint8_t IPRIOR[256];
    uint8_t RESERVED7[0x810];
    __IO uint32_t SCTLR;
}PFIC_Type;

// memory mapped structure for SysTick
typedef struct
{
    __IO u32 CTLR;  // 0x00
    __IO u32 SR;    // 0x04
    __IO u64 CNT;   // 0x08
    __IO u64 CMP;   // 0x0C
} SysTick_Type;

// Reset and Clock Control
typedef struct
{
    __IO uint32_t CTLR;       // 0x00
    __IO uint32_t CFGR0;      // 0x04
    __IO uint32_t INTR;       // 0x08
    __IO uint32_t APB2PRSTR;  // 0x0C
    __IO uint32_t APB1PRSTR;  // 0x10
    __IO uint32_t AHBPCENR;   // 0x14
    __IO uint32_t APB2PCENR;  // 0x18
    __IO uint32_t APB1PCENR;  // 0x1C
    __IO uint32_t BDCTLR;     // 0x20
    __IO uint32_t RSTSCKR;    // 0x24
    __IO uint32_t AHBRSTR;    // 0x28
    __IO uint32_t CFGR2;      // 0x2C
} RCC_TypeDef;

// General Purpose I/O
typedef struct
{
    __IO uint32_t CFGLR;  // 0x00
    __IO uint32_t CFGHR;  // 0x04
    __IO uint32_t INDR;   // 0x08
    __IO uint32_t OUTDR;  // 0x0C
    __IO uint32_t BSHR;   // 0x10
    __IO uint32_t BCR;    // 0x14
    __IO uint32_t LCKR;   // 0x18
} GPIO_TypeDef;

// Enhanced Registers
typedef struct
{
    __IO uint32_t EXTEN_CTR;  // 0x00
} EXTEN_TypeDef;

// Universal Synchronous Asynchronous Receiver Transmitter
typedef struct
{
    __IO uint16_t STATR;
    uint16_t  RESERVED0;
    __IO uint16_t DATAR;
    uint16_t  RESERVED1;
    __IO uint16_t BRR;
    uint16_t  RESERVED2;
    __IO uint16_t CTLR1;
    uint16_t  RESERVED3;
    __IO uint16_t CTLR2;
    uint16_t  RESERVED4;
    __IO uint16_t CTLR3;
    uint16_t  RESERVED5;
    __IO uint16_t GPR;
    uint16_t  RESERVED6;
} USART_TypeDef;

// Digital to Analog Converter
typedef struct
{
    __IO uint32_t CTLR;
    __IO uint32_t SWTR;
    __IO uint32_t R12BDHR1;
    __IO uint32_t L12BDHR1;
    __IO uint32_t R8BDHR1;
    __IO uint32_t R12BDHR2;
    __IO uint32_t L12BDHR2;
    __IO uint32_t R8BDHR2;
    __IO uint32_t RD12BDHR;
    __IO uint32_t LD12BDHR;
    __IO uint32_t RD8BDHR;
    __IO uint32_t DOR1;
    __IO uint32_t DOR2;
} DAC_TypeDef;

// Inter Integrated Circuit Interface
typedef struct
{
    __IO uint16_t CTLR1;
    uint16_t  RESERVED0;
    __IO uint16_t CTLR2;
    uint16_t  RESERVED1;
    __IO uint16_t OADDR1;
    uint16_t  RESERVED2;
    __IO uint16_t OADDR2;
    uint16_t  RESERVED3;
    __IO uint16_t DATAR;
    uint16_t  RESERVED4;
    __IO uint16_t STAR1;
    uint16_t  RESERVED5;
    __IO uint16_t STAR2;
    uint16_t  RESERVED6;
    __IO uint16_t CKCFGR;
    uint16_t  RESERVED7;
    __IO uint16_t RTR;
    uint16_t  RESERVED8;
} I2C_TypeDef;

// Serial Peripheral Interface
typedef struct
{
    __IO uint16_t CTLR1;
    uint16_t  RESERVED0;
    __IO uint16_t CTLR2;
    uint16_t  RESERVED1;
    __IO uint16_t STATR;
    uint16_t  RESERVED2;
    __IO uint16_t DATAR;
    uint16_t  RESERVED3;
    __IO uint16_t CRCR;
    uint16_t  RESERVED4;
    __IO uint16_t RCRCR;
    uint16_t  RESERVED5;
    __IO uint16_t TCRCR;
    uint16_t  RESERVED6;
    __IO uint16_t I2SCFGR;
    uint16_t  RESERVED7;
    __IO uint16_t I2SPR;
    uint16_t  RESERVED8;
    __IO uint16_t HSCR;
    uint16_t  RESERVED9;
} SPI_TypeDef;

// External Interrupt/Event Controller
typedef struct
{
    __IO uint32_t INTENR; 
    __IO uint32_t EVENR;   
    __IO uint32_t RTENR;   
    __IO uint32_t FTENR;   
    __IO uint32_t SWIEVR;  
    __IO uint32_t INTFR;   
} EXTI_TypeDef;

// Alternate Function I/O
typedef struct
{
    __IO uint32_t ECR;
    __IO uint32_t PCFR1;
    __IO uint32_t EXTICR[4];
    uint32_t RESERVED0;
    __IO uint32_t PCFR2;  
} AFIO_TypeDef;

// DMA Channel Controller
typedef struct
{
    __IO uint32_t CFGR;
    __IO uint32_t CNTR;
    __IO uint32_t PADDR;
    __IO uint32_t MADDR;
} DMA_Channel_TypeDef;

// DMA Controller
typedef struct
{
    __IO uint32_t INTFR;
    __IO uint32_t INTFCR;
} DMA_TypeDef;


#define SET         1
#define RESET       0

#define ENABLE      1
#define DISABLE     0

#define LED_R       3 // PB3
#define LED_G       4 // PB4
#define LED_B       5 // PB5

#define PA0         0 // 
#define PA1         1 // 
#define PA2         2 // USR_BTN
#define PA3         3 // 
#define PA4         4 // DAC
#define PA5         5 // SPI1 CLK
#define PA6         6 // SPI1 MISO
#define PA7         7 // SPI1 MOSI
#define PA8         8 // 
#define PA9         9 // Tx1
#define PA10       10 // 
#define PA11       11 // 
#define PA12       12 // 
#define PA13       13 // 
#define PA14       14 // 
#define PA15       15 // 

#define PB0         0 // 
#define PB1         1 // 
#define PB2         2 // 
#define PB3         3 // LED_R
#define PB4         4 // LED_G
#define PB5         5 // LED_B
#define PB6         6 // 
#define PB7         7 // 
#define PB8         8 // 
#define PB9         9 // 
#define PB10       10 // SCK I2C2
#define PB11       11 // SDA I2C2
#define PB12       12 // 
#define PB13       13 // 
#define PB14       14 // 
#define PB15       15 // 

#define GPIO_Msk        0b1111
#define GPIO_Speed_50   0b0011
#define GPIO_IN_FLOAT   0b0100
#define GPIO_AF         0b1000 // Alternate Functions
#define GPIO_AF_50      0b1011 // Alternate Functions with speed 50Hz
#define GPIO_AF_OD      0b1100 // Alternate Functions
#define GPIO_AF_OD_50   0b1111 // Alternate Functions with speed 50Hz
#define GPIO_IPU        0b1000 // Input Pull Up

// Peripheral memory map
#define FLASH_BASE             ((uint32_t)0x08000000)
#define SRAM_BASE              ((uint32_t)0x20000000)
#define PERIPH_BASE            ((uint32_t)0x40000000)

#define APB1PERIPH_BASE        (PERIPH_BASE)                // 0x40000000
#define APB2PERIPH_BASE        (PERIPH_BASE + 0x10000)      // 0x40010000
#define AHBPERIPH_BASE         (PERIPH_BASE + 0x20000)      // 0x40020000

#define I2C1_BASE              (APB1PERIPH_BASE + 0x5400)   // 0x40005400
#define I2C2_BASE              (APB1PERIPH_BASE + 0x5800)   // 0x40005800
#define DAC_BASE               (APB1PERIPH_BASE + 0x7400)   // 0x40007400

#define AFIO_BASE              (APB2PERIPH_BASE + 0x0000)   // 0x40010000
#define EXTI_BASE              (APB2PERIPH_BASE + 0x0400)   // 0x40010400
#define GPIOA_BASE             (APB2PERIPH_BASE + 0x0800)   // 0x40010800
#define GPIOB_BASE             (APB2PERIPH_BASE + 0x0C00)   // 0x40010C00
#define SPI1_BASE              (APB2PERIPH_BASE + 0x3000)   // 0x40013000
#define USART1_BASE            (APB2PERIPH_BASE + 0x3800)   // 0x40013800

#define DMA1_BASE              (AHBPERIPH_BASE + 0x0000)    // 0x40020000
#define DMA1_Channel1_BASE     (AHBPERIPH_BASE + 0x0008)    // 0x40020008
#define DMA1_Channel2_BASE     (AHBPERIPH_BASE + 0x001C)    // 0x4002001C
#define DMA1_Channel3_BASE     (AHBPERIPH_BASE + 0x0030)    // 0x40020030

#define DMA2_BASE              (AHBPERIPH_BASE + 0x0400)    // 0x40020400

#define DMA2_EXTEN_BASE        (AHBPERIPH_BASE + 0x04D0)    // 0x400204D0
#define RCC_BASE               (AHBPERIPH_BASE + 0x1000)    // 0x40021000
#define EXTEN_BASE             (AHBPERIPH_BASE + 0x3800)    // 0x40023800


#define PFIC                   ((PFIC_Type *) 0xE000E000 )
#define NVIC                   PFIC

#define SysTick                ((SysTick_Type *) 0xE000F000)

#define I2C2                   ((I2C_TypeDef *) I2C2_BASE)
#define DAC                    ((DAC_TypeDef *) DAC_BASE)

#define AFIO                   ((AFIO_TypeDef *) AFIO_BASE)
#define EXTI                   ((EXTI_TypeDef *) EXTI_BASE)
#define GPIOA                  ((GPIO_TypeDef *) GPIOA_BASE)
#define GPIOB                  ((GPIO_TypeDef *) GPIOB_BASE)
#define SPI1                   ((SPI_TypeDef *) SPI1_BASE)
#define USART1                 ((USART_TypeDef *) USART1_BASE)

#define DMA1                   ((DMA_TypeDef *) DMA1_BASE)
#define DMA2                   ((DMA_TypeDef *) DMA2_BASE)
#define DMA2_EXTEN             ((DMA_TypeDef *) DMA2_EXTEN_BASE)
#define DMA1_Channel3          ((DMA_Channel_TypeDef *) DMA1_Channel3_BASE)
#define RCC                    ((RCC_TypeDef *) RCC_BASE)
#define EXTEN                  ((EXTEN_TypeDef *) EXTEN_BASE)


#define RCC_AHBPCENR_DMA1EN    ((uint32_t)0x00000001)

#define RCC_APB1PCENR_I2C2EN   ((uint32_t)0x00400000) // (1 << 22)
#define RCC_APB1PCENR_DACEN    ((uint32_t)0x20000000) // (1 << 29)

#define RCC_APB2PCENR_AFIOEN   ((uint32_t)0x00000001) // (1 << 0)
#define RCC_APB2PCENR_IOPAEN   ((uint32_t)0x00000004) // (1 << 2)
#define RCC_APB2PCENR_IOPBEN   ((uint32_t)0x00000008) // (1 << 3)
#define RCC_APB2PCENR_SPI1EN   ((uint32_t)0x00001000) // (1 << 12)
#define RCC_APB2PCENR_USART1EN ((uint32_t)0x00004000) // (1 << 14)

#define EXTEN_PLL_HSI_PRE      ((uint32_t)0x00000010) // (1 << 4)

#define RCC_HPRE               ((uint32_t)0x000000F0) // HPRE[3:0] bits (AHB prescaler)
#define RCC_HPRE_DIV1          ((uint32_t)0x00000000) // SYSCLK not divided
#define RCC_PPRE2_DIV1         ((uint32_t)0x00000000) // HCLK not divided
#define RCC_PPRE1_DIV2         ((uint32_t)0x00000400) // HCLK divided by 2
#define RCC_PLLSRC             ((uint32_t)0x00010000) // PLL entry clock source
#define RCC_PLLXTPRE           ((uint32_t)0x00020000) // HSE divider for PLL entry
#define RCC_PLLMULL            ((uint32_t)0x003C0000) // PLLMUL[3:0] bits (PLL multiplication factor)
#define RCC_PLLMULL6           ((uint32_t)0x00100000) // PLL input clock*6
#define RCC_PLLMULL18          ((uint32_t)0x003C0000) // PLL input clock*18
#define RCC_PLLSRC_HSI_Div2    ((uint32_t)0x00000000) // HSI clock divided by 2 selected as PLL entry clock source
#define RCC_PLLON              ((uint32_t)0x01000000) // PLL enable
#define RCC_PLLRDY             ((uint32_t)0x02000000) // PLL clock ready flag
#define RCC_SW                 ((uint32_t)0x00000003) // SW[1:0] bits (System clock Switch)
#define RCC_SW_PLL             ((uint32_t)0x00000002) // PLL selected as system clock
#define RCC_SWS                ((uint32_t)0x0000000C) // SWS[1:0] bits (System Clock Switch Status)


#define HSE_VALUE              ((uint32_t)8000000) // внешний осцилятор
#define HSI_VALUE              ((uint32_t)8000000) // внутренний осцилятор

#define SYSCLK_FREQ_48MHz_HSI  48000000
#define SYSCLK_FREQ_144MHz_HSI 144000000


           
/*

USART

*/

#define CTLR1_UE_Set              ((uint16_t)0x2000) // USART Enable Mask
#define CTLR1_UE_Reset            ((uint16_t)0xDFFF) // USART Disable Mask

#define CTLR2_STOP_CLEAR_Mask     ((uint16_t)0xCFFF) // USART CR2 STOP Bits Mask
#define CTLR1_CLEAR_Mask          ((uint16_t)0xE9F3) // USART CR1 Mask
#define CTLR3_CLEAR_Mask          ((uint16_t)0xFCFF) // USART CR3 Mask

   
// USART_Word_Length
#define USART_WordLength_8b       ((uint16_t)0x0000)
#define USART_WordLength_9b       ((uint16_t)0x1000)

// USART_Stop_Bits  
#define USART_StopBits_1          ((uint16_t)0x0000)
#define USART_StopBits_0_5        ((uint16_t)0x1000)
#define USART_StopBits_2          ((uint16_t)0x2000)
#define USART_StopBits_1_5        ((uint16_t)0x3000)

// USART_Parity  
#define USART_Parity_No           ((uint16_t)0x0000)
#define USART_Parity_Even         ((uint16_t)0x0400)
#define USART_Parity_Odd          ((uint16_t)0x0600) 

// USART_Mode 
#define USART_Mode_Rx             ((uint16_t)0x0004)
#define USART_Mode_Tx             ((uint16_t)0x0008)

/* USART_Hardware_Flow_Control */
#define USART_HardwareFlowControl_None       ((uint16_t)0x0000)
#define USART_HardwareFlowControl_RTS        ((uint16_t)0x0100)
#define USART_HardwareFlowControl_CTS        ((uint16_t)0x0200)
#define USART_HardwareFlowControl_RTS_CTS    ((uint16_t)0x0300)

// USART_Flags
#define USART_FLAG_CTS            ((uint16_t)0x0200)
#define USART_FLAG_LBD            ((uint16_t)0x0100)
#define USART_FLAG_TXE            ((uint16_t)0x0080)
#define USART_FLAG_TC             ((uint16_t)0x0040)
#define USART_FLAG_RXNE           ((uint16_t)0x0020)
#define USART_FLAG_IDLE           ((uint16_t)0x0010)
#define USART_FLAG_ORE            ((uint16_t)0x0008)
#define USART_FLAG_NE             ((uint16_t)0x0004)
#define USART_FLAG_FE             ((uint16_t)0x0002)
#define USART_FLAG_PE             ((uint16_t)0x0001)


/*

DAC

*/

#define DAC_EN1            ((uint32_t)0x00000001)        /* DAC channel1 enable */

#define CTLR_CLEAR_MASK    ((uint32_t)0x00000FFE)

// DAC_trigger_selection
#define DAC_Trigger_None        ((uint32_t)0x00000000) /* Conversion is automatic once the DAC1_DHRxxxx register has been loaded, and not by external trigger */
#define DAC_Trigger_T6_TRGO     ((uint32_t)0x00000004) /* TIM6 TRGO selected as external conversion trigger for DAC channel */
#define DAC_Trigger_T8_TRGO     ((uint32_t)0x0000000C) /* TIM8 TRGO selected as external conversion trigger for DAC channel only in High-density devices*/
#define DAC_Trigger_T7_TRGO     ((uint32_t)0x00000014) /* TIM7 TRGO selected as external conversion trigger for DAC channel */
#define DAC_Trigger_T5_TRGO     ((uint32_t)0x0000001C) /* TIM5 TRGO selected as external conversion trigger for DAC channel */
#define DAC_Trigger_T2_TRGO     ((uint32_t)0x00000024) /* TIM2 TRGO selected as external conversion trigger for DAC channel */
#define DAC_Trigger_T4_TRGO     ((uint32_t)0x0000002C) /* TIM4 TRGO selected as external conversion trigger for DAC channel */
#define DAC_Trigger_Ext_IT9     ((uint32_t)0x00000034) /* EXTI Line9 event selected as external conversion trigger for DAC channel */
#define DAC_Trigger_Software    ((uint32_t)0x0000003C) /* Conversion started by software trigger for DAC channel */

// DAC_wave_generation
#define DAC_WaveGeneration_None        ((uint32_t)0x00000000)
#define DAC_WaveGeneration_Noise       ((uint32_t)0x00000040)
#define DAC_WaveGeneration_Triangle    ((uint32_t)0x00000080)

// DAC_lfsrunmask_triangleamplitude
#define DAC_LFSRUnmask_Bit0            ((uint32_t)0x00000000) /* Unmask DAC channel LFSR bit0 for noise wave generation */
#define DAC_LFSRUnmask_Bits1_0         ((uint32_t)0x00000100) /* Unmask DAC channel LFSR bit[1:0] for noise wave generation */
#define DAC_LFSRUnmask_Bits2_0         ((uint32_t)0x00000200) /* Unmask DAC channel LFSR bit[2:0] for noise wave generation */
#define DAC_LFSRUnmask_Bits3_0         ((uint32_t)0x00000300) /* Unmask DAC channel LFSR bit[3:0] for noise wave generation */
#define DAC_LFSRUnmask_Bits4_0         ((uint32_t)0x00000400) /* Unmask DAC channel LFSR bit[4:0] for noise wave generation */
#define DAC_LFSRUnmask_Bits5_0         ((uint32_t)0x00000500) /* Unmask DAC channel LFSR bit[5:0] for noise wave generation */
#define DAC_LFSRUnmask_Bits6_0         ((uint32_t)0x00000600) /* Unmask DAC channel LFSR bit[6:0] for noise wave generation */
#define DAC_LFSRUnmask_Bits7_0         ((uint32_t)0x00000700) /* Unmask DAC channel LFSR bit[7:0] for noise wave generation */
#define DAC_LFSRUnmask_Bits8_0         ((uint32_t)0x00000800) /* Unmask DAC channel LFSR bit[8:0] for noise wave generation */
#define DAC_LFSRUnmask_Bits9_0         ((uint32_t)0x00000900) /* Unmask DAC channel LFSR bit[9:0] for noise wave generation */
#define DAC_LFSRUnmask_Bits10_0        ((uint32_t)0x00000A00) /* Unmask DAC channel LFSR bit[10:0] for noise wave generation */
#define DAC_LFSRUnmask_Bits11_0        ((uint32_t)0x00000B00) /* Unmask DAC channel LFSR bit[11:0] for noise wave generation */
#define DAC_TriangleAmplitude_1        ((uint32_t)0x00000000) /* Select max triangle amplitude of 1 */
#define DAC_TriangleAmplitude_3        ((uint32_t)0x00000100) /* Select max triangle amplitude of 3 */
#define DAC_TriangleAmplitude_7        ((uint32_t)0x00000200) /* Select max triangle amplitude of 7 */
#define DAC_TriangleAmplitude_15       ((uint32_t)0x00000300) /* Select max triangle amplitude of 15 */
#define DAC_TriangleAmplitude_31       ((uint32_t)0x00000400) /* Select max triangle amplitude of 31 */
#define DAC_TriangleAmplitude_63       ((uint32_t)0x00000500) /* Select max triangle amplitude of 63 */
#define DAC_TriangleAmplitude_127      ((uint32_t)0x00000600) /* Select max triangle amplitude of 127 */
#define DAC_TriangleAmplitude_255      ((uint32_t)0x00000700) /* Select max triangle amplitude of 255 */
#define DAC_TriangleAmplitude_511      ((uint32_t)0x00000800) /* Select max triangle amplitude of 511 */
#define DAC_TriangleAmplitude_1023     ((uint32_t)0x00000900) /* Select max triangle amplitude of 1023 */
#define DAC_TriangleAmplitude_2047     ((uint32_t)0x00000A00) /* Select max triangle amplitude of 2047 */
#define DAC_TriangleAmplitude_4095     ((uint32_t)0x00000B00) /* Select max triangle amplitude of 4095 */

// DAC_output_buffer
#define DAC_OutputBuffer_Enable        ((uint32_t)0x00000000)
#define DAC_OutputBuffer_Disable       ((uint32_t)0x00000002)

// DAC_Channel_selection
#define DAC_Channel_1           ((uint32_t)0x00000000)
#define DAC_Channel_2           ((uint32_t)0x00000010)

// DAC_data_alignment
#define DAC_Align_12b_R         ((uint32_t)0x00000000)
#define DAC_Align_12b_L         ((uint32_t)0x00000004)
#define DAC_Align_8b_R          ((uint32_t)0x00000008)

// DHR registers offsets
#define DHR12R1_OFFSET     ((uint32_t)0x00000008)
#define DHR12R2_OFFSET     ((uint32_t)0x00000014)
#define DHR12RD_OFFSET     ((uint32_t)0x00000020)



/*

DMA

*/


// DMA_data_transfer_direction
#define DMA_DIR_PeripheralDST              ((uint32_t)0x00000010)
#define DMA_DIR_PeripheralSRC              ((uint32_t)0x00000000)

// DMA_peripheral_incremented_mode
#define DMA_PeripheralInc_Enable           ((uint32_t)0x00000040)
#define DMA_PeripheralInc_Disable          ((uint32_t)0x00000000)
											
// DMA_memory_incremented_mode
#define DMA_MemoryInc_Enable               ((uint32_t)0x00000080)
#define DMA_MemoryInc_Disable              ((uint32_t)0x00000000)
										
// DMA_peripheral_data_size
#define DMA_PeripheralDataSize_Byte        ((uint32_t)0x00000000)
#define DMA_PeripheralDataSize_HalfWord    ((uint32_t)0x00000100)
#define DMA_PeripheralDataSize_Word        ((uint32_t)0x00000200)

// DMA_memory_data_size
#define DMA_MemoryDataSize_Byte            ((uint32_t)0x00000000)
#define DMA_MemoryDataSize_HalfWord        ((uint32_t)0x00000400)
#define DMA_MemoryDataSize_Word            ((uint32_t)0x00000800)

// DMA_circular_normal_mode
#define DMA_Mode_Circular                  ((uint32_t)0x00000020)
#define DMA_Mode_Normal                    ((uint32_t)0x00000000)

// DMA_priority_level
#define DMA_Priority_VeryHigh              ((uint32_t)0x00003000)
#define DMA_Priority_High                  ((uint32_t)0x00002000)
#define DMA_Priority_Medium                ((uint32_t)0x00001000)
#define DMA_Priority_Low                   ((uint32_t)0x00000000)

// DMA_memory_to_memory
#define DMA_M2M_Enable                     ((uint32_t)0x00004000)
#define DMA_M2M_Disable                    ((uint32_t)0x00000000)

// DMA2 FLAG mask
#define FLAG_Mask                 ((uint32_t)0x10000000)
#define DMA2_EXTEN_FLAG_Mask      ((uint32_t)0x20000000)

// DMA registers Masks
#define CFGR_CLEAR_Mask           ((uint32_t)0xFFFF800F)



// DMA_flags_definition
#define DMA1_FLAG_TC3                      ((uint32_t)0x00000200)


// Channel enable
#define  DMA_CFGR1_EN                      ((uint16_t)0x0001)            


/*

EXTI

*/

// EXTI mode enumeration
typedef enum
{
    EXTI_Mode_Interrupt = 0x00,
    EXTI_Mode_Event = 0x04
}EXTIMode_TypeDef;

// EXTI Trigger enumeration
typedef enum
{
    EXTI_Trigger_Rising = 0x08,
    EXTI_Trigger_Falling = 0x0C,  
    EXTI_Trigger_Rising_Falling = 0x10
}EXTITrigger_TypeDef;

// EXTI_Lines 
#define EXTI_Line0       ((uint32_t)0x00001)  /* External interrupt line 0 */
#define EXTI_Line1       ((uint32_t)0x00002)  /* External interrupt line 1 */
#define EXTI_Line2       ((uint32_t)0x00004)  /* External interrupt line 2 */
#define EXTI_Line3       ((uint32_t)0x00008)  /* External interrupt line 3 */
#define EXTI_Line4       ((uint32_t)0x00010)  /* External interrupt line 4 */
#define EXTI_Line5       ((uint32_t)0x00020)  /* External interrupt line 5 */
#define EXTI_Line6       ((uint32_t)0x00040)  /* External interrupt line 6 */
#define EXTI_Line7       ((uint32_t)0x00080)  /* External interrupt line 7 */
#define EXTI_Line8       ((uint32_t)0x00100)  /* External interrupt line 8 */
#define EXTI_Line9       ((uint32_t)0x00200)  /* External interrupt line 9 */
#define EXTI_Line10      ((uint32_t)0x00400)  /* External interrupt line 10 */
#define EXTI_Line11      ((uint32_t)0x00800)  /* External interrupt line 11 */
#define EXTI_Line12      ((uint32_t)0x01000)  /* External interrupt line 12 */
#define EXTI_Line13      ((uint32_t)0x02000)  /* External interrupt line 13 */
#define EXTI_Line14      ((uint32_t)0x04000)  /* External interrupt line 14 */
#define EXTI_Line15      ((uint32_t)0x08000)  /* External interrupt line 15 */
#define EXTI_Line16      ((uint32_t)0x10000)  /* External interrupt line 16 Connected to the PVD Output */
#define EXTI_Line17      ((uint32_t)0x20000)  /* External interrupt line 17 Connected to the RTC Alarm event */
#define EXTI_Line18      ((uint32_t)0x40000)  /* External interrupt line 18 Connected to the USBD/USBFS OTG Wakeup from suspend event */                                    
#define EXTI_Line19      ((uint32_t)0x80000)  /* External interrupt line 19 Connected to the Ethernet Wakeup event */
#define EXTI_Line20      ((uint32_t)0x100000) /* External interrupt line 20 Connected to the USBHS Wakeup event */






/*

GPIO

*/

// GPIO_Port_Sources
#define GPIO_PortSourceGPIOA        ((uint8_t)0x00)
#define GPIO_PortSourceGPIOB        ((uint8_t)0x01)
#define GPIO_PortSourceGPIOC        ((uint8_t)0x02)
#define GPIO_PortSourceGPIOD        ((uint8_t)0x03)
#define GPIO_PortSourceGPIOE        ((uint8_t)0x04)
#define GPIO_PortSourceGPIOF        ((uint8_t)0x05)
#define GPIO_PortSourceGPIOG        ((uint8_t)0x06)

// GPIO_Pin_sources
#define GPIO_PinSource0             ((uint8_t)0x00)
#define GPIO_PinSource1             ((uint8_t)0x01)
#define GPIO_PinSource2             ((uint8_t)0x02)
#define GPIO_PinSource3             ((uint8_t)0x03)
#define GPIO_PinSource4             ((uint8_t)0x04)
#define GPIO_PinSource5             ((uint8_t)0x05)
#define GPIO_PinSource6             ((uint8_t)0x06)
#define GPIO_PinSource7             ((uint8_t)0x07)
#define GPIO_PinSource8             ((uint8_t)0x08)
#define GPIO_PinSource9             ((uint8_t)0x09)
#define GPIO_PinSource10            ((uint8_t)0x0A)
#define GPIO_PinSource11            ((uint8_t)0x0B)
#define GPIO_PinSource12            ((uint8_t)0x0C)
#define GPIO_PinSource13            ((uint8_t)0x0D)
#define GPIO_PinSource14            ((uint8_t)0x0E)
#define GPIO_PinSource15            ((uint8_t)0x0F)




/*

I2C

*/

#define READY       1
#define NoREADY     0

// I2C acknowledgement
#define I2C_Ack_Enable           ((uint16_t)0x0400)
#define I2C_Ack_Disable          ((uint16_t)0x0000)

// I2C acknowledged address
#define I2C_AcknowledgedAddress_7bit    ((uint16_t)0x4000)
#define I2C_AcknowledgedAddress_10bit   ((uint16_t)0xC000)

// I2C transfer direction
#define I2C_Direction_Transmitter       ((uint8_t)0x00)
#define I2C_Direction_Receiver          ((uint8_t)0x01)

// I2C mode
#define I2C_Mode_I2C             ((uint16_t)0x0000)
#define I2C_Mode_SMBusDevice     ((uint16_t)0x0002)  
#define I2C_Mode_SMBusHost       ((uint16_t)0x000A)

// I2C flags
#define I2C_FLAG_BUSY            ((uint32_t)0x00020000)

// I2C events
#define I2C_EVENT_MASTER_MODE_SELECT                ((uint32_t)0x00030001)  // BUSY, MSL and SB flag
#define I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED  ((uint32_t)0x00070082)  // BUSY, MSL, ADDR, TXE and TRA flags
#define I2C_EVENT_MASTER_BYTE_TRANSMITTED           ((uint32_t)0x00070084)  // TRA, BUSY, MSL, TXE and BTF flags

// I2C SPE mask
#define CTLR1_PE_Set             ((uint16_t)0x0001)
#define CTLR1_PE_Reset           ((uint16_t)0xFFFE)

// I2C START mask
#define CTLR1_START_Set          ((uint16_t)0x0100)
#define CTLR1_START_Reset        ((uint16_t)0xFEFF)

// I2C STOP mask
#define CTLR1_STOP_Set           ((uint16_t)0x0200)
#define CTLR1_STOP_Reset         ((uint16_t)0xFDFF)

// I2C ACK mask
#define CTLR1_ACK_Set            ((uint16_t)0x0400)
#define CTLR1_ACK_Reset          ((uint16_t)0xFBFF)

// I2C FREQ mask
#define CTLR2_FREQ_Reset         ((uint16_t)0xFFC0)

// I2C F/S mask
#define CKCFGR_FS_Set            ((uint16_t)0x8000)

// I2C CCR mask
#define CKCFGR_CCR_Set           ((uint16_t)0x0FFF)

// I2C ADD0 mask
#define OADDR1_ADD0_Set          ((uint16_t)0x0001)
#define OADDR1_ADD0_Reset        ((uint16_t)0xFFFE)

// I2C FLAG mask
#define FLAG_Mask                ((uint32_t)0x00FFFFFF)



/*

NVIC

*/

// Preemption_Priority_Group
#define NVIC_PriorityGroup_0           ((uint32_t)0x00)
#define NVIC_PriorityGroup_1           ((uint32_t)0x01)
#define NVIC_PriorityGroup_2           ((uint32_t)0x02)
#define NVIC_PriorityGroup_3           ((uint32_t)0x03)
#define NVIC_PriorityGroup_4           ((uint32_t)0x04)





/*

SPI

*/

// SPI or I2S mode selection masks
#define SPI_Mode_Select       ((uint16_t)0xF7FF)
#define I2S_Mode_Select       ((uint16_t)0x0800)

// SPI SPE mask
#define CTLR1_SPE_Set         ((uint16_t)0x0040)
#define CTLR1_SPE_Reset       ((uint16_t)0xFFBF)

// SPI_data_direction  
#define SPI_Direction_2Lines_FullDuplex ((uint16_t)0x0000)
#define SPI_Direction_2Lines_RxOnly     ((uint16_t)0x0400)
#define SPI_Direction_1Line_Rx          ((uint16_t)0x8000)
#define SPI_Direction_1Line_Tx          ((uint16_t)0xC000)

// SPI_mode
#define SPI_Mode_Master                 ((uint16_t)0x0104)
#define SPI_Mode_Slave                  ((uint16_t)0x0000)

// SPI_data_size
#define SPI_DataSize_16b                ((uint16_t)0x0800)
#define SPI_DataSize_8b                 ((uint16_t)0x0000)

// SPI_Clock_Polarity
#define SPI_CPOL_Low                    ((uint16_t)0x0000)
#define SPI_CPOL_High                   ((uint16_t)0x0002)

// SPI_Clock_Phase
#define SPI_CPHA_1Edge                  ((uint16_t)0x0000)
#define SPI_CPHA_2Edge                  ((uint16_t)0x0001)

// SPI_Slave_Select_management
#define SPI_NSS_Soft                    ((uint16_t)0x0200)
#define SPI_NSS_Hard                    ((uint16_t)0x0000)

// SPI_BaudRate_Prescaler
#define SPI_BaudRatePrescaler_2         ((uint16_t)0x0000)
#define SPI_BaudRatePrescaler_4         ((uint16_t)0x0008)
#define SPI_BaudRatePrescaler_8         ((uint16_t)0x0010)
#define SPI_BaudRatePrescaler_16        ((uint16_t)0x0018)
#define SPI_BaudRatePrescaler_32        ((uint16_t)0x0020)
#define SPI_BaudRatePrescaler_64        ((uint16_t)0x0028)
#define SPI_BaudRatePrescaler_128       ((uint16_t)0x0030)
#define SPI_BaudRatePrescaler_256       ((uint16_t)0x0038)

// SPI_MSB_LSB_transmission
#define SPI_FirstBit_MSB                ((uint16_t)0x0000)
#define SPI_FirstBit_LSB                ((uint16_t)0x0080)

// SPI_I2S_flags_definition
#define SPI_I2S_FLAG_RXNE               ((uint16_t)0x0001)
#define SPI_I2S_FLAG_TXE                ((uint16_t)0x0002)
#define I2S_FLAG_CHSIDE                 ((uint16_t)0x0004)
#define I2S_FLAG_UDR                    ((uint16_t)0x0008)
#define SPI_FLAG_CRCERR                 ((uint16_t)0x0010)
#define SPI_FLAG_MODF                   ((uint16_t)0x0020)
#define SPI_I2S_FLAG_OVR                ((uint16_t)0x0040)
#define SPI_I2S_FLAG_BSY                ((uint16_t)0x0080)


#endif /* __DEFINES_H_ */
