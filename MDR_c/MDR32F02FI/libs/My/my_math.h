
#pragma once

#ifndef __MY_MATH_
#define __MY_MATH_

#include <stdlib.h>

#define CONST_PI    3.1415926535
#define CONST_2PI   2 * CONST_PI
#define modd(x, y) ((x) - (int)((x) / (y)) * (y))


unsigned short my_ceil(double num)
{
    return (unsigned short)(num + 1);
}


/*
    // cos_taylor_running_yterms
    https://habr.com/ru/companies/ruvds/articles/742620/
*/
double my_cos(double x)
{
    int div = (int)(x / CONST_PI);
    x = x - (div * CONST_PI);
    char sign = 1;
    if (div % 2 != 0)
        sign = -1;

    double result = 1.0;
    double inter = 1.0;
    double num = x * x;

    int y = 10; // точность вычислений, чем больше точность, тем дольше вычисление

    for (int i = 1; i <= y; i++)
    {
        double comp = 2.0 * i;
        double den = comp * (comp - 1.0);
        inter *= num / den;
        if (i % 2 == 0)
            result += inter;
        else
            result -= inter;
    }
    return sign * result;
}

#endif /* __MY_MATH_ */
