
#include <stdint.h>
#include <stdio.h>
#include "main.h"
#include "defines.h"
#include "system.h"
#include "delay.h"
#include "usart.h"
#include "spi.h"
#include "sd_card.h"


// Основная функция
int main(void)
{
    Delay_Init();
    GPIO_Init();
    USART1_Init(115200);
    printf("SystemClk: %d\r\n", SystemCoreClock);
    printf("SPI SDcard\r\n");
    
    /*
    если SystemCoreClock = HSI_VALUE, тогда SPI_BaudRatePrescaler_2
    если SystemCoreClock = SYSCLK_FREQ_48MHz_HSI, тогда SPI_BaudRatePrescaler_4
    если SystemCoreClock = SYSCLK_FREQ_144MHz_HSI, тогда SPI_BaudRatePrescaler_8    
    */

    SD_Init_TypeDef init = SD_Init(GPIOA, PA0);

    if (init.errorCode) {
        // while(1) ;
        printf("init error: %d\r\n", init.errorCode);
    }
    
    if (init.status) printf("init status: %d\r\n", init.status);
    if (init.type) {
        // printf("init type: %d\r\n", init.type);
        switch (init.type) {
            case 1:
                printf("SD_CARD_TYPE: SD1\r\n");
            break;
            case 2:
                printf("SD_CARD_TYPE: SD2\r\n");
            break;            
            case 3:
                printf("SD_CARD_TYPE: SDHC\r\n");
            break;
            default:
                printf("SD_CARD_TYPE: udefined\r\n");
            break;
        }
    }

    if ( ! SDvolume_init() ) {
        printf("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card\r\n");
        // while (1);
    }

    // Print_VolumeInfo();

    SDfile_openRoot();
    SDfile_ls(LS_R | LS_DATE | LS_SIZE, 0);
    SDfile_close();
      
    while(1)
    {
        Test_Screen();
    }
}

// Инициализация PB3, PB4, PB5
void GPIO_Init(void)
{
    // включаем тактирование порта B
    RCC->APB2PCENR |= RCC_APB2PCENR_IOPBEN;
    // PB3
    GPIOB->CFGLR &= ~(GPIO_Msk << LED_R*4); // ~(0b1111);
    GPIOB->CFGLR |= (GPIO_Speed_50 << LED_R*4); // 0b0011;
    // PB4
    GPIOB->CFGLR &= ~(GPIO_Msk << LED_G*4); // ~(0b1111);
    GPIOB->CFGLR |= (GPIO_Speed_50 << LED_G*4); // 0b0011;
    // PB5
    GPIOB->CFGLR &= ~(GPIO_Msk << LED_B*4); // ~(0b1111);
    GPIOB->CFGLR |= (GPIO_Speed_50 << LED_B*4); // 0b0011;

    GPIOB->BSHR |= (1 << LED_G);
}

// мигание светодиодов
void Leds_Blink(uint16_t delay)
{    
    GPIOB->OUTDR ^= (1 << LED_R);
    GPIOB->OUTDR ^= (1 << LED_G);
    GPIOB->OUTDR ^= (1 << LED_B);
    Delay_Ms(delay);
}

// тест дисплея
void Test_Screen(void) 
{    
    uint16_t delay = 500;
    
	Leds_Blink(delay);
}

// 
void Print_VolumeInfo(void)
{    
    printf("Clusters:           %d\r\n", SDvolume_clusterCount_);
    printf("Blocks x Cluster:   %d\r\n", SDvolume_blocksPerCluster_);

    printf("Total Blocks:       %d\r\n\r\n", SDvolume_blocksPerCluster_ * SDvolume_clusterCount_);

    uint32_t volumesize;
    printf("Volume type is:     FAT%d\r\n", SDvolume_fatType_);

    volumesize = SDvolume_blocksPerCluster_; 
    volumesize *= SDvolume_clusterCount_;
    volumesize /= 2; // SD card blocks are always 512 bytes (2 blocks are 1 KB)
    printf("Volume size (KB):   %d\r\n", volumesize);
    volumesize /= 1024;
    printf("Volume size (MB):   %d\r\n", volumesize);
    volumesize = (float)volumesize / 1024.0;
    if (volumesize) printf("Volume size (GB):   %d\r\n", volumesize);
    else printf("Volume size (GB):   %d\r\n", 1);
}

