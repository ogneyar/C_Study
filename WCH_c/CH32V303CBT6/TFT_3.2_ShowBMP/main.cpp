
#include "debug.h"
#include <ch32v30x.h>
#include "SD.h"
#include "tft_shield_3_2.h"

#define LED_R       3 // PB3
#define LED_G       4 // PB4
#define LED_B       5 // PB5

// #define __DEBUG     1

#define BUFFPIXEL   80
#define WIDTH       240
#define HEIGHT      320

//Список функций
void GPIO_Leds_Init(void);
void Leds_Blink(uint16_t delay);
void Test_Screen(void);
void Bmp_Carousel(void);
void bmpDraw(char *filename, int x = 0, int y = 0);
uint16_t read16(File f);
uint32_t read32(File f);


const int chipSelect = 0;
uint8_t test = 42;


// Основная функция
int main(void)
{
    Delay_Init();    
    USART_Printf_Init(115200);
    printf("SystemClk: %d\r\n", SystemCoreClock);    
    GPIO_Leds_Init();
        
    Lcd_Init();
    Lcd_Fill(0xffff);
    
	if ( ! SD.begin(chipSelect)) {		
		printf("Error SD begin\r\n");
		printf("card.Error: %d\r\n", SD.card.errorCode());
		printf("card.Status: %d\r\n", SD.card.errorData());
	  	// while (1);
	} else {
		printf("SD begin is correct\r\n");
	}
        
    while(1) 
    {
        test++;
        // Test_Screen();
        Bmp_Carousel();
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


// тест дисплея
void Test_Screen(void) 
{    
    uint16_t delay = 500;
    
    Lcd_Fill(0xf800);
	Leds_Blink(delay);
    Lcd_Fill(0x07E0);
	Leds_Blink(delay);
    Lcd_Fill(0x001F);
	Leds_Blink(delay);
    Lcd_Clear();         
	Leds_Blink(delay);
}


// карусель картинок
void Bmp_Carousel(void)
{    
    uint16_t delay = 500;
    
    bmpDraw("0001.bmp");
	Leds_Blink(delay);
	Leds_Blink(delay);
	Leds_Blink(delay);
	Leds_Blink(delay);
    bmpDraw("0002.bmp");
	Leds_Blink(delay);
	Leds_Blink(delay);
	Leds_Blink(delay);
	Leds_Blink(delay);
    bmpDraw("0003.bmp");
	Leds_Blink(delay);
	Leds_Blink(delay);
	Leds_Blink(delay);
	Leds_Blink(delay);
    bmpDraw("0004.bmp");
	Leds_Blink(delay);
	Leds_Blink(delay);
	Leds_Blink(delay);
	Leds_Blink(delay);
    bmpDraw("0005.bmp");
	Leds_Blink(delay);
	Leds_Blink(delay);
	Leds_Blink(delay);
	Leds_Blink(delay);
    bmpDraw("0006.bmp");
	Leds_Blink(delay);
	Leds_Blink(delay);
	Leds_Blink(delay);
	Leds_Blink(delay);
    bmpDraw("0007.bmp");
	Leds_Blink(delay);
	Leds_Blink(delay);
	Leds_Blink(delay);
	Leds_Blink(delay);
    bmpDraw("0008.bmp");
	Leds_Blink(delay);
	Leds_Blink(delay);
	Leds_Blink(delay);
	Leds_Blink(delay);
    bmpDraw("0009.bmp");
	Leds_Blink(delay);
	Leds_Blink(delay);
	Leds_Blink(delay);
	Leds_Blink(delay);
    bmpDraw("0010.bmp");
	Leds_Blink(delay);
	Leds_Blink(delay);
	Leds_Blink(delay);
	Leds_Blink(delay);
    bmpDraw("0011.bmp");
	Leds_Blink(delay);
	Leds_Blink(delay);
	Leds_Blink(delay);
	Leds_Blink(delay);
    bmpDraw("0012.bmp");
	Leds_Blink(delay);
	Leds_Blink(delay);
	Leds_Blink(delay);
	Leds_Blink(delay);
    bmpDraw("0013.bmp");
	Leds_Blink(delay);
	Leds_Blink(delay);
	Leds_Blink(delay);
	Leds_Blink(delay);
    bmpDraw("0014.bmp");
	Leds_Blink(delay);
	Leds_Blink(delay);
	Leds_Blink(delay);
	Leds_Blink(delay);
    bmpDraw("0015.bmp");
	Leds_Blink(delay);
	Leds_Blink(delay);
	Leds_Blink(delay);
	Leds_Blink(delay);
    bmpDraw("0016.bmp");
	Leds_Blink(delay);
	Leds_Blink(delay);
	Leds_Blink(delay);
	Leds_Blink(delay);
    bmpDraw("0017.bmp");
	Leds_Blink(delay);
	Leds_Blink(delay);
	Leds_Blink(delay);
	Leds_Blink(delay);
    bmpDraw("0018.bmp");
	Leds_Blink(delay);
	Leds_Blink(delay);
	Leds_Blink(delay);
	Leds_Blink(delay);
    bmpDraw("0019.bmp");
	Leds_Blink(delay);
	Leds_Blink(delay);
	Leds_Blink(delay);
	Leds_Blink(delay);
    bmpDraw("0020.bmp");
	Leds_Blink(delay);
	Leds_Blink(delay);
	Leds_Blink(delay);
	Leds_Blink(delay);
    bmpDraw("0021.bmp");
	Leds_Blink(delay);
	Leds_Blink(delay);
	Leds_Blink(delay);
	Leds_Blink(delay);
    bmpDraw("0022.bmp");
	Leds_Blink(delay);
	Leds_Blink(delay);
	Leds_Blink(delay);
	Leds_Blink(delay);
    bmpDraw("0023.bmp");
	Leds_Blink(delay);
	Leds_Blink(delay);
	Leds_Blink(delay);
	Leds_Blink(delay);
    bmpDraw("0024.bmp");
	Leds_Blink(delay);
	Leds_Blink(delay);
	Leds_Blink(delay);
	Leds_Blink(delay);
    bmpDraw("0025.bmp");
	Leds_Blink(delay);
	Leds_Blink(delay);
	Leds_Blink(delay);
	Leds_Blink(delay);
}


void bmpDraw(char *filename, int x, int y)
{
    File 	bmpFile;
    int		bmpWidth, bmpHeight;   // W+H in pixels
    uint8_t	bmpDepth;			   // Bit depth (currently must be 24)
    uint32_t bmpImageoffset; 	   // Start of image data in file
    uint32_t rowSize;			   // Not always = bmpWidth; may have padding
    uint8_t	sdbuffer[3*BUFFPIXEL]; // pixel in buffer (R+G+B per pixel)
    uint16_t lcdbuffer[BUFFPIXEL];  // pixel out buffer (16-bit per pixel)
    uint8_t	buffidx = sizeof(sdbuffer); // Current position in sdbuffer
    bool	goodBmp = false;	   // Set to true on valid header parse
    bool	flip	= true; 	   // BMP is stored bottom-to-top
    int		w, h, row, col;
    uint8_t	r, g, b;
    uint32_t pos = 0;
    uint8_t	lcdidx = 0;
    
    if((x >= WIDTH) || (y >= HEIGHT)) return;

#ifdef __DEBUG
    printf("\r\nLoading image '%s'\r\n", filename);
#endif

    SPI1->CTLR1 &= ~SPI_BaudRatePrescaler_2;
    SPI1->CTLR1 |= SPI_BaudRatePrescaler_8;

    if ( ! (bmpFile = SD.open(filename))) {
#ifdef __DEBUG
        printf("File not found\r\n");
#endif
        return;
    }
    
    // Parse BMP header
    if(read16(bmpFile) == 0x4D42) { // BMP signature
#ifdef __DEBUG
        printf("File size: %d\r\n", read32(bmpFile));
#else
        (void)read32(bmpFile);
#endif
        (void)read32(bmpFile); // Read & ignore creator bytes
        bmpImageoffset = read32(bmpFile); // Start of image data
#ifdef __DEBUG
        printf("Image Offset: %d\r\n", bmpImageoffset);
        // Read DIB header
        printf("Header size: %d\r\n", read32(bmpFile));
#else
        (void)read32(bmpFile);
#endif
        
        bmpWidth  = read32(bmpFile);
        bmpHeight = read32(bmpFile);

        if(read16(bmpFile) == 1) { // # planes -- must be '1'
            bmpDepth = read16(bmpFile); // bits per pixel
#ifdef __DEBUG
            printf("Bit Depth: %d\r\n", bmpDepth);
#endif
            if((bmpDepth == 24) && (read32(bmpFile) == 0)) { // 0 = uncompressed
    
                goodBmp = true; // Supported BMP format -- proceed!

#ifdef __DEBUG
                printf("Image size: %d x %d\r\n", bmpWidth, bmpHeight);
#endif
    
                // BMP rows are padded (if needed) to 4-byte boundary
                rowSize = (bmpWidth * 3 + 3) & ~3;
        
                // If bmpHeight is negative, image is in top-down order.
                // This is not canon but has been observed in the wild.
                if(bmpHeight < 0) {
                    bmpHeight = -bmpHeight;
                    flip 	 = false;
                }
        
                // Crop area to be loaded
                w = bmpWidth;
                h = bmpHeight;
                if((x+w-1) >= WIDTH)  w = WIDTH - x;
                if((y+h-1) >= HEIGHT) h = HEIGHT - y;
        

                SPI1->CTLR1 &= ~SPI_BaudRatePrescaler_8;
                SPI1->CTLR1 |= SPI_BaudRatePrescaler_2;

                // Set TFT address window to clipped image bounds
                GPIOA->BCR |= DD_CS;
                Lcd_Address_Set(x, y, x+w-1, y+h-1);
                GPIOA->BSHR |= DD_CS;
        
                for (row=0; row<h; row++) { // For each scanline...
                    // Seek to start of scan line.  It might seem labor-
                    // intensive to be doing this on every line, but this
                    // method covers a lot of gritty details like cropping
                    // and scanline padding.  Also, the seek only takes
                    // place if the file position actually needs to change
                    // (avoids a lot of cluster math in SD library).
                    if(flip) // Bitmap is stored bottom-to-top order (normal BMP)
                        pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
                    else 	// Bitmap is stored top-to-bottom
                        pos = bmpImageoffset + row * rowSize;                        

                    SPI1->CTLR1 &= ~SPI_BaudRatePrescaler_2;
                    SPI1->CTLR1 |= SPI_BaudRatePrescaler_8;

                    if(bmpFile.position() != pos) { // Need seek?
                        bmpFile.seek(pos);
                        buffidx = sizeof(sdbuffer); // Force buffer reload
                    }
        
                    for (col=0; col<w; col++) { // For each column...
                        // Time to read more pixel data?
                        if (buffidx >= sizeof(sdbuffer)) { // Indeed
                            // Push LCD buffer to the display first
                            if(lcdidx > 0) {
                                
                                SPI1->CTLR1 &= ~SPI_BaudRatePrescaler_8;
                                SPI1->CTLR1 |= SPI_BaudRatePrescaler_2;

                                Lcd_PushColors(lcdbuffer, lcdidx);
                                lcdidx = 0;
                            }

                            SPI1->CTLR1 &= ~SPI_BaudRatePrescaler_2;
                            SPI1->CTLR1 |= SPI_BaudRatePrescaler_8;

                            bmpFile.read(sdbuffer, sizeof(sdbuffer));     

#ifdef __DEBUG
                            // printf("sdbuffer: %d\r\n", (const char *)sdbuffer);
                            // printf("sdbuffer[0]: 0x%x\r\n", sdbuffer[0]);
#endif
                            buffidx = 0; // Set index to beginning
                        }
            
                        // Convert pixel from BMP to TFT format
                        b = sdbuffer[buffidx++];
                        g = sdbuffer[buffidx++];
                        r = sdbuffer[buffidx++];
                        lcdbuffer[lcdidx++] = Lcd_Color565(r,g,b);
                    } // end pixel
                } // end scanline
                // Write any remaining data to LCD
                if(lcdidx > 0) {

                    SPI1->CTLR1 &= ~SPI_BaudRatePrescaler_8;
                    SPI1->CTLR1 |= SPI_BaudRatePrescaler_2;

                    Lcd_PushColors(lcdbuffer, lcdidx);
                }
                
            } // end goodBmp
        }
    }
    
    SPI1->CTLR1 &= ~SPI_BaudRatePrescaler_2;
    SPI1->CTLR1 |= SPI_BaudRatePrescaler_8;

    bmpFile.close();
// #ifdef __DEBUG
    if(!goodBmp) printf("BMP format not recognized.\r\n");
// #endif
}

uint16_t read16(File f)
{
    uint16_t result;
    ((uint8_t *)&result)[0] = f.read(); // LSB
    ((uint8_t *)&result)[1] = f.read(); // MSB
    return result;
}
 
uint32_t read32(File f)
{
    uint32_t result;
    ((uint8_t *)&result)[0] = f.read(); // LSB
    ((uint8_t *)&result)[1] = f.read();
    ((uint8_t *)&result)[2] = f.read();
    ((uint8_t *)&result)[3] = f.read(); // MSB
    return result;
}
 
