
#ifndef __SD_CARD_H_
#define __SD_CARD_H_

#include "defines.h"
#include "system.h"
#include "spi.h"




/** init timeout ms */
unsigned int const SD_INIT_TIMEOUT = 2000;
/** erase timeout ms */
unsigned int const SD_ERASE_TIMEOUT = 10000;
/** read timeout ms */
unsigned int const SD_READ_TIMEOUT = 300;
/** write time out ms */
unsigned int const SD_WRITE_TIMEOUT = 600;
//------------------------------------------------------------------------------
// SD card errors
/** timeout error for command CMD0 */
uint8_t const SD_CARD_ERROR_CMD0 = 0X1;
/** CMD8 was not accepted - not a valid SD card*/
uint8_t const SD_CARD_ERROR_CMD8 = 0X2;
/** card returned an error response for CMD17 (read block) */
uint8_t const SD_CARD_ERROR_CMD17 = 0X3;
/** card returned an error response for CMD24 (write block) */
uint8_t const SD_CARD_ERROR_CMD24 = 0X4;
/**  WRITE_MULTIPLE_BLOCKS command failed */
uint8_t const SD_CARD_ERROR_CMD25 = 0X05;
/** card returned an error response for CMD58 (read OCR) */
uint8_t const SD_CARD_ERROR_CMD58 = 0X06;
/** SET_WR_BLK_ERASE_COUNT failed */
uint8_t const SD_CARD_ERROR_ACMD23 = 0X07;
/** card's ACMD41 initialization process timeout */
uint8_t const SD_CARD_ERROR_ACMD41 = 0X08;
/** card returned a bad CSR version field */
uint8_t const SD_CARD_ERROR_BAD_CSD = 0X09;
/** erase block group command failed */
uint8_t const SD_CARD_ERROR_ERASE = 0X0A;
/** card not capable of single block erase */
uint8_t const SD_CARD_ERROR_ERASE_SINGLE_BLOCK = 0X0B;
/** Erase sequence timed out */
uint8_t const SD_CARD_ERROR_ERASE_TIMEOUT = 0X0C;
/** card returned an error token instead of read data */
uint8_t const SD_CARD_ERROR_READ = 0X0D;
/** read CID or CSD failed */
uint8_t const SD_CARD_ERROR_READ_REG = 0X0E;
/** timeout while waiting for start of read data */
uint8_t const SD_CARD_ERROR_READ_TIMEOUT = 0X0F;
/** card did not accept STOP_TRAN_TOKEN */
uint8_t const SD_CARD_ERROR_STOP_TRAN = 0X10;
/** card returned an error token as a response to a write operation */
uint8_t const SD_CARD_ERROR_WRITE = 0X11;
/** attempt to write protected block zero */
uint8_t const SD_CARD_ERROR_WRITE_BLOCK_ZERO = 0X12;
/** card did not go ready for a multiple block write */
uint8_t const SD_CARD_ERROR_WRITE_MULTIPLE = 0X13;
/** card returned an error to a CMD13 status check after a write */
uint8_t const SD_CARD_ERROR_WRITE_PROGRAMMING = 0X14;
/** timeout occurred during write programming */
uint8_t const SD_CARD_ERROR_WRITE_TIMEOUT = 0X15;
/** incorrect rate selected */
uint8_t const SD_CARD_ERROR_SCK_RATE = 0X16;
//------------------------------------------------------------------------------
// card types
/** Standard capacity V1 SD card */
uint8_t const SD_CARD_TYPE_SD1 = 1;
/** Standard capacity V2 SD card */
uint8_t const SD_CARD_TYPE_SD2 = 2;
/** High Capacity SD card */
uint8_t const SD_CARD_TYPE_SDHC = 3;
//------------------------------------------------------------------------------



// SD card commands
/** GO_IDLE_STATE - init card in spi mode if CS low */
uint8_t const CMD0 = 0X00;
/** SEND_IF_COND - verify SD Memory Card interface operating condition.*/
uint8_t const CMD8 = 0X08;
/** SEND_CSD - read the Card Specific Data (CSD register) */
uint8_t const CMD9 = 0X09;
/** SEND_CID - read the card identification information (CID register) */
uint8_t const CMD10 = 0X0A;
/** SEND_STATUS - read the card status register */
uint8_t const CMD13 = 0X0D;
/** READ_BLOCK - read a single data block from the card */
uint8_t const CMD17 = 0X11;
/** WRITE_BLOCK - write a single data block to the card */
uint8_t const CMD24 = 0X18;
/** WRITE_MULTIPLE_BLOCK - write blocks of data until a STOP_TRANSMISSION */
uint8_t const CMD25 = 0X19;
/** ERASE_WR_BLK_START - sets the address of the first block to be erased */
uint8_t const CMD32 = 0X20;
/** ERASE_WR_BLK_END - sets the address of the last block of the continuous
    range to be erased*/
