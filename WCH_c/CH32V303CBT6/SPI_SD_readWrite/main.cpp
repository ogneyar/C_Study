
#include "debug.h"
#include <ch32v30x.h>
#include <string>
#include "SD.h"

#define LED_R       3 // PB3
#define LED_G       4 // PB4
#define LED_B       5 // PB5

// #define DEBUG_SD 1

//Список функций
void GPIO_Leds_Init(void);
void Leds_Blink(uint16_t delay);

const int chipSelect = 0;


// Основная функция
int main(void)
{
    Delay_Init();    
    USART_Printf_Init(115200);
    printf("SystemClk: %d\r\n", SystemCoreClock);    
    GPIO_Leds_Init();
    
	if ( ! SD.begin(chipSelect)) {		
		printf("Error SD begin\r\n");
		printf("card.Error: %d\r\n", SD.card.errorCode());
		printf("card.Status: %d\r\n", SD.card.errorData());
	  	while (1);
	} else {
		printf("SD begin is correct\r\n");
	}

    File dataFile;
    const char * fileName = "temp.txt";
    // const char * fileName = "0001.bmp";

    // const uint8_t * dataString = (const uint8_t *)"dataString";
    // dataFile = SD.open(fileName, FILE_WRITE);
    // if (dataFile) {
    //     printf("SD open file for write\r\n");
    //     dataFile.write(dataString, sizeof(dataString));
    // }else printf("error opening %s\r\n", fileName);     

    int ch = -1;
    char *str;
    uint8_t i = 0;
    dataFile = SD.open(fileName);
    if (dataFile) {
        printf("SD open file for read\r\n");
        ch = dataFile.read();
        while (ch != -1) {
            printf("%c", ch);
            str[i++] = ch;
            ch = dataFile.read();
        }
        str[i] = '\0';
        printf("\r\n");
        printf("sizeof(str): %d\r\n", (u8)sizeof(str));
        printf("str[2]: %c\r\n", (char)str[2]);
    }else printf("error opening %s\r\n", fileName);


    dataFile.close();


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



