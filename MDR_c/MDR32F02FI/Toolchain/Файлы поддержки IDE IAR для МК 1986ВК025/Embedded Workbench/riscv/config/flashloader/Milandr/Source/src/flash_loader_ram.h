//#define FLASH_REG_BASE	0x40018000
#define FLASH_CMD	(FLASH_REG_BASE + 0x00)
#define FLASH_ADR	(FLASH_REG_BASE + 0x04)
#define FLASH_DI	(FLASH_REG_BASE + 0x08)
#define FLASH_DO	(FLASH_REG_BASE + 0x0C)
#define FLASH_KEY	(FLASH_REG_BASE + 0x10)

#define FLASH_TMR	(1 << 14)
#define FLASH_NVSTR	(1 << 13)
#define FLASH_PROG	(1 << 12)
#define FLASH_MAS1	(1 << 11)
#define FLASH_ERASE	(1 << 10)
#define FLASH_IFREN	(1 << 9)
#define FLASH_SE	(1 << 8)
#define FLASH_YE	(1 << 7)
#define FLASH_XE	(1 << 6)
#define FLASH_RD	(1 << 2)
#define FLASH_WR	(1 << 1)
#define FLASH_CON	(1 << 0)
#define FLASH_DELAY_MASK	(7 << 3)

//#define KEY		0x8AAA5551

#define __IOM volatile
#define __IM  volatile
#define __IO  volatile
#define uint32_t unsigned int
#define  int32_t   signed int

#define NOP __no_operation()

/**
  * @brief EEPROM_CNTR description (EEPROM_CNTR)
  */

typedef struct {                                /*!< (@ 0x40018000) EEPROM_CNTR Structure                                      */

  union {
    __IOM uint32_t CMD;                         /*!< (@ 0x00000000) Registr upravlenija                                        */

    struct {
      __IOM uint32_t CON        : 1;            /*!< [0..0] Perekljuchenie kontrollera pamjati EEPROM na registrovoe
                                                     upravlenie                                                                */
      __IM  uint32_t            : 2;
      __IOM uint32_t Delay      : 3;            /*!< [5..3] Zaderzhka pamjati programm pri chtenii v ciklah (v rabochem
                                                     rezhime)                                                                  */
      __IOM uint32_t XE         : 1;            /*!< [6..6] Bit razreshenija dostupa k bankam pamjati                          */
      __IOM uint32_t YE         : 1;            /*!< [7..7] Bit razreshenija starshej chasti adresa pri dostupe v
                                                     pamjat' so storony registrov.                                             */
      __IOM uint32_t SE         : 1;            /*!< [8..8] Bit razreshenija mladshej chasti adresa pri dostupe v
                                                     pamjat' so storony registrov.                                             */
      __IOM uint32_t IFREN      : 1;            /*!< [9..9] Bit vybora tipa pamjati pri dostupe v pamjat'                      */
      __IOM uint32_t ERASE      : 1;            /*!< [10..10] Bit stiranija pamjati pri dostupe v pamjat'                      */
      __IOM uint32_t MAS1       : 1;            /*!< [11..11] Bit stiranija pamjati pri dostupe v pamjat'                      */
      __IOM uint32_t PROG       : 1;            /*!< [12..12] Bit programirovanija pamjati pri dostupe v pamjat'               */
      __IOM uint32_t NVSTR      : 1;            /*!< [13..13] Operacii zapisi ili stiranija                                    */
      __IOM uint32_t TMR        : 1;            /*!< [14..14] Bit perevoda pamjat' v testovyj rezhim                           */
    } CMD_b;
  } ;

  union {
    __IOM uint32_t ADR;                         /*!< (@ 0x00000004) ADR description                                            */

    struct {
      __IM  uint32_t            : 2;
      __IOM uint32_t YADR       : 7;            /*!< [8..2] YADR[6:0] = col[6:0]                                               */
      __IOM uint32_t XADR       : 9;            /*!< [17..9] XADR[8:0] = page[5:0], row[2:0]                                   */
    } ADR_b;
  } ;
  __IOM uint32_t  DI;                           /*!< (@ 0x00000008) Dannye dlja zapisi v EERPOM                                */
  __IOM uint32_t  DO;                           /*!< (@ 0x0000000C) Dannye, schitannye iz EERPOM                               */
  __IOM uint32_t  KEY;                          /*!< (@ 0x00000010) 0x8AAA5551                                                 */
} EEPROM_CNTR_Type;                             /*!< Size = 20 (0x14)                                                          */
#define EEPROM_CNTR_BASE            0x40018000UL
#define EEPROM_CNTR                 ((EEPROM_CNTR_Type*)       EEPROM_CNTR_BASE)

