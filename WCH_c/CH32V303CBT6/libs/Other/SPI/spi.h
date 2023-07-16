
#ifndef __SPI_H_
#define __SPI_H_

/*
 pins:
    CS   -- PA2
    RES  -- PA3
    DC   -- PA4
    CLK  -- PA5
    MISO -- PA6
    MOSI -- PA7
*/

#ifdef __cplusplus
 extern "C" {
#endif

#include <ch32v30x.h>


/* Список функций */
void SPI1_Master_Init(uint8_t mode);
void SPI1_SendData(uint16_t Data);
uint16_t SPI1_ReceiveData(void);
uint8_t SPI1_GetFlagStatus(uint16_t SPI_FLAG);
uint8_t SPI1_Send(uint8_t data);
uint8_t SPI1_Receive(void);
uint8_t SPI1_Transmit(uint8_t data);


#define DD_CS 		(1 << PA2)
#define DD_RES 		(1 << PA3)
#define DD_DC 		(1 << PA4)
#define DD_CLK 		(1 << PA5)
#define DD_MISO 	(1 << PA6)
#define DD_MOSI 	(1 << PA7)

#ifdef __cplusplus
}
#endif

#endif /* __SPI_H_ */
