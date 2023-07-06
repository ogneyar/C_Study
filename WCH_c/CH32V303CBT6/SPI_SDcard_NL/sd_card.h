
#ifndef __SD_CARD_H_
#define __SD_CARD_H_

#include "defines.h"
#include "system.h"
#include "usart.h"
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


/* SDinfo.h */

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
    uint8_t errorCode;
} SD_Init_TypeDef;


//------------------------------------------------------------------------------

/* FatStructs.h */

struct biosParmBlock {
    uint16_t bytesPerSector;
    uint8_t  sectorsPerCluster;
    uint16_t reservedSectorCount;
    uint8_t  fatCount;
    uint16_t rootDirEntryCount;
    uint16_t totalSectors16;
    uint8_t  mediaType;
    uint16_t sectorsPerFat16;
    uint16_t sectorsPerTrtack;
    uint16_t headCount;
    uint32_t hidddenSectors;
    uint32_t totalSectors32;
    uint32_t sectorsPerFat32;
    uint16_t fat32Flags;
    uint16_t fat32Version;
    uint32_t fat32RootCluster;
    uint16_t fat32FSInfo;
    uint16_t fat32BackBootBlock;
    uint8_t  fat32Reserved[12];
} __attribute__((packed));

typedef struct biosParmBlock bpb_t;


struct fat32BootSector {
    uint8_t  jmpToBootCode[3];
    char     oemName[8];
    bpb_t    bpb;
    uint8_t  driveNumber;
    uint8_t  reserved1;
    uint8_t  bootSignature;
    uint32_t volumeSerialNumber;
    char     volumeLabel[11];
    char     fileSystemType[8];
    uint8_t  bootCode[420];
    uint8_t  bootSectorSig0;
    uint8_t  bootSectorSig1;
} __attribute__((packed));

uint16_t const FAT16EOC = 0XFFFF;
uint16_t const FAT16EOC_MIN = 0XFFF8;
uint32_t const FAT32EOC = 0X0FFFFFFF;
uint32_t const FAT32EOC_MIN = 0X0FFFFFF8;
uint32_t const FAT32MASK = 0X0FFFFFFF;

typedef struct fat32BootSector fbs_t;


struct directoryEntry {
    uint8_t  name[11];
    uint8_t  attributes;
    uint8_t  reservedNT;
    uint8_t  creationTimeTenths;
    uint16_t creationTime;
    uint16_t creationDate;
    uint16_t lastAccessDate;
    uint16_t firstClusterHigh;
    uint16_t lastWriteTime;
    uint16_t lastWriteDate;
    uint16_t firstClusterLow;
    uint32_t fileSize;
} __attribute__((packed));

typedef struct directoryEntry dir_t;


struct partitionTable {
    uint8_t  boot;
    uint8_t  beginHead;
    unsigned beginSector : 6;
    unsigned beginCylinderHigh : 2;
    uint8_t  beginCylinderLow;
    uint8_t  type;
    uint8_t  endHead;
    unsigned endSector : 6;
    unsigned endCylinderHigh : 2;
    uint8_t  endCylinderLow;
    uint32_t firstSector;
    uint32_t totalSectors;
} __attribute__((packed));

typedef struct partitionTable part_t;


struct masterBootRecord {
  uint8_t  codeArea[440];
  uint32_t diskSignature;
  uint16_t usuallyZero;
  part_t   part[4];
  uint8_t  mbrSig0;
  uint8_t  mbrSig1;
} __attribute__((packed));

typedef struct masterBootRecord mbr_t;

//------------------------------------------------------------------------------

/* SdFat.h */

typedef union {
    uint8_t  data[512];
    uint16_t fat16[256];
    uint32_t fat32[128];
    dir_t    dir[16];
    mbr_t    mbr;
    fbs_t    fbs;
} cache_t;


static uint8_t const CACHE_FOR_READ = 0;
static uint8_t const CACHE_FOR_WRITE = 1;

// static uint32_t allocSearchStart_ = 2;   // start cluster for alloc search
static uint8_t blocksPerCluster_;    // cluster size in blocks
static uint32_t blocksPerFat_;       // FAT size in blocks
static uint32_t clusterCount_;       // clusters in one FAT
static uint8_t clusterSizeShift_;    // shift to convert cluster count to block count
static uint32_t dataStartBlock_;     // first data block number
static uint8_t fatCount_;            // number of FATs on volume
static uint32_t fatStartBlock_;      // start block for first FAT
static uint8_t fatType_ = 0;             // volume type (12, 16, OR 32)
static uint16_t rootDirEntryCount_;  // number of entries in FAT16 root dir
static uint32_t rootDirStart_;       // root start block for FAT16, cluster for FAT32

