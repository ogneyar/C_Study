
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
    // const char * fileName = "temp.txt";
    // const char * fileName = "test.txt";
    // const char * fileName = "0001.bmp";
    // const char * fileName = "0001.txt";
    // const char * fileName = "ttt.txt";
    const char * fileName = "eee.txt";


    /*
        запись не работает !

        если в файле Sd2Card.cpp в методе Sd2Card::writeData (на 691-692 строках) 
        закомментировать chipSelectHigh(); и return false;
        тогда что-то в файл запишется, но это не выход

        решение пока ненайдено :(
    */

    // const uint8_t * dataString = (const uint8_t *)"zzz";
    // // dataFile = SD.open(fileName, FILE_WRITE);
    // dataFile = SD.open(fileName, O_WRITE | O_APPEND);
    // if (dataFile) {
    //     printf("SD open file for write\r\n");
    //     dataFile.write(dataString, sizeof(dataString));
    //     // dataFile.write((const uint8_t *)'z', 1);
    //     dataFile.close();
    // }else printf("error opening %s\r\n", fileName);     


    /* 
        чтение работает

        это побайтовое чтение
    */

    // int ch = -1;
    // char *str;
    // uint8_t i = 0;
    // dataFile = SD.open(fileName);
    // if (dataFile) {
    //     printf("SD open file for read\r\n");
    //     ch = dataFile.read();
    //     while (ch != -1) {
    //         printf("%c", ch);
    //         str[i++] = ch;
    //         ch = dataFile.read();
    //     }
    //     str[i] = '\0';
    //     printf("\r\n");
    //     printf("sizeof(str): %d\r\n", (u8)sizeof(str));
    //     printf("str[2]: %c\r\n", (char)str[2]);
    //     dataFile.close();
    // }else printf("error opening %s\r\n", fileName);


    /* 
        чтение работает

        а это чтение в буфер
    */

    uint8_t	buffer[500];
    dataFile = SD.open(fileName);
    if (dataFile) {
        printf("SD open file for read\r\n");
        dataFile.read(buffer, sizeof(buffer));
        printf((const char*)buffer);
        printf("\r\n");
        dataFile.close();
    }else printf("error opening %s\r\n", fileName);


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



