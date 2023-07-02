

// НЕ РАБОТАЕТ!!!!!!!


#include <stdint.h>
#include <stdio.h>
#include "main.h"
#include "defines.h"
#include "system.h"
#include "delay.h"
#include "usart.h"
#include "spi.h"
#include "tft_defines.h"
#include "tft.h"

// #define __DEBUG 1


// Основная функция
int main(void)
{
    Delay_Init();
    GPIO_Init();
    // USART1_Init(115200);
    // printf("SystemClk:%d\r\n", SystemCoreClock);
    // printf("SPI TFT SHIELD 2.4inch\r\n");

    // SPI1_Master_Init(0);    
    // Delay_Ms(100);
      
    // инициализация дисплея
    // tft_begin(); // SPFD5408 can use ILI9320 driver (0x9320)
    _width = WIDTH; // 240;
    _height = HEIGHT; // 320;

    _lcd_capable = 0 | REV_SCREEN | READ_BGR;

    init_table16(ILI9320_regValues, sizeof(ILI9320_regValues)); 
    _lcd_rev = ((_lcd_capable & REV_SCREEN) != 0);
    setRotation(0);             //PORTRAIT
    invertDisplay(0);

    // Delay_Ms(1000);
    
    // начальное меню
    // show_tft();    
    fillScreen(BLACK);
    drawRectWide(90, 160, 40, 60, WHITE);

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
    // // PB4
    // GPIOB->CFGLR &= ~(GPIO_Msk << LED_G*4); // ~(0b1111);
    // GPIOB->CFGLR |= (GPIO_Speed_50 << LED_G*4); // 0b0011;
    // PB5
    GPIOB->CFGLR &= ~(GPIO_Msk << LED_B*4); // ~(0b1111);
    GPIOB->CFGLR |= (GPIO_Speed_50 << LED_B*4); // 0b0011;

    // GPIOB->BSHR |= (1 << LED_G);
    GPIOB->BSHR |= (1 << LED_R);
    
    // включаем тактирование порта A
    RCC->APB2PCENR |= RCC_APB2PCENR_IOPAEN;
    
    // reset();
    // setWriteDir();
    GPIOB->CFGLR &= ~(GPIO_Msk << PB4*4); 
    GPIOB->CFGLR |= (GPIO_Speed_50 << PB4*4); 
    GPIOB->CFGLR &= ~(GPIO_Msk << PB6*4); 
    GPIOB->CFGLR |= (GPIO_Speed_50 << PB6*4); 
    GPIOB->CFGLR &= ~(GPIO_Msk << PB7*4); 
    GPIOB->CFGLR |= (GPIO_Speed_50 << PB7*4); 
    GPIOB->CFGHR &= ~(GPIO_Msk << (PB8-8)*4); 
    GPIOB->CFGHR |= (GPIO_Speed_50 << (PB8-8)*4); 
    GPIOB->CFGHR &= ~(GPIO_Msk << (PB9-8)*4); 
    GPIOB->CFGHR |= (GPIO_Speed_50 << (PB9-8)*4); 
    GPIOB->CFGHR &= ~(GPIO_Msk << (PB12-8)*4); 
    GPIOB->CFGHR |= (GPIO_Speed_50 << (PB12-8)*4); 
    GPIOB->CFGHR &= ~(GPIO_Msk << (PB13-8)*4); 
    GPIOB->CFGHR |= (GPIO_Speed_50 << (PB13-8)*4); 
    GPIOB->CFGHR &= ~(GPIO_Msk << (PB15-8)*4); 
    GPIOB->CFGHR |= (GPIO_Speed_50 << (PB15-8)*4); 
    // CTL_INIT();
    GPIOA->CFGLR &= ~(GPIO_Msk << PA0*4); 
    GPIOA->CFGLR |= (GPIO_Speed_50 << PA0*4); 
    GPIOA->CFGLR &= ~(GPIO_Msk << PA1*4); 
    GPIOA->CFGLR |= (GPIO_Speed_50 << PA1*4); 
    GPIOA->CFGLR &= ~(GPIO_Msk << PA2*4); 
    GPIOA->CFGLR |= (GPIO_Speed_50 << PA2*4); 
    GPIOA->CFGLR &= ~(GPIO_Msk << PA3*4); 
    GPIOA->CFGLR |= (GPIO_Speed_50 << PA3*4); 
    // CS_IDLE;
    GPIOB->BSHR |= (1 << PA3);
    // RD_IDLE;
    GPIOB->BSHR |= (1 << PA0);
    // WR_IDLE;    
    GPIOB->BSHR |= (1 << PA1);

}

// мигание светодиодов
void Leds_Blink(uint16_t delay)
{    
    GPIOB->OUTDR ^= (1 << LED_R);
    // GPIOB->OUTDR ^= (1 << LED_G);
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
void show_tft(void)
{
  
//   fillScreen(BLACK);
//   drawRectWide(90, 160, 40, 60, WHITE);
  
//   setCursor(100, 185);
//   tft_print("Run");
  
//   while (1) {    
//     tp = ts.getPoint();
//     pinMode(XM, OUTPUT);
//     pinMode(YP, OUTPUT);
//     pinMode(XP, OUTPUT);
//     pinMode(YM, OUTPUT);
//     if (tp.z < MINPRESSURE || tp.z > MAXPRESSURE) continue;
//     if (tp.x > 450 && tp.x < 690  && tp.y > 450 && tp.y < 570) break;
//   }
  
//   BOXSIZE = _width / 8;
//   fillScreen(BLACK);  
//   // блок кнопок (выход, очистка экрана, ...)
//   buttons();
  
//   fillRect(0, 0, BOXSIZE, BOXSIZE, WHITE);
//   fillRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, RED);
//   fillRect(BOXSIZE * 2, 0, BOXSIZE, BOXSIZE, ORANGE);
//   fillRect(BOXSIZE * 3, 0, BOXSIZE, BOXSIZE, YELLOW);
//   fillRect(BOXSIZE * 4, 0, BOXSIZE, BOXSIZE, GREEN);
//   fillRect(BOXSIZE * 5, 0, BOXSIZE, BOXSIZE, CYAN);
//   fillRect(BOXSIZE * 6, 0, BOXSIZE, BOXSIZE, BLUE);
//   fillRect(BOXSIZE * 7, 0, BOXSIZE, BOXSIZE, MAGENTA);

//   drawRectWide(0, 0, BOXSIZE, BOXSIZE, BLACK);
//   currentcolor = WHITE;
  // delay(1000);
}

//
void buttons(void)
{  
//   drawRectWide(0, 280, 40, 60, WHITE);  
//   setCursor(190, 305);
//   print("End");
//   drawRectWide(60, 290, 30, 120, WHITE);
//   setCursor(85, 310);
//   print("CLEAR");
//   drawRectWide(180, 280, 40, 60, WHITE);
//   setCursor(10, 305);
//   print("Bmp");
}




