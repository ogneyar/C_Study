
#ifndef __SD_CARD_H_
#define __SD_CARD_H_

#include "sd_defines.h"
#include "defines.h"
#include "system.h"
#include "usart.h"
#include "spi.h"


// #define DEBUG_SD_CARD 1
// #define DEBUG_SD_VOLUME 1
// #define DEBUG_SD_FILE 1

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
uint8_t SDvolume_chainSize(uint32_t cluster, uint32_t* size);
uint8_t SDvolume_isEOC(uint32_t cluster);
uint8_t SDvolume_fatGet(uint32_t cluster, uint32_t* value);
uint8_t SDvolume_blockOfCluster(uint32_t position);
uint32_t SDvolume_clusterStartBlock(uint32_t cluster);
uint8_t SDvolume_freeChain(uint32_t cluster);
uint8_t SDvolume_fatPut(uint32_t cluster, uint32_t value);
void SDvolume_cacheSetDirty(void);
uint8_t SDvolume_fatPutEOC(uint32_t cluster);
uint8_t SDvolume_cacheZeroBlock(uint32_t blockNumber);
uint8_t SDvolume_allocContiguous(uint32_t count, uint32_t* curCluster);
// SDfile
uint8_t SDfile_openRoot(void);
void SDfile_ls(uint8_t flags, uint8_t indent);
void SDfile_rewind(void);
dir_t* SDfile_readDirCache(void);
int16_t SDfile_read_(void);
int16_t SDfile_read(void* buf, uint16_t nbyte);
uint8_t SDfile_isDir(void);
uint8_t SDfile_isOpen(void);
uint8_t SDfile_isFile(void);
uint8_t SDfile_isSubDir(void);
uint8_t SDfile_isRoot(void);
uint8_t SDfile_unbufferedRead(void);
void SDfile_printDirName(const dir_t dir, uint8_t width);
void SDfile_printFatDate(uint16_t fatDate);
void SDfile_printFatTime(uint16_t fatTime);
void SDfile_printTwoDigits(uint8_t v);
uint8_t SDfile_makeDir(const char* dirName);
uint8_t SDfile_close(void);
uint8_t SDfile_sync(uint8_t blocking);
dir_t* SDfile_cacheDirEntry(uint8_t action);
uint8_t SDfile_openByIndex(uint16_t index, uint8_t oflag); // open by index
uint8_t SDfile_seekSet(uint32_t pos);
uint8_t SDfile_openCachedEntry(uint8_t dirIndex, uint8_t oflag);
uint8_t SDfile_truncate(uint32_t length);
uint8_t SDfile_open(const char* fileName, uint8_t oflag); // open by name
uint8_t SDfile_make83Name(const char* str, uint8_t* name);
uint8_t SDfile_addDirCluster(void);
uint8_t SDfile_addCluster(void);




/**********************************************************************************

                *********************************************
                *                                           *
                *                  SDcard                   *
                *                                           *
                *********************************************

**********************************************************************************/

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






/**********************************************************************************

                *********************************************
                *                                           *
                *                 SDvolume                  *
                *                                           *
                *********************************************

**********************************************************************************/

//
uint8_t SDvolume_init()
{
    uint32_t volumeStartBlock = 0;
    
    if ( ! SDvolume_cacheRawBlock(volumeStartBlock, CACHE_FOR_READ) ) {        
#ifdef DEBUG_SD_VOLUME
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
#ifdef DEBUG_SD_VOLUME
        printf("not valid FAT volume\r\n");
        printf("bpb->bytesPerSector: %d\r\n", bpb->bytesPerSector);
        printf("bpb->fatCount: %d\r\n", bpb->fatCount);
        printf("bpb->reservedSectorCount: %d\r\n", bpb->reservedSectorCount);
        printf("bpb->sectorsPerCluster: %d\r\n", bpb->sectorsPerCluster);
#endif
        return 0;
    }
    SDvolume_fatCount_ = bpb->fatCount;
    SDvolume_blocksPerCluster_ = bpb->sectorsPerCluster;

    // determine shift that is same as multiply by blocksPerCluster_
    SDvolume_clusterSizeShift_ = 0;
    while (SDvolume_blocksPerCluster_ != (1 << SDvolume_clusterSizeShift_)) {
        // error if not power of 2
        if (SDvolume_clusterSizeShift_++ > 7) {
#ifdef DEBUG_SD_VOLUME
            printf("error if not power of 2\r\n");
#endif
            return 0;
        }
    }
    SDvolume_blocksPerFat_ = bpb->sectorsPerFat16 ?
                    bpb->sectorsPerFat16 : bpb->sectorsPerFat32;

    SDvolume_fatStartBlock_ = volumeStartBlock + bpb->reservedSectorCount;

    // count for FAT16 zero for FAT32
    SDvolume_rootDirEntryCount_ = bpb->rootDirEntryCount;

    // directory start for FAT16 dataStart for FAT32
    SDvolume_rootDirStart_ = SDvolume_fatStartBlock_ + bpb->fatCount * SDvolume_blocksPerFat_;

    // data start for FAT16 and FAT32
    SDvolume_dataStartBlock_ = SDvolume_rootDirStart_ + ((32 * bpb->rootDirEntryCount + 511) / 512);

    // total blocks for FAT16 or FAT32
    uint32_t totalBlocks = bpb->totalSectors16 ?
                            bpb->totalSectors16 : bpb->totalSectors32;
    // total data blocks
    SDvolume_clusterCount_ = totalBlocks - (SDvolume_dataStartBlock_ - volumeStartBlock);

    // divide by cluster size to get cluster count
    SDvolume_clusterCount_ >>= SDvolume_clusterSizeShift_;

    // FAT type is determined by cluster count
    if (SDvolume_clusterCount_ < 4085) {
        SDvolume_fatType_ = 12;
    } else if (SDvolume_clusterCount_ < 65525) {
        SDvolume_fatType_ = 16;
    } else {
        SDvolume_rootDirStart_ = bpb->fat32RootCluster;
        SDvolume_fatType_ = 32;
    }
#ifdef DEBUG_SD_VOLUME
    printf("fatType_: %d\r\n", fatType_);
#endif

    return 1;
}