//------------------------------------------------------------------------------

/* SdVolume.cpp */

static uint32_t SDvolume_cacheBlockNumber_ = 0XFFFFFFFF;
static cache_t  SDvolume_cacheBuffer_;     // 512 byte cache for Sd2Card
// Sd2Card* SDvolume_sdCard_;          // pointer to SD card object
static uint8_t  SDvolume_cacheDirty_ = 0;  // cacheFlush() will write block if true
static uint32_t SDvolume_cacheMirrorBlock_ = 0;  // mirror  block for second FAT

//------------------------------------------------------------------------------


// #define DEBUG_SD_CARD 1
#define SD_PROTECT_BLOCK_ZERO 1


/* Список функций */
// SDcard
SD_Init_TypeDef SD_Init(GPIO_TypeDef *port, uint8_t chipSelectPin);
uint8_t SD_sendData(uint8_t data);
uint8_t SD_receiveData();
uint8_t SD_cardCommand(uint8_t cmd, uint32_t arg);
void SD_readEnd(void);
uint8_t SD_cardAcmd(uint8_t cmd, uint32_t arg);
uint8_t SD_waitNotBusy(uint32_t timeoutMillis);
uint8_t SD_readBlock(uint32_t block, uint8_t* dst);
uint8_t SD_readData(uint32_t block, uint16_t offset, uint16_t count, uint8_t* dst);
uint8_t SD_waitStartBlock(void);
uint8_t SD_writeBlock(uint32_t blockNumber, const uint8_t* src, uint8_t blocking);
uint8_t SD_writeData(uint8_t token, const uint8_t* src);
// SDvolume
uint8_t SDvolume_init();
uint8_t SDvolume_cacheRawBlock(uint32_t blockNumber, uint8_t action);
uint8_t SDvolume_cacheFlush(uint8_t blocking);
uint8_t SDvolume_cacheMirrorBlockFlush(uint8_t blocking);


static GPIO_TypeDef *SD_port_ = GPIOA;
static uint32_t SD_block_ = 0;
static uint8_t SD_chipSelectPin_ = 0;
static uint8_t SD_errorCode_ = 0;
static uint8_t SD_inBlock_ = 0;
static uint16_t SD_offset_ = 0;
static uint8_t SD_partialBlockRead_ = 0;
static uint8_t SD_status_ = 0;
static uint8_t SD_type_ = 0;


