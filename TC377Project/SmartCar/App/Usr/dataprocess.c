/*
 * dataprocess.c
 *
 *  Created on: 2020年12月6日
 *      Author: 936305695
 *  @Brief:
 *      This file is for data process(read/process).
 *      数据读取/处理 相关子程序
 */
#include <dataprocess.h>
#include "include.h"

#define LESensor_Min 5.0

float CalculateDistance(void *argv)
{
/*
 * SESensor  : [0]  [1]  [2]  [3]  [4]  [5]  [6]
 * Direction :  |   ---   \   ---   /   ---   |
 * N_ADC     : (-100.0,100.0)
 * Distance  : (-1.0,1.0)
 *
 * */

    data_t *data = (data_t *)argv;

    static float difference_h = 0.0;
    static float difference_v = 0.0;
    static float difference_o = 0.0;

    static float sum_h = 0.0;
    static float sum_v = 0.0;
    static float sum_o = 0.0;

    static float distance_h = 0.0;
    static float distance_v = 0.0;
    static float distance_o = 0.0;

    static float distance = 0.0;

    if(data->N_LADC[1] > LESensor_Min && data->N_LADC[5] > LESensor_Min) //横电感差值
    {
        sum_h = data->N_LADC[1] + data->N_LADC[5];
        difference_h = data->N_LADC[1] - data->N_LADC[5];

        distance_h = CalculateDistanceDifDivSum(data->N_LADC[1],data->N_LADC[5]);
    }

    if(data->N_LADC[0] > LESensor_Min && data->N_LADC[6] > LESensor_Min) //竖电感差值
    {
        sum_v = data->N_LADC[0] + data->N_LADC[6];
        difference_v = data->N_LADC[0] - data->N_LADC[6];

        distance_v = CalculateDistanceDifDivSum(data->N_LADC[0],data->N_LADC[6]);
    }

    if(data->N_LADC[2] > LESensor_Min && data->N_LADC[4] > LESensor_Min) //斜电感差值
    {
        sum_o = data->N_LADC[2] + data->N_LADC[4];
        difference_o = data->N_LADC[2] - data->N_LADC[4];
        distance_o = CalculateDistanceDifDivSum(data->N_LADC[2],data->N_LADC[4]);
    }

    float weight = 0.0;


    if(Is_Zero(difference_h) && Is_Zero(difference_v))
    {
        distance = 0.0;
    }
    else
    {
        if(fabs(difference_h) > fabs(difference_v))
        {

            weight = fabs(difference_v)/fabs(difference_h);

            distance = (1 - weight) * distance_h + weight * distance_v;
        }
        else
        {
            weight = fabs(difference_h)/fabs(difference_v);

            distance = weight * distance_h + (1 - weight) * distance_v;
        }
    }

    if(data->N_LADC[3] > 80.0)
    {
        //环岛判别
    }

    uint index = FindMaxIndex(data->N_LADC,CData.MaxLADCDeviceNum);

    switch(index)
    {
        case 0:
            //distance = -1.0;
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            //distance = 1.0;
            break;
    }

    return distance;
}