//
uint8_t SDvolume_cacheRawBlock(uint32_t blockNumber, uint8_t action)
{
    if (SDvolume_cacheBlockNumber_ != blockNumber) {
        if ( ! SDvolume_cacheFlush(1) ) { // 1 по умолчанию
#ifdef DEBUG_SD_VOLUME
        printf(" ! SDvolume_cacheFlush\r\n");
#endif
            return 0;
        }
        if ( ! SD_readBlock(blockNumber, SDvolume_cacheBuffer_.data) ) {   
#ifdef DEBUG_SD_VOLUME
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
uint8_t SDvolume_cacheFlush(uint8_t blocking) // 1 по умолчанию
{
    if (SDvolume_cacheDirty_) 
    {
        if ( ! SD_writeBlock(SDvolume_cacheBlockNumber_, SDvolume_cacheBuffer_.data, blocking)) {
#ifdef DEBUG_SD_FILE
            printf("SDvolume_cacheFlush ! SD_writeBlock\r\n");
            printf("SDvolume_cacheFlush SDvolume_cacheDirty_ = %d\r\n", SDvolume_cacheDirty_);
            printf("SDvolume_cacheFlush SDvolume_cacheBlockNumber_ = %d\r\n", SDvolume_cacheBlockNumber_);
#endif
            return 0;
        }

        if ( ! blocking) {
            return 1;
        }

        // mirror FAT tables
        if ( ! SDvolume_cacheMirrorBlockFlush(blocking) ) {
#ifdef DEBUG_SD_FILE
            printf("SDvolume_cacheFlush ! SDvolume_cacheMirrorBlockFlush\r\n");
#endif
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


// return the size in bytes of a cluster chain
uint8_t SDvolume_chainSize(uint32_t cluster, uint32_t* size)
{
    uint32_t s = 0;
    do {
        if ( ! SDvolume_fatGet(cluster, &cluster)) {
            return 0;
        }
        s += 512UL << SDvolume_clusterSizeShift_;
    } while ( ! SDvolume_isEOC(cluster));
    *size = s;

    return 1;
}


//
uint8_t SDvolume_isEOC(uint32_t cluster)
{
  return  cluster >= (SDvolume_fatType_ == 16 ? FAT16EOC_MIN : FAT32EOC_MIN);
}


// Fetch a FAT entry
uint8_t SDvolume_fatGet(uint32_t cluster, uint32_t* value)
{
    if (cluster > (SDvolume_clusterCount_ + 1)) {
        return 0;
    }
    uint32_t lba = SDvolume_fatStartBlock_;
    lba += SDvolume_fatType_ == 16 ? cluster >> 8 : cluster >> 7;
    if (lba != SDvolume_cacheBlockNumber_) {
        if ( ! SDvolume_cacheRawBlock(lba, CACHE_FOR_READ)) {
        return 0;
        }
    }
    if (SDvolume_fatType_ == 16) {
        *value = SDvolume_cacheBuffer_.fat16[cluster & 0XFF];
    } else {
        *value = SDvolume_cacheBuffer_.fat32[cluster & 0X7F] & FAT32MASK;
    }

    return 1;
}


//
uint8_t SDvolume_blockOfCluster(uint32_t position)
{
    return (position >> 9) & (SDvolume_blocksPerCluster_ - 1);
}


//
uint32_t SDvolume_clusterStartBlock(uint32_t cluster)
{
    return SDvolume_dataStartBlock_ + ((cluster - 2) << SDvolume_clusterSizeShift_);
}


//
uint8_t SDvolume_freeChain(uint32_t cluster)
{
    // clear free cluster location
    SDvolume_allocSearchStart_ = 2;

    do {
        uint32_t next;
        if ( ! SDvolume_fatGet(cluster, &next)) {
            return 0;
        }

        // free cluster
        if ( ! SDvolume_fatPut(cluster, 0)) {
            return 0;
        }

        cluster = next;
    } while ( ! SDvolume_isEOC(cluster));

    return 1;
}


//
uint8_t SDvolume_fatPut(uint32_t cluster, uint32_t value)
{
    // error if reserved cluster
    if (cluster < 2) {
        return 0;
    }

    // error if not in FAT
    if (cluster > (SDvolume_clusterCount_ + 1)) {
        return 0;
    }

    // calculate block address for entry
    uint32_t lba = SDvolume_fatStartBlock_;
    lba += SDvolume_fatType_ == 16 ? cluster >> 8 : cluster >> 7;

    if (lba != SDvolume_cacheBlockNumber_) {
        if ( ! SDvolume_cacheRawBlock(lba, CACHE_FOR_READ)) {
        return 0;
        }
    }
    // store entry
    if (SDvolume_fatType_ == 16) {
        SDvolume_cacheBuffer_.fat16[cluster & 0XFF] = value;
    } else {
        SDvolume_cacheBuffer_.fat32[cluster & 0X7F] = value;
    }
    SDvolume_cacheSetDirty();

    // mirror second FAT
    if (SDvolume_fatCount_ > 1) {
        SDvolume_cacheMirrorBlock_ = lba + SDvolume_blocksPerFat_;
    }

    return 1;
}


//
void SDvolume_cacheSetDirty(void)
{
    SDvolume_cacheDirty_ |= CACHE_FOR_WRITE;
}


//
uint8_t SDvolume_fatPutEOC(uint32_t cluster)
{
  return SDvolume_fatPut(cluster, 0x0FFFFFFF);
}


//
uint8_t SDvolume_cacheZeroBlock(uint32_t blockNumber)
{
    if ( ! SDvolume_cacheFlush(1)) { // 1 по умолчанию
#ifdef DEBUG_SD_FILE
        printf("SDvolume_cacheZeroBlock ! SDvolume_cacheFlush\r\n");
#endif
        return 0;
    }

    // loop take less flash than memset(cacheBuffer_.data, 0, 512);
    for (uint16_t i = 0; i < 512; i++) {
        SDvolume_cacheBuffer_.data[i] = 0;
    }
    SDvolume_cacheBlockNumber_ = blockNumber;
    SDvolume_cacheSetDirty();

    return 1;
}


//
uint8_t SDvolume_allocContiguous(uint32_t count, uint32_t* curCluster)
{
    // start of group
    uint32_t bgnCluster;

    // flag to save place to start next search
    uint8_t setStart;

    // set search start cluster
    if (*curCluster) {
        // try to make file contiguous
        bgnCluster = *curCluster + 1;

        // don't save new start location
        setStart = 0;
    } else {
        // start at likely place for free cluster
        bgnCluster = SDvolume_allocSearchStart_;

        // save next search start if one cluster
        setStart = 1 == count;
    }
    // end of group
    uint32_t endCluster = bgnCluster;

    // last cluster of FAT
    uint32_t fatEnd = SDvolume_clusterCount_ + 1;

    // search the FAT for free clusters
    for (uint32_t n = 0;; n++, endCluster++) {
        // can't find space checked all clusters
        if (n >= SDvolume_clusterCount_) {
            return 0;
        }

        // past end - start from beginning of FAT
        if (endCluster > fatEnd) {
        bgnCluster = endCluster = 2;
        }
        uint32_t f;
        if ( ! SDvolume_fatGet(endCluster, &f)) {
            return 0;
        }

        if (f != 0) {
        // cluster in use try next cluster as bgnCluster
        bgnCluster = endCluster + 1;
        } else if ((endCluster - bgnCluster + 1) == count) {
            // done - found space
            break;
        }
    }
    // mark end of chain
    if ( ! SDvolume_fatPutEOC(endCluster)) {
        return 0;
    }

    // link clusters
    while (endCluster > bgnCluster) {
        if ( ! SDvolume_fatPut(endCluster - 1, endCluster)) {
            return 0;
        }
        endCluster--;
    }
    if (*curCluster != 0) {
        // connect chains
        if ( ! SDvolume_fatPut(*curCluster, bgnCluster)) {
            return 0;
        }
    }
    // return first cluster number to caller
    *curCluster = bgnCluster;

    // remember possible next free cluster
    if (setStart) {
        SDvolume_allocSearchStart_ = bgnCluster + 1;
    }

    return 1;
}




/**********************************************************************************

                *********************************************
                *                                           *
                *                    SDfile                 *
                *                                           *
                *********************************************

**********************************************************************************/


//
uint8_t SDfile_openRoot(void)
{
    // error if file is already open
    if (SDfile_isOpen()) {
        return 0;
    }

    if (SDvolume_fatType_ == 16) {
        SDfile_type_ = FAT_FILE_TYPE_ROOT16;
        SDfile_firstCluster_ = 0;
        SDfile_fileSize_ = 32 * SDvolume_rootDirEntryCount_;
    } else if (SDvolume_fatType_ == 32) {
        SDfile_type_ = FAT_FILE_TYPE_ROOT32;
        SDfile_firstCluster_ = SDvolume_rootDirStart_;
        if ( ! SDvolume_chainSize(SDfile_firstCluster_, &SDfile_fileSize_)) {
            return 0;
        }
    } else {
        // volume is not initialized or FAT12
        return 0;
    }
    
    // read only
    SDfile_flags_ = O_READ;

    // set to start of file
    SDfile_curCluster_ = 0;
    SDfile_curPosition_ = 0;

    // root has no directory entry
    SDfile_dirBlock_ = 0;
    SDfile_dirIndex_ = 0;

    return 1;
}


//
void SDfile_ls(uint8_t flags, uint8_t indent) // indent =  0, по умолчанию
{
    dir_t* p;

    SDfile_rewind();
    while ((p = SDfile_readDirCache())) {
        // done if past last used entry
        if (p->name[0] == DIR_NAME_FREE) {
            break;
        }

        // skip deleted entry and entries for . and  ..
        if (p->name[0] == DIR_NAME_DELETED || p->name[0] == '.') {
            continue;
        }

        // only list subdirectories and files
        if ( ! DIR_IS_FILE_OR_SUBDIR(p)) {
            continue;
        }

        // print any indent spaces
        for (int8_t i = 0; i < indent; i++) {
            printf(" ");
        }

#ifdef DEBUG_SD_FILE
        printf("SDfile_ls print dir/file name\r\n");
#endif

        // print file name with possible blank fill
        SDfile_printDirName(*p, flags & (LS_DATE | LS_SIZE) ? 14 : 0);

        // print modify date/time if requested
        if (flags & LS_DATE) {
            SDfile_printFatDate(p->lastWriteDate);
            printf(" ");
            SDfile_printFatTime(p->lastWriteTime);
        }
        // print size if requested
        if ( ! DIR_IS_SUBDIR(p) && (flags & LS_SIZE)) {            
#ifdef DEBUG_SD_FILE
            printf("SDfile_ls print file size\r\n");
#endif
            printf(" ");
            if (p->fileSize >= GIGABYTE) { // >= 1 Gb
                printf("%d,%d Gb", p->fileSize / GIGABYTE, p->fileSize % GIGABYTE);
            }else
            if (p->fileSize >= MEGABYTE) { // >= 1 Mb
                printf("%d,%d Mb", p->fileSize / MEGABYTE, p->fileSize % MEGABYTE);
            }else
            if (p->fileSize >= KILOBYTE) { // >= 1 Kb
                printf("%d,%d Kb", p->fileSize / KILOBYTE, p->fileSize % KILOBYTE);
            }else {
                printf("%d b", p->fileSize);
            }
        }
        printf("\r\n");

        // list subdirectory content if requested
        if ((flags & LS_R) && DIR_IS_SUBDIR(p)) {                 
#ifdef DEBUG_SD_FILE
            printf("list subdirectory content\r\n");
#endif
            uint16_t index = SDfile_curPosition_ / 32 - 1;
            /* на си надо как-то рекурсивно вызвать эту же самую библиотеку */
            /* так не работает!!! */
            if (SDfile_openByIndex(index, O_READ)) {
                SDfile_ls(flags, indent + 2);
            }
            SDfile_seekSet(32 * (index + 1));
        }
    }
}


//
void SDfile_rewind(void)
{
    SDfile_curPosition_ = SDfile_curCluster_ = 0;
}


//
dir_t* SDfile_readDirCache(void)
{
    // error if not directory
    if ( ! SDfile_isDir()) {
        return NULL;
    }

    // index of entry in cache
    uint8_t i = (SDfile_curPosition_ >> 5) & 0XF;

    // use read to locate and cache block
    if (SDfile_read_() < 0) {
        return NULL;
    }

    // advance to next entry
    SDfile_curPosition_ += 31;

    // return pointer to entry
    return (SDvolume_cacheBuffer_.dir + i);
}


//
int16_t SDfile_read_(void)
{
  uint8_t b;
  return SDfile_read(&b, 1) == 1 ? b : -1;
}


//
int16_t SDfile_read(void* buf, uint16_t nbyte)
{
    uint8_t* dst = (uint8_t*)(buf);

    // error if not open or write only
    if ( ! SDfile_isOpen() || !(SDfile_flags_ & O_READ)) {
        return -1;
    }

    // max bytes left in file
    if (nbyte > (SDfile_fileSize_ - SDfile_curPosition_)) {
        nbyte = SDfile_fileSize_ - SDfile_curPosition_;
    }

    // amount left to read
    uint16_t toRead = nbyte;
    while (toRead > 0) {
        uint32_t block;  // raw device block number
        uint16_t offset = SDfile_curPosition_ & 0X1FF;  // offset in block
        if (SDfile_type_ == FAT_FILE_TYPE_ROOT16) {
        block = SDvolume_rootDirStart_ + (SDfile_curPosition_ >> 9);
        } else {
        uint8_t blockOfCluster = SDvolume_blockOfCluster(SDfile_curPosition_);
        if (offset == 0 && blockOfCluster == 0) {
            // start of new cluster
            if (SDfile_curPosition_ == 0) {
                // use first cluster in file
                SDfile_curCluster_ = SDfile_firstCluster_;
            } else {
                // get next cluster from FAT
                if ( ! SDvolume_fatGet(SDfile_curCluster_, &SDfile_curCluster_)) {
                    return -1;
                }
            }
        }
        block = SDvolume_clusterStartBlock(SDfile_curCluster_) + blockOfCluster;
        }
        uint16_t n = toRead;

        // amount to be read from current block
        if (n > (512 - offset)) {
        n = 512 - offset;
        }

        // no buffering needed if n == 512 or user requests no buffering
        if ((SDfile_unbufferedRead() || n == 512) &&
            block != SDvolume_cacheBlockNumber_) {
        if ( ! SD_readData(block, offset, n, dst)) {
            return -1;
        }
        dst += n;
        } else {
        // read block to cache and copy data to caller
        if ( ! SDvolume_cacheRawBlock(block, CACHE_FOR_READ)) {
            return -1;
        }
        uint8_t* src = SDvolume_cacheBuffer_.data + offset;
        uint8_t* end = src + n;
        while (src != end) {
            *dst++ = *src++;
        }
        }
        SDfile_curPosition_ += n;
        toRead -= n;
    }
    return nbyte;
}


//
uint8_t SDfile_isDir(void)
{
  return SDfile_type_ >= FAT_FILE_TYPE_MIN_DIR;
}


//
uint8_t SDfile_isOpen(void)
{
  return SDfile_type_ != FAT_FILE_TYPE_CLOSED;
}


//
uint8_t SDfile_isFile(void)
{
    return SDfile_type_ == FAT_FILE_TYPE_NORMAL;
}


//
uint8_t SDfile_isSubDir(void)
{
    return SDfile_type_ == FAT_FILE_TYPE_SUBDIR;
}


//
uint8_t SDfile_isRoot(void)
{
    return SDfile_type_ == FAT_FILE_TYPE_ROOT16 || SDfile_type_ == FAT_FILE_TYPE_ROOT32;
}


//
uint8_t SDfile_unbufferedRead(void)
{
    return SDfile_flags_ & F_FILE_UNBUFFERED_READ;
}


//
// void SDfile_printDirName(const dir_t& dir, uint8_t width)
void SDfile_printDirName(const dir_t dir, uint8_t width)
{
    uint8_t w = 0;
    for (uint8_t i = 0; i < 11; i++) {
        if (dir.name[i] == ' ') {
            continue;
        }
        if (i == 8) {
            printf(".");
            w++;
        }
        printf("%c", dir.name[i]);
        w++;
    }
    if (DIR_IS_SUBDIR(&dir)) {
        printf("/");
        w++;
    }
    while (w < width) {
        printf(" ");
        w++;
    }
}


//
void SDfile_printFatDate(uint16_t fatDate)
{
    printf("%d", FAT_YEAR(fatDate)); 
    printf("-");
    SDfile_printTwoDigits(FAT_MONTH(fatDate));
    printf("-");
    SDfile_printTwoDigits(FAT_DAY(fatDate));
}


//
void SDfile_printFatTime(uint16_t fatTime)
{
    SDfile_printTwoDigits(FAT_HOUR(fatTime));
    printf(":");
    SDfile_printTwoDigits(FAT_MINUTE(fatTime));
    printf(":");
    SDfile_printTwoDigits(FAT_SECOND(fatTime));
}


//
void SDfile_printTwoDigits(uint8_t v)
{
    char str[3];
    str[0] = '0' + v / 10;
    str[1] = '0' + v % 10;
    str[2] = 0;
    printf(str);
}


//
uint8_t SDfile_makeDir(const char* dirName)
{
    dir_t d;

    // create a normal file
    if ( ! SDfile_open(dirName, O_CREAT | O_EXCL | O_RDWR)) {             
#ifdef DEBUG_SD_FILE
        printf(" ! SDfile_open\r\n");
#endif
        return 0;
    }

    // convert SdFile to directory
    SDfile_flags_ = O_READ;
    SDfile_type_ = FAT_FILE_TYPE_SUBDIR;

    // allocate and zero first cluster
    if ( ! SDfile_addDirCluster()) {        
#ifdef DEBUG_SD_FILE
        printf(" ! SDfile_addDirCluster\r\n");
#endif
        return 0;
    }

    // force entry to SD
    if ( ! SDfile_sync(1)) { // 1 по умолчанию   
#ifdef DEBUG_SD_FILE
        printf(" ! SDfile_sync\r\n");
#endif
        return 0;
    }

    // cache entry - should already be in cache due to sync() call
    dir_t* p = SDfile_cacheDirEntry(CACHE_FOR_WRITE);  
    if (!p) {
#ifdef DEBUG_SD_FILE
        printf(" ! SDfile_cacheDirEntry\r\n");
#endif
        return 0;
    }

    // change directory entry  attribute
    p->attributes = DIR_ATT_DIRECTORY;

    // make entry for '.'
    memcpy(&d, p, sizeof(d));
    for (uint8_t i = 1; i < 11; i++) {
        d.name[i] = ' ';
    }
    d.name[0] = '.';

    // cache block for '.'  and '..'
    uint32_t block = SDvolume_clusterStartBlock(SDfile_firstCluster_);
    if ( ! SDvolume_cacheRawBlock(block, CACHE_FOR_WRITE)) { 
#ifdef DEBUG_SD_FILE
        printf(" ! SDvolume_cacheRawBlock\r\n");
#endif
        return 0;
    }

    // copy '.' to block
    memcpy(&SDvolume_cacheBuffer_.dir[0], &d, sizeof(d));

    // make entry for '..'
    d.name[1] = '.';
    if (SDfile_isRoot()) {
        d.firstClusterLow = 0;
        d.firstClusterHigh = 0;
    } else {
        d.firstClusterLow = SDfile_firstCluster_ & 0XFFFF;
        d.firstClusterHigh = SDfile_firstCluster_ >> 16;
    }
    // copy '..' to block
    memcpy(&SDvolume_cacheBuffer_.dir[1], &d, sizeof(d));

    // set position after '..'
    SDfile_curPosition_ = 2 * sizeof(d);

    // write first block
    return SDvolume_cacheFlush(1); // 1 по умолчанию
}


//
uint8_t SDfile_close(void) 
{
    printf("SDfile close\r\n");

    if ( ! SDfile_sync(1)) { // 1 по умолчанию
        return 0;
    }
    SDfile_type_ = FAT_FILE_TYPE_CLOSED;

    return 1;
}



uint8_t SDfile_sync(uint8_t blocking) // 1 по умолчанию
{
    // only allow open files and directories
    if ( ! SDfile_isOpen()) {
        return 0;
    }

    if (SDfile_flags_ & F_FILE_DIR_DIRTY) {
        dir_t* d = SDfile_cacheDirEntry(CACHE_FOR_WRITE);
        if (!d) {
            return 0;
        }

        // do not set filesize for dir files
        if ( ! SDfile_isDir()) {
            d->fileSize = SDfile_fileSize_;
        }

        // update first cluster fields
        d->firstClusterLow = SDfile_firstCluster_ & 0XFFFF;
        d->firstClusterHigh = SDfile_firstCluster_ >> 16;

        // set modify time if user supplied a callback date/time function
        if (dateTime_) {
            dateTime_(&d->lastWriteDate, &d->lastWriteTime);
            d->lastAccessDate = d->lastWriteDate;
        }
        // clear directory dirty
        SDfile_flags_ &= ~F_FILE_DIR_DIRTY;
    }

    if ( ! blocking) {
        SDfile_flags_ &= ~F_FILE_NON_BLOCKING_WRITE;
    }

    return SDvolume_cacheFlush(blocking);
}


//
dir_t* SDfile_cacheDirEntry(uint8_t action)
{
  if ( ! SDvolume_cacheRawBlock(SDfile_dirBlock_, action)) {
    return NULL;
  }
  return SDvolume_cacheBuffer_.dir + SDfile_dirIndex_;
}


//
uint8_t SDfile_openByIndex(uint16_t index, uint8_t oflag)
{
    // error if already open
    if (SDfile_isOpen()) {
        return 0;
    }

    // don't open existing file if O_CREAT and O_EXCL - user call error
    if ((oflag & (O_CREAT | O_EXCL)) == (O_CREAT | O_EXCL)) {
        return 0;
    }

    // seek to location of entry
    if ( ! SDfile_seekSet(32 * index)) {
        return 0;
    }

    // read entry into cache
    dir_t* p = SDfile_readDirCache();
    if (p == NULL) {
        return 0;
    }

    // error if empty slot or '.' or '..'
    if (p->name[0] == DIR_NAME_FREE ||
        p->name[0] == DIR_NAME_DELETED || p->name[0] == '.') {
        return 0;
    }
    // open cached entry
    return SDfile_openCachedEntry(index & 0XF, oflag);
}


//
uint8_t SDfile_seekSet(uint32_t pos)
{
    // error if file not open or seek past end of file
    if ( ! SDfile_isOpen() || pos > SDfile_fileSize_) {
        return 0;
    }

    if (SDfile_type_ == FAT_FILE_TYPE_ROOT16) {
        SDfile_curPosition_ = pos;
        return 1;
    }
    if (pos == 0) {
        // set position to start of file
        SDfile_curCluster_ = 0;
        SDfile_curPosition_ = 0;
        return 1;
    }
    // calculate cluster index for cur and new position
    uint32_t nCur = (SDfile_curPosition_ - 1) >> (SDvolume_clusterSizeShift_ + 9);
    uint32_t nNew = (pos - 1) >> (SDvolume_clusterSizeShift_ + 9);

    if (nNew < nCur || SDfile_curPosition_ == 0) {
        // must follow chain from first cluster
        SDfile_curCluster_ = SDfile_firstCluster_;
    } else {
        // advance from curPosition
        nNew -= nCur;
    }
    while (nNew--) {
        if ( ! SDvolume_fatGet(SDfile_curCluster_, &SDfile_curCluster_)) {
            return 0;
        }
    }
    SDfile_curPosition_ = pos;

    return 1;
}


//
uint8_t SDfile_openCachedEntry(uint8_t dirIndex, uint8_t oflag)
{
    // location of entry in cache
    dir_t* p = SDvolume_cacheBuffer_.dir + dirIndex;

    // write or truncate is an error for a directory or read-only file
    if (p->attributes & (DIR_ATT_READ_ONLY | DIR_ATT_DIRECTORY)) {
        if (oflag & (O_WRITE | O_TRUNC)) {
        return 0;
        }
    }
    // remember location of directory entry on SD
    SDfile_dirIndex_ = dirIndex;
    SDfile_dirBlock_ = SDvolume_cacheBlockNumber_;

    // copy first cluster number for directory fields
    SDfile_firstCluster_ = (uint32_t)p->firstClusterHigh << 16;
    SDfile_firstCluster_ |= p->firstClusterLow;

    // make sure it is a normal file or subdirectory
    if (DIR_IS_FILE(p)) {
        SDfile_fileSize_ = p->fileSize;
        SDfile_type_ = FAT_FILE_TYPE_NORMAL;
    } else if (DIR_IS_SUBDIR(p)) {
        if ( ! SDvolume_chainSize(SDfile_firstCluster_, &SDfile_fileSize_)) {
        return 0;
        }
        SDfile_type_ = FAT_FILE_TYPE_SUBDIR;
    } else {
        return 0;
    }
    // save open flags for read/write
    SDfile_flags_ = oflag & (O_ACCMODE | O_SYNC | O_APPEND);

    // set to start of file
    SDfile_curCluster_ = 0;
    SDfile_curPosition_ = 0;

    // truncate file to zero length if requested
    if (oflag & O_TRUNC) {
        return SDfile_truncate(0);
    }
    return 1;
}


//
uint8_t SDfile_truncate(uint32_t length)
{
    // error if not a normal file or read-only
    if ( ! SDfile_isFile() || !(SDfile_flags_ & O_WRITE)) {
        return 0;
    }

    // error if length is greater than current size
    if (length > SDfile_fileSize_) {
        return 0;
    }

    // fileSize and length are zero - nothing to do
    if (SDfile_fileSize_ == 0) {
        return 1;
    }

    // remember position for seek after truncation
    uint32_t newPos = SDfile_curPosition_ > length ? length : SDfile_curPosition_;

    // position to last cluster in truncated file
    if ( ! SDfile_seekSet(length)) {
        return 0;
    }

    if (length == 0) {
        // free all clusters
        if ( ! SDvolume_freeChain(SDfile_firstCluster_)) {
        return 0;
        }
        SDfile_firstCluster_ = 0;
    } else {
        uint32_t toFree;
        if ( ! SDvolume_fatGet(SDfile_curCluster_, &toFree)) {
        return 0;
        }

        if ( ! SDvolume_isEOC(toFree)) {
        // free extra clusters
        if ( ! SDvolume_freeChain(toFree)) {
            return 0;
        }

        // current cluster is end of chain
        if ( ! SDvolume_fatPutEOC(SDfile_curCluster_)) {
            return 0;
        }
        }
    }
    SDfile_fileSize_ = length;

    // need to update directory entry
    SDfile_flags_ |= F_FILE_DIR_DIRTY;

    if ( ! SDfile_sync(1)) { // 1 по умолчанию
        return 0;
    }

    // set file to correct position
    return SDfile_seekSet(newPos);
}


// Open By Name
uint8_t SDfile_open(const char* fileName, uint8_t oflag)
{
    uint8_t dname[11];
    dir_t* p;

    // error if already open
    if (SDfile_isOpen()) {                    
#ifdef DEBUG_SD_FILE
        printf("SDfile_open SDfile_isOpen\r\n");
#endif
        return 0;
    }

    if ( ! SDfile_make83Name(fileName, dname)) {                
#ifdef DEBUG_SD_FILE
        printf("SDfile_open ! SDfile_make83Name\r\n");
#endif
        return 0;
    }
    
    SDfile_rewind();
                  
    // bool for empty entry found
    uint8_t emptyFound = 0;

    // search for file
    while (SDfile_curPosition_ < SDfile_fileSize_) {
        uint8_t index = 0XF & (SDfile_curPosition_ >> 5);
        p = SDfile_readDirCache();
        if (p == NULL) {
            return 0;
        }

        if (p->name[0] == DIR_NAME_FREE || p->name[0] == DIR_NAME_DELETED) {
        // remember first empty slot
        if (!emptyFound) {
            emptyFound = 1;
            SDfile_dirIndex_ = index;
            SDfile_dirBlock_ = SDvolume_cacheBlockNumber_;
        }
        // done if no entries follow
        if (p->name[0] == DIR_NAME_FREE) {
            break;
        }
        } else if (!memcmp(dname, p->name, 11)) {
        // don't open existing file if O_CREAT and O_EXCL
        if ((oflag & (O_CREAT | O_EXCL)) == (O_CREAT | O_EXCL)) {
            return 0;
        }

        // open found file
        return SDfile_openCachedEntry(0XF & index, oflag);
        }
    }
    
    // only create file if O_CREAT and O_WRITE
    if ((oflag & (O_CREAT | O_WRITE)) != (O_CREAT | O_WRITE)) {
#ifdef DEBUG_SD_FILE
        printf("SDfile_open (oflag & (O_CREAT | O_WRITE)) != (O_CREAT | O_WRITE)\r\n");
#endif
        return 0;
    }

    // cache found slot or add cluster if end of file
    if (emptyFound) {
        p = SDfile_cacheDirEntry(CACHE_FOR_WRITE);
        if (!p) {
#ifdef DEBUG_SD_FILE
            printf("SDfile_open ! SDfile_cacheDirEntry\r\n");
#endif
            return 0;
        }
    } else {
        if (SDfile_type_ == FAT_FILE_TYPE_ROOT16) {
#ifdef DEBUG_SD_FILE
            printf("SDfile_open file_type_ == FAT_FILE_TYPE_ROOT16\r\n");
#endif
            return 0;
        }

        // add and zero cluster for dirFile - first cluster is in cache for write
        if ( ! SDfile_addDirCluster()) {            
#ifdef DEBUG_SD_FILE
            printf("SDfile_open ! SDfile_addDirCluster\r\n");
#endif
            return 0;
        }

        // use first entry in cluster
        SDfile_dirIndex_ = 0;
        p = SDvolume_cacheBuffer_.dir;
    }
    // initialize as empty file
    memset(p, 0, sizeof(dir_t));
    memcpy(p->name, dname, 11);

    // set timestamps
    if (dateTime_) {
        // call user function
        dateTime_(&p->creationDate, &p->creationTime);
    } else {
        // use default date/time
        p->creationDate = FAT_DEFAULT_DATE;
        p->creationTime = FAT_DEFAULT_TIME;
    }
    p->lastAccessDate = p->creationDate;
    p->lastWriteDate = p->creationDate;
    p->lastWriteTime = p->creationTime;

    // force write of entry to SD
    if ( ! SDvolume_cacheFlush(1)) { // 1 по умолчанию         
#ifdef DEBUG_SD_FILE
        printf("SDfile_open ! SDvolume_cacheFlush\r\n");
#endif
        return 0;
    }

    // open entry in cache
    return SDfile_openCachedEntry(SDfile_dirIndex_, oflag);
}


uint8_t SDfile_make83Name(const char* str, uint8_t* name)
{
    uint8_t c;
    uint8_t n = 7;  // max index for part before dot
    uint8_t i = 0;
    // blank fill name and extension
    while (i < 11) {
        name[i++] = ' ';
    }
    i = 0;
    while ((c = *str++) != '\0') {
        if (c == '.') {
            if (n == 10) {
                return 0;  // only one dot allowed
            }
            n = 10;  // max index for full 8.3 name
            i = 8;   // place for extension
        } else {
            // illegal FAT characters
            uint8_t b;
            const uint8_t valid[] = "|<>^+=?/[];,*\"\\";
            const uint8_t *p = valid;
            while ((b = *p++)) if (b == c) {
                return 0;
            }
            // check size and only allow ASCII printable characters
            if (i > n || c < 0X21 || c > 0X7E) {
                return 0;
            }
            // only upper case allowed in 8.3 names - convert lower to upper
            name[i++] = c < 'a' || c > 'z' ?  c : c + ('A' - 'a');
        }
    }
    // must have a file name, extension is optional
    return name[0] != ' ';
}


//
uint8_t SDfile_addDirCluster(void)
{
    if ( ! SDfile_addCluster()) {        
#ifdef DEBUG_SD_FILE
        printf("SDfile_addDirCluster ! SDfile_addCluster\r\n");
#endif
        return 0;
    }

    // zero data in cluster insure first cluster is in cache
    uint32_t block = SDvolume_clusterStartBlock(SDfile_curCluster_);
    for (uint8_t i = SDvolume_blocksPerCluster_; i != 0; i--) {
        if ( ! SDvolume_cacheZeroBlock(block + i - 1)) {     
#ifdef DEBUG_SD_FILE
            printf("SDfile_addDirCluster ! SDvolume_cacheZeroBlock\r\n");
            printf("SDfile_addDirCluster block = %d + (i = %d) - 1\r\n", block, i);
            printf("SDvolume_blocksPerCluster_ = %d\r\n", SDvolume_blocksPerCluster_);
#endif
            return 0;
        }
    }
    // Increase directory file size by cluster size
    SDfile_fileSize_ += 512UL << SDvolume_clusterSizeShift_;

    return 1;
}


//
uint8_t SDfile_addCluster(void)
{
    if ( ! SDvolume_allocContiguous(1, &SDfile_curCluster_)) {
        return 0;
    }

    // if first cluster of file link to directory entry
    if (SDfile_firstCluster_ == 0) {
        SDfile_firstCluster_ = SDfile_curCluster_;
        SDfile_flags_ |= F_FILE_DIR_DIRTY;
    }
    SDfile_flags_ |= F_FILE_CLUSTER_ADDED;

    return 1;
}



#endif /* __SD_CARD_H_ */
