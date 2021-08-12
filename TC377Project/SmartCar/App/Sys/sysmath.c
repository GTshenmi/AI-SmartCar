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

float Q_RSqrt( float number )
{
    sint32_t i;
    float x2, y;
    const float threehalfs = 1.5F;
 
    x2 = number * 0.5F;
    y  = number;
    i  = * ( sint32_t * ) &y;                       // evil floating point bit level hacking
    i  = 0x5f3759df - ( i >> 1 );                   // what the fuck?
    y  = * ( float * ) &i;
    y  = y * ( threehalfs - ( x2 * y * y ) );       // 1st iteration
//    y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed
 
    return y;
}

float Q_Sqrt( float number )
{
    sint32_t i;
    float x2, y;
    const float threehalfs = 1.5F;
 
    x2 = number * 0.5F;
    y  = number;
    
    i  = * ( sint32_t * ) &y;                      
    i = (i + 0x3f76cf62) >> 1;  
    y = *(float *)&i; 
    
    y = (y +  number/ y) * 0.5;  
 
    return y;
}

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

uint16_t FindMinIndex(float *array,uint16_t len)
{
     float min = array[0];
     uint16_t index = 0;

     for(uint16_t i = 0 ; i < len ;i++)
     {
         if(array[i] < min)
         {
             min = array[i];
             index = i;
         }
     }

     return index;
}

float frand()
{
    float result = 0.0;

    //uint unit = rand() % 3;

    uint seed = (uint) os.time.getTime(us);

    srand(seed);

    int randNum = (rand());

    result = (randNum * 1.0) / (RAND_MAX);

    return result;
}

float random(float lower,float upper)
{
    float result = frand();

    result = result * (upper - lower) + lower;

    return result;
}

float invSqrt(float x) {
	
    float halfx = 0.5f * x;
    float y = x;
	
    long i = *(long*)&y;
    i = 0x5f3759df - (i>>1);
    y = *(float*)&i;
	
    y = y * (1.5f - (halfx * y * y));
	
    return y;
	
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
