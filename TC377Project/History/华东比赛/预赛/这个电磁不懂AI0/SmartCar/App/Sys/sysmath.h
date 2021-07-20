/*
 * sysmath.h
 *
 *  Created on: 2021Äê1ÔÂ26ÈÕ
 *      Author: 936305695
 */

#ifndef APP_SYS_SYSMATH_H_
#define APP_SYS_SYSMATH_H_

#include "platform.h"

#define PI 3.14

#define Zero 1e-6

#define INFINITE 0xFFFFFFFF

#define Is_Zero(x) ((fabs(x)) < Zero)

#define FIs_Equal(x,y) (fabs((x) - (y)) < Zero)

#define fsign(x) ((x) > 0.0 ?  1.0 : -1.0 )

#define max(a,b) ((a) > (b) ? (a) : (b))

#define min(a,b) ((a) < (b) ? (a) : (b))

#define square(x) ((x) * (x))

#define Is_ApproximatelyEqual(x,y,error)    ((fabs(((x) * 1.0 - (y)) * 1.0) - fabs(error * 1.0)) <= 0.0 ? 1 : 0)

#define __float__(x) ((float)(x))
#define __char__(x)  ((char)(x))
#define __string__(x)  ((char *)(x))
#define __int__(x)   ((int)(x))
#define __uint__(x)  ((uint)(x))

#define __uint8__(x)  ((uint8_t)(x))
#define __uint16__(x)  ((uint16_t)(x))
#define __uint32__(x)  ((uint32_t)(x))
#define __uint64__(x)  ((uint64_t)(x))

#define __sint8__(x)  ((sint8_t)(x))
#define __sint16__(x)  ((sint16_t)(x))
#define __sint32__(x)  ((sint32_t)(x))
#define __sint64__(x)  ((sint64_t)(x))

#define __pint__(x)    ((int *)(x))



inline float fdiv(float x,float y)
{
    float result = 0.0;

    if(Is_Zero(x) && Is_Zero(y))
    {
        result = NAN;
    }
    else if(Is_Zero(y) && !(Is_Zero(x)))
    {
        result = ( y > 0 ) ? 3.4e38 : -1.1e38;
    }
    else
    {
        result = x/y;
    }

    return result;
}

float invSqrt(float x);
float NormalizeFloat(float value,float min,float max);
float CalculateDistanceDifDivSum(float L1,float L2);
uint16_t FindMaxIndex(float *array,uint16_t len);
uint16_t FindMinIndex(float *array,uint16_t len);
float ConstrainFloat(float value,float min,float max);
float random(float lower,float upper);
float frand(void);
double Sigmoid(double input);
double Tanh(double input);
double Rule(double input);
double Rule8(double input);


#endif /* APP_SYS_SYSMATH_H_ */
