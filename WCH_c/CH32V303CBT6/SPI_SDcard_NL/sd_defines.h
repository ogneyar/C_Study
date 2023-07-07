
#ifndef __SD_DEFINES_H_
#define __SD_DEFINES_H_

#include "sd_card.h"


uint32_t GIGABYTE = (1024 * 1024 * 1024);
uint32_t MEGABYTE = (1024 * 1024);
uint32_t KILOBYTE = (1024);


unsigned int const SD_INIT_TIMEOUT = 2000;
unsigned int const SD_ERASE_TIMEOUT = 10000;
unsigned int const SD_READ_TIMEOUT = 300;
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


uint8_t const DIR_NAME_0XE5 = 0X05;
uint8_t const DIR_NAME_DELETED = 0XE5;
uint8_t const DIR_NAME_FREE = 0X00;
uint8_t const DIR_ATT_READ_ONLY = 0X01;
uint8_t const DIR_ATT_HIDDEN = 0X02;
uint8_t const DIR_ATT_SYSTEM = 0X04;
uint8_t const DIR_ATT_VOLUME_ID = 0X08;
uint8_t const DIR_ATT_DIRECTORY = 0X10;
uint8_t const DIR_ATT_ARCHIVE = 0X20;
uint8_t const DIR_ATT_LONG_NAME = 0X0F;
uint8_t const DIR_ATT_LONG_NAME_MASK = 0X3F;
uint8_t const DIR_ATT_DEFINED_BITS = 0X3F;


static inline uint8_t DIR_IS_LONG_NAME(const dir_t* dir) {
  return (dir->attributes & DIR_ATT_LONG_NAME_MASK) == DIR_ATT_LONG_NAME;
}

uint8_t const DIR_ATT_FILE_TYPE_MASK = (DIR_ATT_VOLUME_ID | DIR_ATT_DIRECTORY);

static inline uint8_t DIR_IS_FILE(const dir_t* dir) {
  return (dir->attributes & DIR_ATT_FILE_TYPE_MASK) == 0;
}

static inline uint8_t DIR_IS_SUBDIR(const dir_t* dir) {
  return (dir->attributes & DIR_ATT_FILE_TYPE_MASK) == DIR_ATT_DIRECTORY;
}

static inline uint8_t DIR_IS_FILE_OR_SUBDIR(const dir_t* dir) {
  return (dir->attributes & DIR_ATT_VOLUME_ID) == 0;
}


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


// flags for ls()
uint8_t const LS_DATE = 1;
uint8_t const LS_SIZE = 2;
uint8_t const LS_R = 4;

// use the gnu style oflag in open()
uint8_t const O_READ = 0X01;
uint8_t const O_RDONLY = O_READ;
uint8_t const O_WRITE = 0X02;
uint8_t const O_WRONLY = O_WRITE;
uint8_t const O_RDWR = (O_READ | O_WRITE);
uint8_t const O_ACCMODE = (O_READ | O_WRITE);
uint8_t const O_APPEND = 0X04;
uint8_t const O_SYNC = 0X08;
uint8_t const O_CREAT = 0X10;
uint8_t const O_EXCL = 0X20;
uint8_t const O_TRUNC = 0X40;

// flags for timestamp
uint8_t const T_ACCESS = 1;
uint8_t const T_CREATE = 2;
uint8_t const T_WRITE = 4;

// values for type_
uint8_t const FAT_FILE_TYPE_CLOSED = 0;
uint8_t const FAT_FILE_TYPE_NORMAL = 1;
uint8_t const FAT_FILE_TYPE_ROOT16 = 2;
uint8_t const FAT_FILE_TYPE_ROOT32 = 3;
uint8_t const FAT_FILE_TYPE_SUBDIR = 4;
uint8_t const FAT_FILE_TYPE_MIN_DIR = FAT_FILE_TYPE_ROOT16;


static inline uint16_t FAT_DATE(uint16_t year, uint8_t month, uint8_t day) {
  return (year - 1980) << 9 | month << 5 | day;
}

static inline uint16_t FAT_YEAR(uint16_t fatDate) {
  return 1980 + (fatDate >> 9);
}

static inline uint8_t FAT_MONTH(uint16_t fatDate) {
  return (fatDate >> 5) & 0XF;
}