uint8_t const CMD33 = 0X21;
/** ERASE - erase all previously selected blocks */
uint8_t const CMD38 = 0X26;
/** APP_CMD - escape for application specific command */
uint8_t const CMD55 = 0X37;
/** READ_OCR - read the OCR register of a card */
uint8_t const CMD58 = 0X3A;
/** SET_WR_BLK_ERASE_COUNT - Set the number of write blocks to be
     pre-erased before writing */
uint8_t const ACMD23 = 0X17;
/** SD_SEND_OP_COMD - Sends host capacity support information and
    activates the card's initialization process */
uint8_t const ACMD41 = 0X29;
//------------------------------------------------------------------------------
/** status for card in the ready state */
uint8_t const R1_READY_STATE = 0X00;
/** status for card in the idle state */
uint8_t const R1_IDLE_STATE = 0X01;
/** status bit for illegal command */
uint8_t const R1_ILLEGAL_COMMAND = 0X04;
/** start data token for read or write single block*/
uint8_t const DATA_START_BLOCK = 0XFE;
/** stop token for write multiple blocks*/
uint8_t const STOP_TRAN_TOKEN = 0XFD;
/** start data token for write multiple blocks*/
uint8_t const WRITE_MULTIPLE_TOKEN = 0XFC;
/** mask for data response tokens after a write block operation */
uint8_t const DATA_RES_MASK = 0X1F;
/** write data accepted token */
uint8_t const DATA_RES_ACCEPTED = 0X05;
//------------------------------------------------------------------------------


typedef struct {
    uint8_t status;
    uint8_t type;
    uint8_t error;
} SD_Init_TypeDef;


// #define DEBUG_SD_CARD 1


/* Список функций */
SD_Init_TypeDef SD_Init(GPIO_TypeDef *port, uint8_t chipSelectPin_);
uint8_t SD_sendData(uint8_t data);
uint8_t SD_receiveData();
uint8_t SD_cardCommand(uint8_t cmd, uint32_t arg);
uint8_t SD_cardAcmd(uint8_t cmd, uint32_t arg);
uint8_t SD_waitNotBusy(uint32_t timeoutMillis);


static uint8_t chipSelectPin_;
static GPIO_TypeDef *port_;



