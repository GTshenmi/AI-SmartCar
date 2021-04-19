/*
 * sysmath.c
 *
 *  Created on: 2021Äê1ÔÂ26ÈÕ
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
    if(Is_Zero(L1) && Is_Zero(L2))
        return 0.0;
    else
        return (L1 - L2) / (L1 + L2);
}

float CalculateDistanceP(float L1,float L2)
{
    if(Is_Zero(L1) && Is_Zero(L2))
        return 0;
    else
    {
        return (1/L1 - 1/L2);
    }
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

    uint seed = (uint) os.time.getTime(ms);

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

double Sigmoid(double input)
{
    return 1.0/(1.0 + exp(-input));
}

double Tanh(double input)
{
    return (exp(input) - exp(-input))/(exp(input) + exp(-input));
}

double Rule(double input)
{
    return max(0,input);
}

double Rule8(double input)
{
    double res = max(0,input);

    res = ConstrainFloat(res,0.0,8.0);

    return res;
}