static inline uint8_t FAT_DAY(uint16_t fatDate) {
  return fatDate & 0X1F;
}

static inline uint16_t FAT_TIME(uint8_t hour, uint8_t minute, uint8_t second) {
  return hour << 11 | minute << 5 | second >> 1;
}

static inline uint8_t FAT_HOUR(uint16_t fatTime) {
  return fatTime >> 11;
}

static inline uint8_t FAT_MINUTE(uint16_t fatTime) {
  return (fatTime >> 5) & 0X3F;
}

static inline uint8_t FAT_SECOND(uint16_t fatTime) {
  return 2 * (fatTime & 0X1F);
}

uint16_t const FAT_DEFAULT_DATE = ((2000 - 1980) << 9) | (1 << 5) | 1;
uint16_t const FAT_DEFAULT_TIME = (1 << 11);

// bits defined in flags_
static uint8_t const F_OFLAG = (O_ACCMODE | O_APPEND | O_SYNC);
static uint8_t const F_FILE_NON_BLOCKING_WRITE = 0X10;
static uint8_t const F_FILE_CLUSTER_ADDED = 0X20;
static uint8_t const F_FILE_UNBUFFERED_READ = 0X40;
static uint8_t const F_FILE_DIR_DIRTY = 0X80;



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

static uint32_t SDvolume_allocSearchStart_ = 2;   // start cluster for alloc search
static uint8_t SDvolume_blocksPerCluster_;    // cluster size in blocks
static uint32_t SDvolume_blocksPerFat_;       // FAT size in blocks
static uint32_t SDvolume_clusterCount_;       // clusters in one FAT
static uint8_t SDvolume_clusterSizeShift_;    // shift to convert cluster count to block count
static uint32_t SDvolume_dataStartBlock_;     // first data block number
static uint8_t SDvolume_fatCount_;            // number of FATs on volume
static uint32_t SDvolume_fatStartBlock_;      // start block for first FAT
static uint8_t SDvolume_fatType_ = 0;         // volume type (12, 16, OR 32)
static uint16_t SDvolume_rootDirEntryCount_;  // number of entries in FAT16 root dir
static uint32_t SDvolume_rootDirStart_;       // root start block for FAT16, cluster for FAT32

//------------------------------------------------------------------------------

/* SdVolume.cpp */

static uint32_t SDvolume_cacheBlockNumber_ = 0XFFFFFFFF;
static cache_t  SDvolume_cacheBuffer_;     // 512 byte cache for Sd Card
static uint8_t  SDvolume_cacheDirty_ = 0;  // cacheFlush() will write block if true
static uint32_t SDvolume_cacheMirrorBlock_ = 0;  // mirror  block for second FAT

//------------------------------------------------------------------------------


// SDcard
static GPIO_TypeDef *SD_port_ = GPIOA;
static uint32_t SD_block_ = 0;
static uint8_t SD_chipSelectPin_ = 0;
static uint8_t SD_errorCode_ = 0;
static uint8_t SD_inBlock_ = 0;
static uint16_t SD_offset_ = 0;
static uint8_t SD_partialBlockRead_ = 0;
static uint8_t SD_status_ = 0;
static uint8_t SD_type_ = 0;


// SDfile
static uint8_t   SDfile_flags_;         // See above for definition of flags_ bits
static uint8_t   SDfile_type_ = FAT_FILE_TYPE_CLOSED; // type of file see above for values
static uint32_t  SDfile_curCluster_;    // cluster for current file position
static uint32_t  SDfile_curPosition_;   // current file position in bytes from beginning
static uint32_t  SDfile_dirBlock_;      // SD block that contains directory entry for file
static uint8_t   SDfile_dirIndex_;      // index of entry in dirBlock 0 <= dirIndex_ <= 0XF
static uint32_t  SDfile_fileSize_;      // file size in bytes
static uint32_t  SDfile_firstCluster_;  // first cluster of file
// SdVolume* vol_;           // volume where file is located



// callback function for date/time
void (*dateTime_)(uint16_t* date, uint16_t* time) = NULL;
void (*oldDateTime_)(uint16_t date, uint16_t time) = NULL;  // NOLINT



#endif /* __SD_DEFINES_H_ */