// Инициализация 
SD_Init_TypeDef SD_Init(GPIO_TypeDef *port, uint8_t chipSelectPin)
{        
    uint32_t arg;
    uint8_t SD_status_ = 0;
    uint8_t SD_type_ = 0;
    uint8_t SD_errorCode_ = 0;    
    uint32_t d = 0;
    SD_Init_TypeDef response;

    SD_chipSelectPin_ = chipSelectPin;
    SD_port_ = &port;

    SPI1_Master_Init(0);     

    // set pin modes
    if (SD_chipSelectPin_ < 8) {
        port->CFGLR &= ~(GPIO_Msk << SD_chipSelectPin_*4); // ~(0b1111);
        port->CFGLR |= (GPIO_Speed_50 << SD_chipSelectPin_*4); // 0b0011;
    }else {
        port->CFGHR &= ~(GPIO_Msk << (SD_chipSelectPin_-8)*4); // ~(0b1111);
        port->CFGHR |= (GPIO_Speed_50 << (SD_chipSelectPin_-8)*4); // 0b0011;
    }

    // chipSelectHigh
    SD_port_->BSHR |= (1 << SD_chipSelectPin_); 
    
    // must supply min of 74 clock cycles with CS high.
    for (uint8_t i = 0; i < 10; i++) {
        SD_sendData(0XFF);
    } 

    // chipSelectLow
    SD_port_->BCR |= (1 << SD_chipSelectPin_);

#ifdef DEBUG_SD_CARD
    printf("command to go idle in SPI mode\r\n");
#endif

    d = 0;
    // command to go idle in SPI mode
    while ((SD_status_ = SD_cardCommand(CMD0, 0)) != R1_IDLE_STATE) {
        d++;
        if (d > (SD_INIT_TIMEOUT << 2)) {
            SD_errorCode_ = SD_CARD_ERROR_CMD0;
            goto fail;
        }
    }

#ifdef DEBUG_SD_CARD
    printf("check SD version\r\n");
#endif

    // check SD version
    if ((SD_cardCommand(CMD8, 0x1AA) & R1_ILLEGAL_COMMAND)) {
        SD_type_ = SD_CARD_TYPE_SD1;
    } else {
        // only need last byte of r7 response
        for (uint8_t i = 0; i < 4; i++) {
            SD_status_ = SD_receiveData();
        }
        if (SD_status_ != 0XAA) {
            SD_errorCode_ = SD_CARD_ERROR_CMD8;
            goto fail;
        }
        SD_type_ = SD_CARD_TYPE_SD2;
    }
    
#ifdef DEBUG_SD_CARD
    printf("initialize card and send host supports SDHC\r\n");
#endif

    // initialize card and send host supports SDHC if SD2
    // arg = type() == SD_CARD_TYPE_SD2 ? 0X40000000 : 0;
    arg = SD_type_ == SD_CARD_TYPE_SD2 ? 0X40000000 : 0;

    d = 0;
    while ((SD_status_ = SD_cardAcmd(ACMD41, arg)) != R1_READY_STATE) {
        // check for timeout  
        d++;
        if (d > (SD_INIT_TIMEOUT << 2)) {
            SD_errorCode_ = SD_CARD_ERROR_ACMD41;
            goto fail;
        }
    }
    
#ifdef DEBUG_SD_CARD
    printf("if SD2 read OCR register\r\n");
#endif

    // if SD2 read OCR register to check for SDHC card
    if (SD_type_ == SD_CARD_TYPE_SD2) {
        if (SD_cardCommand(CMD58, 0)) {
            SD_errorCode_ = SD_CARD_ERROR_CMD58;
            goto fail;
        }
        if ((SD_receiveData() & 0XC0) == 0XC0) {
            SD_type_ = SD_CARD_TYPE_SDHC;
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
    SD_port_->BSHR |= (1 << SD_chipSelectPin_);

    response.status = SD_status_;
    response.type = SD_type_;
    response.errorCode = SD_errorCode_;

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

    SD_readEnd();

    // chipSelectLow
    SD_port_->BCR |= (1 << SD_chipSelectPin_);

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
void SD_readEnd(void)
{
    if (SD_inBlock_) {
        while (SD_offset_++ < 514) {
            SD_receiveData();
        }
        // chipSelectHigh
        SD_port_->BSHR |= (1 << SD_chipSelectPin_); 

        SD_inBlock_ = 0;
    }
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


//
uint8_t SD_readBlock(uint32_t block, uint8_t* dst) 
{
  return SD_readData(block, 0, 512, dst);
}


//
uint8_t SD_readData(uint32_t block, uint16_t offset, uint16_t count, uint8_t* dst)
{
    if (count == 0) {
        return 1;
    }
    if ((count + offset) > 512) {  
#ifdef DEBUG_SD_CARD
        printf("SD_readData (count + offset) > 512\r\n");
#endif
        goto fail;
    }
    if ( ! SD_inBlock_ || block != SD_block_ || offset < SD_offset_) {
        SD_block_ = block;
        // use address if not SDHC card
        if (SD_type_ != SD_CARD_TYPE_SDHC) {
            block <<= 9;
        }
        if (SD_cardCommand(CMD17, block)) {
            SD_errorCode_ = SD_CARD_ERROR_CMD17; 
#ifdef DEBUG_SD_CARD
            printf("SD_readData SD_errorCode_: %d\r\n", SD_errorCode_);
#endif
            goto fail;
        }
        if ( ! SD_waitStartBlock() ) {
#ifdef DEBUG_SD_CARD
            printf("SD_readData ! SD_waitStartBlock\r\n");
#endif
            goto fail;
        }
        SD_offset_ = 0;
        SD_inBlock_ = 1;
    }

    // skip data before offset
    for (; SD_offset_ < offset; SD_offset_++) {
        SD_receiveData();
    }
    // transfer data
    for (uint16_t i = 0; i < count; i++) {
        dst[i] = SD_receiveData();
    }

    SD_offset_ += count;
    if ( ! SD_partialBlockRead_ || SD_offset_ >= 512) {
        // read rest of data, checksum and set chip select high
        SD_readEnd();
    }
    return 1;

fail:
    // chipSelectHigh
    SD_port_->BSHR |= (1 << SD_chipSelectPin_); 

    return 0;
}


//
uint8_t SD_waitStartBlock(void)
{
    uint32_t d = 0;
    while ((SD_status_ = SD_receiveData()) == 0XFF) {
        d++;
        if (d > (SD_READ_TIMEOUT << 10)) {
            SD_errorCode_ = SD_CARD_ERROR_READ_TIMEOUT;
#ifdef DEBUG_SD_CARD
            printf("SD_waitStartBlock SD_READ_TIMEOUT SD_errorCode_: %d\r\n", SD_errorCode_);
#endif
            goto fail;
        }
    }
    if (SD_status_ != DATA_START_BLOCK) {
        SD_errorCode_ = SD_CARD_ERROR_READ;
#ifdef DEBUG_SD_CARD
        printf("SD_waitStartBlock SD_status_ != DATA_START_BLOCK SD_errorCode_: %d\r\n", SD_errorCode_);
#endif
        goto fail;
    }
    return 1;

fail:
    // chipSelectHigh
    SD_port_->BSHR |= (1 << SD_chipSelectPin_); 

    return 0;
}


//
uint8_t SD_writeBlock(uint32_t blockNumber, const uint8_t* src, uint8_t blocking)
{

#if SD_PROTECT_BLOCK_ZERO
    // don't allow write to first block
    if (blockNumber == 0) {
        SD_errorCode_ = SD_CARD_ERROR_WRITE_BLOCK_ZERO;
        goto fail;
    }
#endif  // SD_PROTECT_BLOCK_ZERO

    // use address if not SDHC card
    if (SD_type_ != SD_CARD_TYPE_SDHC) {
        blockNumber <<= 9;
    }
    if (SD_cardCommand(CMD24, blockNumber)) {
        SD_errorCode_ = SD_CARD_ERROR_CMD24;
        goto fail;
    }
    if ( ! SD_writeData(DATA_START_BLOCK, src)) {
        goto fail;
    }
    if (blocking) {
        // wait for flash programming to complete
        if ( ! SD_waitNotBusy(SD_WRITE_TIMEOUT)) {
            SD_errorCode_ = SD_CARD_ERROR_WRITE_TIMEOUT;
            goto fail;
        }
        // response is r2 so get and check two bytes for nonzero
        if (SD_cardCommand(CMD13, 0) || SD_receiveData()) {
            SD_errorCode_ = SD_CARD_ERROR_WRITE_PROGRAMMING;
            goto fail;
        }
    }
    // chipSelectHigh
    SD_port_->BSHR |= (1 << SD_chipSelectPin_);

    return 1;

fail:
    // chipSelectHigh
    SD_port_->BSHR |= (1 << SD_chipSelectPin_);

    return 0;
}


// send one block of data for write block or write multiple blocks
uint8_t SD_writeData(uint8_t token, const uint8_t* src) 
{
    SD_sendData(token);
    for (uint16_t i = 0; i < 512; i++) {
        SD_sendData(src[i]);
    }
    SD_sendData(0xff);  // dummy crc
    SD_sendData(0xff);  // dummy crc

    SD_status_ = SD_receiveData();

    if ((SD_status_ & DATA_RES_MASK) != DATA_RES_ACCEPTED) 
    {
        SD_errorCode_ = SD_CARD_ERROR_WRITE;
        // chipSelectHigh
        SD_port_->BSHR |= (1 << SD_chipSelectPin_);

        return 0;
    }

    return 1;
}


/***************
 
    SDvolume

***************/

//
uint8_t SDvolume_init()
{
    uint32_t volumeStartBlock = 0;
    
    if ( ! SDvolume_cacheRawBlock(volumeStartBlock, CACHE_FOR_READ) ) {        
#ifdef DEBUG_SD_CARD
        printf(" ! SDvolume_cacheRawBlock\r\n");
#endif
        return 0;
    }
    bpb_t* bpb = &SDvolume_cacheBuffer_.fbs.bpb;
    if (bpb->bytesPerSector != 512 ||
        bpb->fatCount == 0 ||
        bpb->reservedSectorCount == 0 ||
        bpb->sectorsPerCluster == 0) {
        // not valid FAT volume  
#ifdef DEBUG_SD_CARD
        printf("not valid FAT volume\r\n");
        printf("bpb->bytesPerSector: %d\r\n", bpb->bytesPerSector);
        printf("bpb->fatCount: %d\r\n", bpb->fatCount);
        printf("bpb->reservedSectorCount: %d\r\n", bpb->reservedSectorCount);
        printf("bpb->sectorsPerCluster: %d\r\n", bpb->sectorsPerCluster);
#endif
        return 0;
    }
    fatCount_ = bpb->fatCount;
    blocksPerCluster_ = bpb->sectorsPerCluster;

    // determine shift that is same as multiply by blocksPerCluster_
    clusterSizeShift_ = 0;
    while (blocksPerCluster_ != (1 << clusterSizeShift_)) {
        // error if not power of 2
        if (clusterSizeShift_++ > 7) {
#ifdef DEBUG_SD_CARD
            printf("error if not power of 2\r\n");
#endif
            return 0;
        }
    }
    blocksPerFat_ = bpb->sectorsPerFat16 ?
                    bpb->sectorsPerFat16 : bpb->sectorsPerFat32;

    fatStartBlock_ = volumeStartBlock + bpb->reservedSectorCount;

    // count for FAT16 zero for FAT32
    rootDirEntryCount_ = bpb->rootDirEntryCount;

    // directory start for FAT16 dataStart for FAT32
    rootDirStart_ = fatStartBlock_ + bpb->fatCount * blocksPerFat_;

    // data start for FAT16 and FAT32
    dataStartBlock_ = rootDirStart_ + ((32 * bpb->rootDirEntryCount + 511) / 512);

    // total blocks for FAT16 or FAT32
    uint32_t totalBlocks = bpb->totalSectors16 ?
                            bpb->totalSectors16 : bpb->totalSectors32;
    // total data blocks
    clusterCount_ = totalBlocks - (dataStartBlock_ - volumeStartBlock);

    // divide by cluster size to get cluster count
    clusterCount_ >>= clusterSizeShift_;

    // FAT type is determined by cluster count
    if (clusterCount_ < 4085) {
        fatType_ = 12;
    } else if (clusterCount_ < 65525) {
        fatType_ = 16;
    } else {
        rootDirStart_ = bpb->fat32RootCluster;
        fatType_ = 32;
    }
#ifdef DEBUG_SD_CARD
    printf("fatType_: %d\r\n", fatType_);
#endif

    return 1;
}


//
uint8_t SDvolume_cacheRawBlock(uint32_t blockNumber, uint8_t action)
{
    if (SDvolume_cacheBlockNumber_ != blockNumber) {
        if ( ! SDvolume_cacheFlush(1) ) { // 1 по умолчанию
#ifdef DEBUG_SD_CARD
        printf(" ! SDvolume_cacheFlush\r\n");
#endif
            return 0;
        }
        if ( ! SD_readBlock(blockNumber, SDvolume_cacheBuffer_.data) ) {   
#ifdef DEBUG_SD_CARD
        printf(" ! SD_readBlock\r\n");
#endif
            return 0;
        }
        SDvolume_cacheBlockNumber_ = blockNumber;
    }
    SDvolume_cacheDirty_ |= action;
    return 1;
}


//
uint8_t SDvolume_cacheFlush(uint8_t blocking) 
{
    if (SDvolume_cacheDirty_) 
    {
        if ( ! SD_writeBlock(SDvolume_cacheBlockNumber_, SDvolume_cacheBuffer_.data, blocking)) {
            return 0;
        }

        if ( ! blocking) {
            return 1;
        }

        // mirror FAT tables
        if ( ! SDvolume_cacheMirrorBlockFlush(blocking) ) {
            return 0;
        }
        SDvolume_cacheDirty_ = 0;
    }

    return 1;
}


//
uint8_t SDvolume_cacheMirrorBlockFlush(uint8_t blocking)
{
  if (SDvolume_cacheMirrorBlock_) 
  {
    if ( ! SD_writeBlock(SDvolume_cacheMirrorBlock_, SDvolume_cacheBuffer_.data, blocking)) {
      return 0;
    }
    SDvolume_cacheMirrorBlock_ = 0;
  }

  return 1;
}


#endif /* __SD_CARD_H_ */
