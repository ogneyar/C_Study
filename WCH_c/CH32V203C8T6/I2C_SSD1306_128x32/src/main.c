
/*
 *@Note
 I2C interface routine to operate LCD:
 I2C1_SCL(PB10)\I2C1_SDA(PB11).
 This example uses LCD SSD1306. 
*/

#include "debug.h"
#include "stdio.h"

#define SCREEN_BUFFER_LENGTH 512 // 512 * 8 or 128 * 32

uint8_t address = 0x3c;
uint8_t addressWrite = 0x78; // address << 1 | 0x00
uint8_t addressRead = 0x79; // address << 1 | 0x01

uint8_t command = 0x00; // or 0x80 
uint8_t dataByte = 0x40;
uint8_t dataArray = 0xc0;

uint16_t scr_buffer[SCREEN_BUFFER_LENGTH] = {0}; // Буфер дисплея

static const uint8_t ssd1306_128x32_init[] = {
  0xAE, // Выключить дисплей (DISPLAY_OFF)
  
  0x8D, // Управление внутреним преобразователем (CHARGEPUMP)
  0x14, // 0x10 - отключить (VCC подается извне) 0x14 - запустить внутрений DC/DC   
  0x20, // Режим автоматической адресации (ADDRESSING_MODE)
  0x00, // 0-по горизонтали с переходом на новую страницу (строку) // 1 - по вертикали с переходом на новую строку // 2 - только по выбранной странице без перехода   
    
  0xDA, // Аппаратная конфигурация COM (SETCOMPINS)
    0x02, // 0x02 - 128x32, 0x12 - 128x64  	
	
  0xAF, // Дисплей включен (DISPLAY_ON)  
};

/* Список функций */
void IIC_Init(u32 bound, u16 address);
void display_init(void);
void disp_write(uint8_t mode, uint8_t data);
void screen_clear(void);
void screen_update(void);
void disp_write_array(void);
void test_screen(void);



/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{
	Delay_Init();
	USART_Printf_Init(115200);
	printf("SystemClk:%d\r\n",SystemCoreClock);

	IIC_Init( 100000, address );	
	display_init();	
	screen_clear();
	Delay_Ms(1000);

	while(1)
	{
		test_screen();
	}
}



/*********************************************************************
 * @fn      IIC_Init
 *
 * @brief   Initializes the IIC peripheral.
 *
 * @return  none
 */
void IIC_Init(u32 bound, u16 address)
{
	GPIO_InitTypeDef GPIO_InitStructure={0};
	I2C_InitTypeDef I2C_InitTSturcture={0};

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE );
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_I2C2, ENABLE );

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOB, &GPIO_InitStructure );

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOB, &GPIO_InitStructure );

	I2C_InitTSturcture.I2C_ClockSpeed = bound;
	I2C_InitTSturcture.I2C_Mode = I2C_Mode_I2C;
	I2C_InitTSturcture.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitTSturcture.I2C_OwnAddress1 = address;
	I2C_InitTSturcture.I2C_Ack = I2C_Ack_Enable;
	I2C_InitTSturcture.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;

  	I2C_Init( I2C2, &I2C_InitTSturcture );

	I2C_Cmd( I2C2, ENABLE );

	I2C_AcknowledgeConfig( I2C2, ENABLE );
}



// Функция инициализации дисплея
void display_init(void) 
{
	// Посылаем команды в дисплей из массива инициализации
	for(uint8_t i = 0; i < sizeof ssd1306_128x32_init; i++) disp_write(0, ssd1306_128x32_init[i]);
}



// Функция записи данных/команды в дисплей
void disp_write(uint8_t mode, uint8_t data) // Режим: 1-данные, 0-команда
{
	if(mode) mode = dataByte; // Режим данных
	else mode = command; // Режим команды
  
	// i2c_start();
	while( I2C_GetFlagStatus( I2C2, I2C_FLAG_BUSY ) != RESET );
	I2C_GenerateSTART( I2C2, ENABLE );
	
	// i2c_send_byte(address<<1); // команда на запись (7 бит адреса + 1 бит на запись/чтение )	
	while( !I2C_CheckEvent( I2C2, I2C_EVENT_MASTER_MODE_SELECT ) );
	I2C_Send7bitAddress( I2C2, (address << 1), I2C_Direction_Transmitter );

	while( !I2C_CheckEvent( I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ) );

	// i2c_send_byte(mode);   //Control Byte - Command
	I2C_SendData( I2C2, mode );
	while( !I2C_CheckEvent( I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );

	// i2c_send_byte(data);    //payload	
	I2C_SendData( I2C2, data );
	while( !I2C_CheckEvent( I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );

	// i2c_stop();	
	while( !I2C_CheckEvent( I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );
	I2C_GenerateSTOP( I2C2, ENABLE );
}



// Функция очистки буфера дисплея
void screen_clear(void)
{
	for(uint16_t i = 0; i < SCREEN_BUFFER_LENGTH; i++) scr_buffer[i] = 0;
	screen_update();
}



// Функция обновления дисплея
void screen_update(void) 
{
	disp_write(0,0x21); // Установка столбца
	disp_write(0,0);    // Начальный адрес
	disp_write(0,127);  // Конечный адрес
	
	disp_write(0,0x22); // Установка строки
	disp_write(0,0);    // Начальный адрес
	disp_write(0,3);    // Конечный адрес
	
	// Запись данных из буфера в дисплей
	disp_write_array();  
}


// Функция записи массива данных из буфера в дисплей
void disp_write_array(void) 
{
	// i2c_start();
	while( I2C_GetFlagStatus( I2C2, I2C_FLAG_BUSY ) != RESET );
	I2C_GenerateSTART( I2C2, ENABLE );

	// i2c_send_byte(address<<1); // команда на запись (7 бит адреса + 1 бит на запись/чтение )	
	while( !I2C_CheckEvent( I2C2, I2C_EVENT_MASTER_MODE_SELECT ) );
	I2C_Send7bitAddress( I2C2, (address << 1), I2C_Direction_Transmitter );

	while( !I2C_CheckEvent( I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ) );

	for(uint16_t i = 0; i < SCREEN_BUFFER_LENGTH; i++) {
		// i2c_send_byte(dataArray);
		I2C_SendData( I2C2, dataArray );
		while( !I2C_CheckEvent( I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );

		// i2c_send_byte(scr_buffer[i]);   
		I2C_SendData( I2C2, scr_buffer[i] );
		while( !I2C_CheckEvent( I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) ); 
	}

	// i2c_stop();	
	while( !I2C_CheckEvent( I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );
	I2C_GenerateSTOP( I2C2, ENABLE );   
}



// tests
void test_screen(void) 
{
	uint8_t flag = 0xff;
	for(uint16_t i = 0; i < 1024; i++) {
		scr_buffer[i] = flag;
		if (flag) flag = 0x00;
		else flag = 0xff;
	}
	screen_update();	
	Delay_Ms(1000);

	screen_clear();	
	Delay_Ms(1000);

	for(uint16_t i = 0; i < SCREEN_BUFFER_LENGTH; i++) scr_buffer[i] = 0xff;
	screen_update();
	Delay_Ms(1000);
}


