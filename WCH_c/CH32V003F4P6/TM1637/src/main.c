
#include "debug.h"
#include "tm1637.h"


// основная функция
int main(void)
{    
    tm1637_init(GPIOC, GPIO_Pin_2, GPIO_Pin_1);  // по умолчанию CLK = GPIO_Pin_2, а DIO = GPIO_Pin_1
    tm1637_clear();
    tm1637_brightness(1);  // яркость, 0 - 7 (минимум - максимум)
    tm1637_point(1);
    tm1637_display(8, 8, 8, 8);
    Delay_Ms(1500);
    tm1637_display(0, 0, 0, 0);
    Delay_Ms(1500);
    tm1637_display(0, 1, 2, 3);

    while(1);
}
