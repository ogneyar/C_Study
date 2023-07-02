
#ifndef __MAIN_HPP_
#define __MAIN_HPP_

#include "debug.h"
#include <ch32v30x.h>

/* Список функций */
void GPIO_Toggle_INIT(void);
void test(void);
void test(u8 i);


// Тестовый класс
class TestClass {

    private:      
      u8 _test;
      
    public:
        void test_method(void) {
            test_method(100);
        }
        void test_method(u16 delay);

    private:
        void test_private_method(void) {
            _test = 42;
        }
};



//
void TestClass::test_method(u16 delay) 
{
    GPIOB->OUTDR ^= GPIO_Pin_3;
    GPIOB->OUTDR ^= GPIO_Pin_4;
    GPIOB->OUTDR ^= GPIO_Pin_5;
    Delay_Ms(delay);
}


#endif /* __MAIN_HPP_ */
