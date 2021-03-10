/*
 * sysmath.c
 *
 *  Created on: 2021Äê1ÔÂ26ÈÕ
 *      Author: 936305695
 */
#include "sysmath.h"
#include "math.h"

float NormalizeFloat(float value,float max,float min)
{
    if(value > max)
        value = max;
    if(value < min)
        value = min;

    return (value - min) / (max - min);
}

float CalculateDistanceDifDivSum(float L1,float L2)
{
    if(fabs(L1) < 1e-6 && fabs(L2) < 1e-6)
        return 0;
    else
        return (L1 - L2) / (L1 + L2);
}

