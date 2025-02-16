/*
 * sysmath.c
 *
 *  Created on: 2021��1��26��
 *      Author: 936305695
 */
#include "sysmath.h"
#include "math.h"
#include "stdlib.h"
#include "os.h"

float NormalizeFloat(float value,float min,float max)
{
    value = ConstrainFloat(value,min,max);

    return (value - min) / (max - min);
}

float CalculateDistanceDifDivSum(float L1,float L2)
{
    if(fabs(L1) < 1e-6 && fabs(L2) < 1e-6)
        return 0;
    else
        return (L1 - L2) / (L1 + L2);
}

uint16_t FindMaxIndex(float *array,uint16_t len)
{
     float max = 0.0;
     uint16_t index = 0;

     for(uint16_t i = 0 ; i < len ;i++)
     {
         if(array[i] > max)
         {
             max = array[i];
             index = i;
         }
     }

     return index;
}

float frand()
{
    float result = 0.0;

    uint seed = (uint) os.time.get_timeus();;

    srand(seed);

    result = (rand() * 1.0) / (uintMax);

    return result;
}

float random(float lower,float upper)
{
    float result = frand();

    result = result * (upper - lower) + lower;

    return result;
}

float ConstrainFloat(float value,float min,float max)
{
    if(value > max)
        value = max;
    if(value < min)
        value = min;

    return value;
}