// Инициализация 
SD_Init_TypeDef SD_Init(GPIO_TypeDef *port, uint8_t chipSelectPin)
{        
    uint32_t arg;
    uint8_t status_ = 0;
    uint8_t type_ = 0;
    uint8_t error_ = 0;    
    uint32_t d = 0;
    SD_Init_TypeDef response;

    chipSelectPin_ = chipSelectPin;
    port_ = &port;

    SPI1_Master_Init(0);     

    // set pin modes
    if (chipSelectPin_ < 8) {
        port->CFGLR &= ~(GPIO_Msk << chipSelectPin_*4); // ~(0b1111);
        port->CFGLR |= (GPIO_Speed_50 << chipSelectPin_*4); // 0b0011;
    }else {
        port->CFGHR &= ~(GPIO_Msk << (chipSelectPin_-8)*4); // ~(0b1111);
        port->CFGHR |= (GPIO_Speed_50 << (chipSelectPin_-8)*4); // 0b0011;
    }

    // chipSelectHigh
    port->BSHR |= (1 << chipSelectPin_); 
    
    // must supply min of 74 clock cycles with CS high.
    for (uint8_t i = 0; i < 10; i++) {
        SD_sendData(0XFF);
    } 

    // chipSelectLow
    port->BCR |= (1 << chipSelectPin_);

#ifdef DEBUG_SD_CARD
    printf("command to go idle in SPI mode\r\n");
#endif

    d = 0;
    // command to go idle in SPI mode
    while ((status_ = SD_cardCommand(CMD0, 0)) != R1_IDLE_STATE) {
        d++;
        if (d > (SD_INIT_TIMEOUT << 2)) {
            error_ = SD_CARD_ERROR_CMD0;
            goto fail;
        }
    }

#ifdef DEBUG_SD_CARD
    printf("check SD version\r\n");
#endif

    // check SD version
    if ((SD_cardCommand(CMD8, 0x1AA) & R1_ILLEGAL_COMMAND)) {
        type_ = SD_CARD_TYPE_SD1;
    } else {
        // only need last byte of r7 response
        for (uint8_t i = 0; i < 4; i++) {
            status_ = SD_receiveData();
        }
        if (status_ != 0XAA) {
            error_ = SD_CARD_ERROR_CMD8;
            goto fail;
        }
        type_ = SD_CARD_TYPE_SD2;
    }
    
#ifdef DEBUG_SD_CARD
    printf("initialize card and send host supports SDHC\r\n");
#endif

    // initialize card and send host supports SDHC if SD2
    // arg = type() == SD_CARD_TYPE_SD2 ? 0X40000000 : 0;
    arg = type_ == SD_CARD_TYPE_SD2 ? 0X40000000 : 0;

    d = 0;
    while ((status_ = SD_cardAcmd(ACMD41, arg)) != R1_READY_STATE) {
        // check for timeout  
        d++;
        if (d > (SD_INIT_TIMEOUT << 2)) {
            error_ = SD_CARD_ERROR_ACMD41;
            goto fail;
        }
    }
    
#ifdef DEBUG_SD_CARD
    printf("if SD2 read OCR register\r\n");
#endif

    // if SD2 read OCR register to check for SDHC card
    if (type_ == SD_CARD_TYPE_SD2) {
        if (SD_cardCommand(CMD58, 0)) {
            error_ = SD_CARD_ERROR_CMD58;
            goto fail;
        }
        if ((SD_receiveData() & 0XC0) == 0XC0) {
            type_ = SD_CARD_TYPE_SDHC;
        }
        // discard rest of ocr - contains allowed voltage range
        for (uint8_t i = 0; i < 3; i++) {
            SD_receiveData();
        }
    }

#ifdef DEBUG_SD_CARD
    printf("end before fail\r\n");
#endif
    
fail:

#ifdef DEBUG_SD_CARD
    printf("end after fail\r\n");
#endif

    // chipSelectHigh
    port->BSHR |= (1 << chipSelectPin_);

    response.status = status_;
    response.type = type_;
    response.error = error_;

    return response;
}


// отправка данных
uint8_t SD_sendData(uint8_t data)
{
    u8 i = 0;

    while(SPI1_GetFlagStatus(SPI_I2S_FLAG_TXE) == RESET)
    {
        i++;
        if(i > 200)
            return 0;
    }

    SPI1_SendData(data);

    return 1;
}


// приём данных
uint8_t SD_receiveData()
{
    SD_sendData(0XFF);

    u8 i = 0;

    while(SPI1_GetFlagStatus(SPI_I2S_FLAG_RXNE) == RESET);
    {
        i++;
        if(i > 200)
            return 0;
    }

    return SPI1_ReceiveData();
}


// send command and return error code.  Return zero for OK
uint8_t SD_cardCommand(uint8_t cmd, uint32_t arg)
{
    uint8_t status_;

    // chipSelectLow
    port_->BCR |= (1 << chipSelectPin_);

    // wait up to 300 ms if busy
    SD_waitNotBusy(300);

    // send command
    SD_sendData(cmd | 0x40);

    // send argument
    for (int8_t s = 24; s >= 0; s -= 8) {
        SD_sendData(arg >> s);
    }

    // send CRC
    uint8_t crc = 0XFF;
    if (cmd == CMD0) {
        crc = 0X95;  // correct crc for CMD0 with arg 0
    }
    if (cmd == CMD8) {
        crc = 0X87;  // correct crc for CMD8 with arg 0X1AA
    }
    SD_sendData(crc);

    // wait for response
    for (uint8_t i = 0; ((status_ = SD_receiveData()) & 0X80) && i != 0XFF; i++)
        ;

    return status_;
}


//
uint8_t SD_cardAcmd(uint8_t cmd, uint32_t arg) 
{
    SD_cardCommand(CMD55, 0);
    return SD_cardCommand(cmd, arg);
}


// wait for card to go not busy
uint8_t SD_waitNotBusy(uint32_t timeoutMillis)
{
    uint32_t d = 0;
    do {
        if (SD_receiveData() == 0XFF) {
            return 1;
        }
        d++;
    } while (d < (timeoutMillis << 2));

    return 0;
}



#endif /* __SD_CARD_H_ */
