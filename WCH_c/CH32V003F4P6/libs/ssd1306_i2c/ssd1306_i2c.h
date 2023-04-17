
#ifndef _SSD1306_I2C_H_
#define _SSD1306_I2C_H_

#define OLED_WIDTH              128
#define OLED_HEIGHT_32          0x02
#define OLED_HEIGHT_64          0x12
#define OLED_64                 0x3F
#define OLED_32                 0x1F

#define OLED_DISPLAY_OFF        0xAE
#define OLED_DISPLAY_ON         0xAF

#define OLED_COMMAND_MODE       0x00
#define OLED_ONE_COMMAND_MODE   0x80
#define OLED_DATA_MODE          0x40
#define OLED_ONE_DATA_MODE      0xC0

#define OLED_ADDRESSING_MODE    0x20
#define OLED_HORIZONTAL         0x00
#define OLED_VERTICAL           0x01

#define OLED_NORMAL_V           0xC8
#define OLED_FLIP_V             0xC0
#define OLED_NORMAL_H           0xA1
#define OLED_FLIP_H             0xA0

#define OLED_CONTRAST           0x81
#define OLED_SETCOMPINS         0xDA
#define OLED_SETVCOMDETECT      0xDB
#define OLED_CLOCKDIV           0xD5
#define OLED_SETMULTIPLEX       0xA8
#define OLED_COLUMNADDR         0x21
#define OLED_PAGEADDR           0x22
#define OLED_CHARGEPUMP         0x8D

#define OLED_NORMALDISPLAY      0xA6
#define OLED_INVERTDISPLAY      0xA7


#define Size          512
#define TxAdderss     0x3c
#define addressWrite (TxAdderss << 1)


void SSD1306_Init(void);
void SSD1306_Write(uint8_t mode, uint8_t data);
void SSD1306_WriteArray(uint8_t * data, uint16_t length);
void SSD1306_SetColumnAndRow(void);
void SSD1306_ClearScreen(void);
void SSD1306_Fill(uint8_t data);
void SSD1306_TestScreen(void);


#endif /* _SSD1306_I2C_H_ */