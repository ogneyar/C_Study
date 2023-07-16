
#ifndef __XPT2046_H_
#define __XPT2046_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include <ch32v30x.h>

#define	chy 	0x90 // координатные оси дисплея и тачскрина поменяны местами(там где у дисплея Х у тачсрина Y)
#define	chx 	0xD0 // за основу возьмём оси дисплея

#define Xmax	4200
#define Xmin	150

#define Ymax	4200
#define Ymin	150

// #define calibr


void XPT_Init(GPIO_TypeDef *port, uint8_t chipSelectPin);
void XPT_ChipSelectLow(void);
void XPT_ChipSelectHigh(void);
void XPT_GetTouch_xy(volatile uint16_t *x_kor,volatile uint16_t *y_kor);



#ifdef __cplusplus
}
#endif

#endif /* __XPT2046_H_ */