typedef struct
{
    __IO uint32_t RXTX;                 /*!< input/output data */
    __IO uint32_t OE;                   /*!< port direction */
    __IO uint32_t FUNC;                 /*!< port function */
    __IO uint32_t ANALOG;               /*!< analog port mode */
    __IO uint32_t PULL;                 /*!< port pull register control */
    __IO uint32_t RFU;                  /*!< RFU */
    __IO uint32_t PWR;                  /*!< port power control */
    __IO uint32_t RFU2;                 /*!< RFU2 */
    __IO uint32_t SETTX;                /*!< bit-setup register for output ports */
    __IO uint32_t CLRTX;                /*!< bit-clear register for output ports */
    __IO uint32_t RDTX;                 /*!< read-back register for output ports */
} MDR_GPIO_TypeDef;
#define MDR_GPIO2_BASE                  0x40088000U
#define MDR_PORTB                       ((MDR_GPIO_TypeDef *) MDR_GPIO2_BASE)
typedef struct
{
    __IO uint32_t CLOCK_STATUS;         /*!< Frequency status register */
    __IO uint32_t PLL_CONTROL;          /*!< PLL control register */
    __IO uint32_t HS_CONTROL;           /*!< Hi speed oscillator control register */
    __IO uint32_t CPU_CLOCK;            /*!< CPU speed control register */
    __IO uint32_t PER1_CLOCK;           /*!< Peripherals(1) speed control register */
    __IO uint32_t ADC_CLOCK;            /*!< Analog-to-digital control register */
    __IO uint32_t RTC_CLOCK;            /*!< Real-time clock speed register */
    __IO uint32_t PER2_CLOCK;           /*!< Peripherals(2) speed control register */
    __IO uint32_t RFU;                  /*!< ??? */
    __IO uint32_t TIM_CLOCK;            /*!< Timers clock control register */
    __IO uint32_t UART_CLOCK;           /*!< UARTs clock control register */
    __IO uint32_t SSP_CLOCK;            /*!< SSPs clock control register */
} MDR_RCC_TypeDef;
#define MDR_RCC_BASE                    0x40020000U
#define MDR_RCC                         ((MDR_RCC_TypeDef *) MDR_RCC_BASE)
#define RCC_PER2_CLOCK_PORTBEN_Pos      (17U)
#define RCC_PER2_CLOCK_PORTBEN_Msk      (0x1U << RCC_PER2_CLOCK_PORTBEN_Pos)    /*!< 0x00020000 */
#define RCC_PER2_CLOCK_PORTBEN          RCC_PER2_CLOCK_PORTBEN_Msk
#define RCC_PER2_CLOCK_BKPEN_Pos        (12U)
#define RCC_PER2_CLOCK_BKPEN_Msk        (0x1U << RCC_PER2_CLOCK_BKPEN_Pos)      /*!< 0x00001000 */
#define RCC_PER2_CLOCK_BKPEN            RCC_PER2_CLOCK_BKPEN_Msk
#define RCC_PER2_CLOCK_EEPROMEN_Pos     (3U)
#define RCC_PER2_CLOCK_EEPROMEN_Msk     (0x1U << RCC_PER2_CLOCK_EEPROMEN_Pos)   /*!< 0x00000008 */
#define RCC_PER2_CLOCK_EEPROMEN         RCC_PER2_CLOCK_EEPROMEN_Msk


