/*
 * filter.c
 *
 *  Created on: 2021Äê1ÔÂ17ÈÕ
 *      Author: 936305695
 *      unfinished.
 */
#include "filter.h"
#include "stdlib.h"
#include "sort.h"

#ifndef PI
#define PI 3.1415926
#endif

void Filter_Sort(float *data,int len)
{
    Quick_Sort(data,0,len - 1);
}

float LowPass_FilterCalA(float dt,float Fcut)
{
    return dt / (dt + 1/(2 * PI * Fcut));
}
float LowPass_Filter(float old_data,float new_data,float a)
{
    return (old_data * (1 - a) + new_data * a);
}


float Average_Filter(float *data,int len)
{
    float result = 0.0;

    for(int i = 0 ; i < len ; i++)
        result += data[i];

    return result / len;
}

float Silding_Filter(float *data,float new_data,float *weight,int len)
{
    float result = 0.0;

    for(int i = 0 ; i < len - 1 ; i++)
    {
        data[i] = data[i + 1];
    }
    data[len - 1] = new_data;

    if(weight != NULL)
    {
        for(int i = 0 ; i < len ; i++)
        {
            result += data[i] * weight[i];
        }
    }
    else
    {
        for(int i = 0 ; i < len ; i++)
        {
            result += data[i];
        }
        result /= len;
    }

    return result;
}

float Middle_Filter(float *data,int len)
{
    Filter_Sort(data,len);

    if(len % 2)
    {
        return data[(len-1)/2];
    }
    else
    {
        return (data[len/2] + data[len/2 - 1]) / 2;
    }
}

float RemoveExtremumAverage_Filter(float *data,int len)
{
    Filter_Sort(data,len);

    float result = 0.0;
    for(int i = 1 ; i < len - 1 ; i++)
    {
        result += data[i];
    }

    return result / (len - 2);
}

float IIR_Filter(float *a,float *b,float *x,float *y,float new_x,int len)
{
    float result = 0.0;

    for(int i = 0 ; i < len - 1 ; i++)
    {
        x[i] = x[i + 1];
    }
    for(int i = 0 ; i < len - 1 ; i++)
    {
        y[i] = y[i + 1];
    }
    x[len - 1] = new_x;

    for(int i = 0 ; i < len - 1 ; i++)
    {
        result += (a[i] * y[len - 1 - i]) + (b[i] * x[len - 1 - i]);
    }
    y[len - 1] = result;

    return result;
}


float FIR_Filter(float *b,float *x,float new_x,int len)
{
    float result = 0.0;

    for(int i = 0 ; i < len - 1 ; i++)
    {
        x[i] = x[i + 1];
    }
    x[len - 1] = new_x;

    for(int i = 0 ; i < len - 1 ; i++)
    {
        result += (b[i] * x[len - 1 - i]);
    }

    return result;
}

float Fusion_Filter(float *data,int len)
{
    return 0.0;
}

float Limiting_Filter(float data,float max,float min)
{
    if(max < min)
    {
        return max - min;
    }
    if(data > max)
        data = max;
    if(data < min)
        data = min;

    return data;
}




