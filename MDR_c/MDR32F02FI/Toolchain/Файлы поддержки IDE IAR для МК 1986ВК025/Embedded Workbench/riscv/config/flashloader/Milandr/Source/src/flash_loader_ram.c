
#include "flash_loader.h"
#include "flash_loader_extra.h"

#include "flash_loader_ram.h"
#include <intrinsics.h>

#define CORE_FREQ 8000000UL

#define delay(mks) delay_hw((int)((mks)*1.0)) // derating

static inline void __attribute__((always_inline))
delay_hw (register uint32_t mks) {
    register uint32_t t;
    t = __read_csr(_CSR_MCYCLE) + mks*(CORE_FREQ/1000000);
    while ( (int32_t)(__read_csr(_CSR_MCYCLE) - t) < 0 );
}


uint32_t FlashInit(void *base_of_flash, uint32_t image_size,
                   uint32_t link_address, uint32_t flags,
                   int argc, char const *argv[])
{
    if (flags & FLAG_ERASE_ONLY)
    {
        // perform full erase
        
        register uint32_t mem_type;
        register uint32_t flash_cmd;
        
        mem_type = 0;
        
        EEPROM_CNTR->KEY = 0x8AAA5551; 
        flash_cmd = EEPROM_CNTR->CMD;
        
        /* Switch on register access */
        flash_cmd = (flash_cmd & FLASH_DELAY_MASK) | FLASH_CON | FLASH_TMR;
        if (mem_type)
        {
            flash_cmd |= FLASH_IFREN;
        }
        EEPROM_CNTR->CMD = flash_cmd;
        
        EEPROM_CNTR->ADR = (unsigned)0;
        
        flash_cmd |= FLASH_XE | FLASH_MAS1 | FLASH_ERASE;
        EEPROM_CNTR->CMD = flash_cmd;
        delay(5);
        flash_cmd |= FLASH_NVSTR;
        EEPROM_CNTR->CMD = flash_cmd;
        delay(40000);
        flash_cmd &= ~FLASH_ERASE;
        EEPROM_CNTR->CMD = flash_cmd;
        delay(5);
        flash_cmd &= ~(FLASH_XE | FLASH_MAS1 | FLASH_NVSTR);
        EEPROM_CNTR->CMD = flash_cmd;
        
        flash_cmd &= FLASH_DELAY_MASK;
        EEPROM_CNTR->CMD = flash_cmd;
        EEPROM_CNTR->KEY = 0;
        
        
        return RESULT_ERASE_DONE;
    }
    
//    clock enabled im .mac file  MDR_RCC->PER2_CLOCK |= RCC_PER2_CLOCK_EEPROMEN;
    
    return RESULT_OK;
}

#ifdef DEBUG
const char Placeholder[] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";

void main(int argc, char const *argv[])
{
    MDR_RCC->PER2_CLOCK |= RCC_PER2_CLOCK_EEPROMEN;
    MDR_RCC->CPU_CLOCK &=~(3<<8);
    NOP;
    FlashInit(0, 0, 0, 0, argc, argv);
    NOP;
    FlashErase((void *)0, 0x1000);
    FlashWrite((void *)0, 0, sizeof(Placeholder), Placeholder);
    NOP;
    FlashErase((void *)0x1000, 0x1000);
    FlashWrite((void *)0x1000, 0, sizeof(Placeholder)-4, Placeholder+4);
    NOP;
    FlashErase((void *)0x0000, 0x2000);
    FlashWrite((void *)0x3000, 0, sizeof(Placeholder)-8, Placeholder+8);
    NOP;
    FlashInit(0, 0, 0, FLAG_ERASE_ONLY, argc, argv);
    NOP;
}
#endif

uint32_t FlashWrite(void *block_start,
                    uint32_t offset_into_block,
                    uint32_t count,
                    char const *buffer)
{
    register uint32_t address;
    register uint32_t *buf;
    register uint32_t mem_type;
    register uint32_t flash_cmd;
    
    address = (unsigned)block_start + offset_into_block;
    buf = (uint32_t *)buffer;
    count = (count + 3) & ~3; // Adjust size
    
    
    mem_type = 0;
    
    EEPROM_CNTR->KEY = 0x8AAA5551;
    flash_cmd = EEPROM_CNTR->CMD;
    
    // Switch on register access 
    flash_cmd = (flash_cmd & FLASH_DELAY_MASK) | FLASH_CON | FLASH_TMR;
    if (mem_type)
    {
        flash_cmd |= FLASH_IFREN;
    }
    EEPROM_CNTR->CMD = flash_cmd;
    
    
    while (count > 0)
    {
        EEPROM_CNTR->ADR = address;
        EEPROM_CNTR->DI = *buf;
        
        flash_cmd |= FLASH_XE | FLASH_PROG;
        EEPROM_CNTR->CMD = flash_cmd;
        delay(5);
        flash_cmd |= FLASH_NVSTR;
        EEPROM_CNTR->CMD = flash_cmd;
        delay(10);
        flash_cmd |= FLASH_YE;
        EEPROM_CNTR->CMD = flash_cmd;
        
        delay(40);
        
        flash_cmd &= ~FLASH_YE;
        EEPROM_CNTR->CMD = flash_cmd;
        
        flash_cmd &= ~FLASH_PROG;
        EEPROM_CNTR->CMD = flash_cmd;
        delay(5);
        flash_cmd &= ~(FLASH_XE | FLASH_NVSTR);
        EEPROM_CNTR->CMD = flash_cmd;
        delay(10);
        
        buf += 1;
        address += 4;
        count -= 4;
    }
    
    flash_cmd &= FLASH_DELAY_MASK;
    EEPROM_CNTR->CMD = flash_cmd;
    EEPROM_CNTR->KEY = 0;
    
    return 0;
}

uint32_t FlashErase(void *block_start, uint32_t block_size)
{
    register uint32_t mem_type;
    register uint32_t flash_cmd;
    
    mem_type = 0;
    
    EEPROM_CNTR->KEY = 0x8AAA5551; 
    flash_cmd = EEPROM_CNTR->CMD;
    
    /* Switch on register access */
    flash_cmd = (flash_cmd & FLASH_DELAY_MASK) | FLASH_CON | FLASH_TMR;
    if (mem_type)
    {
        flash_cmd |= FLASH_IFREN;
    }
    EEPROM_CNTR->CMD = flash_cmd;
    
    while (block_size > 0)
    {
        EEPROM_CNTR->ADR = (unsigned)block_start;
        
        flash_cmd |= FLASH_XE | FLASH_ERASE;
        EEPROM_CNTR->CMD = flash_cmd;
        delay(5);
        flash_cmd |= FLASH_NVSTR;
        EEPROM_CNTR->CMD = flash_cmd;
        delay(40000);
        flash_cmd &= ~FLASH_ERASE;
        EEPROM_CNTR->CMD = flash_cmd;
        delay(5);
        flash_cmd &= ~(FLASH_XE | FLASH_NVSTR);
        EEPROM_CNTR->CMD = flash_cmd;
        
        block_start = (void *) ((unsigned)block_start + 0x1000);
        block_size -= 0x1000;
    }
    
    flash_cmd &= FLASH_DELAY_MASK;
    EEPROM_CNTR->CMD = flash_cmd;
    EEPROM_CNTR->KEY = 0;
    
    return 0;
}

OPTIONAL_CHECKSUM
uint32_t FlashChecksum(void const *begin, uint32_t count)
{
    return Crc16((uint8_t const *)begin, count);
}

OPTIONAL_SIGNOFF
uint32_t FlashSignoff(void)
{
    return 0;
}


