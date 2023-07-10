
#include "debug.h"
#include <ch32v30x.h>
#include "Sd2Card.h"
#include "SdFat.h"


#define LED_R       3 // PB3
#define LED_G       4 // PB4
#define LED_B       5 // PB5


// #define DEBUG_SD 1


//Список функций
void GPIO_Leds_Init(void);
void Leds_Blink(uint16_t delay);
void Print_CardType(void);
void Print_VolumeInfo(void);

Sd2Card card;
SdVolume volume;
SdFile root;

const int chipSelect = 0;

// Основная функция
int main(void)
{
    Delay_Init();    
    USART_Printf_Init(115200);
    printf("SystemClk: %d\r\n", SystemCoreClock);    
    GPIO_Leds_Init();
    
	if ( ! card.init(chipSelect)) {		
		printf("Error card init\r\n");
		printf("card.Error: %d\r\n", card.errorCode());
		printf("card.Status: %d\r\n", card.errorData());
	  	while (1);
	} else {
		printf("Card init is correct\r\n");
	}
	
	// print the type of card
	Print_CardType();
	
	// Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
	if ( ! volume.init(card)) {
		printf("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card\r\n");
		while (1);
	}

	Print_VolumeInfo();

	printf("\nFiles found on the card (name, date and size in bytes): \r\n");
	root.openRoot(volume);

	// list all files in the card with date and size
	root.ls(LS_R | LS_DATE | LS_SIZE);
	root.close();

    while(1) 
    {
        Leds_Blink(300);
    }
}


// Инициализация PB3, PB4, PB5
void GPIO_Leds_Init(void)
{
    // включаем тактирование порта B
    RCC->APB2PCENR |= RCC_APB2Periph_GPIOB;
    // PB3
    GPIOB->CFGLR &= ~(0b1111 << LED_R*4); // GPIO_Msk
    GPIOB->CFGLR |= (0b0011 << LED_R*4); // GPIO_Speed_50
    // PB4
    GPIOB->CFGLR &= ~(0b1111 << LED_G*4); // GPIO_Msk
    GPIOB->CFGLR |= (0b0011 << LED_G*4); // GPIO_Speed_50
    // PB5
    GPIOB->CFGLR &= ~(0b1111 << LED_B*4); // GPIO_Msk
    GPIOB->CFGLR |= (0b0011 << LED_B*4); // GPIO_Speed_50

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


// print the type of card
void Print_CardType(void)
{       
	printf("Card type:         ");
	switch (card.type()) {
		case SD_CARD_TYPE_SD1:
			printf("SD1\r\n");
		break;
		case SD_CARD_TYPE_SD2:
			printf("SD2\r\n");
		break;
		case SD_CARD_TYPE_SDHC:
			printf("SDHC\r\n");
		break;
		default:
			printf("Unknown\r\n");
	}
}


// 
void Print_VolumeInfo(void)
{    
    printf("Clusters:           %d\r\n", volume.clusterCount());
    printf("Blocks x Cluster:   %d\r\n", volume.blocksPerCluster());

    printf("Total Blocks:       %d\r\n\r\n", volume.blocksPerCluster() * volume.clusterCount());

    uint32_t volumesize;
    printf("Volume type is:     FAT%d\r\n", volume.fatType());

    volumesize = volume.blocksPerCluster(); 
    volumesize *= volume.clusterCount();
    volumesize /= 2; // SD card blocks are always 512 bytes (2 blocks are 1 KB)
    printf("Volume size (KB):   %d\r\n", volumesize);
    volumesize /= 1024;
    printf("Volume size (MB):   %d\r\n", volumesize);
    volumesize = (float)volumesize / 1024.0;
    if (volumesize) printf("Volume size (GB):   %d\r\n", volumesize);
    else printf("Volume size (GB):   %d\r\n", 1);
}

