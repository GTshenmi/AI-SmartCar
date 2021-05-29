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

void GetESensorData(void *argv)
{
    data_t *data = (data_t *)argv;
    for(int i = 0; i < CData.MaxLADCDeviceNum ; i++)
        data->LESensor_SampleValue[i] = LESensor[i].Read(LESensor[i].Self);
    for(int i = 0; i < CData.MaxSADCDeviceNum ; i++)
        data->SESensor_SampleValue[i] = SESensor[i].Read(SESensor[i].Self);
}

void ESensorDataProcess(void *argv)
{
    data_t *data = (data_t *)argv;

    //static float bias[5] = {0.0,0.0,0.0,0.0,0.0};
    //static float Kb[5] = {0.3,0.3,0.2,0.1,0.1};

    /*归一化*/
    for(int i = 0 ; i < CData.MaxLADCDeviceNum ; i++)
        data->LESensor_NormalizedValue[i] = 100.0 * NormalizeFloat(data->LESensor_SampleValue[i] * 1.0,ADCx.MinValue * 1.0,ADCx.MaxValue * 1.0);
    for(int i = 0 ; i < CData.MaxSADCDeviceNum ; i++)
        data->SESensor_NormalizedValue[i] = 100.0 * NormalizeFloat(data->SESensor_SampleValue[i] * 1.0,ADCx.MinValue * 1.0,ADCx.MaxValue * 1.0);

    data->Bias = CalculateBias(data);

    //data->Bias = FIR_Filter(Kb,bias,data->_Bias,5);
}


#define LESensor_Min 10.0

float CalculateDistance(float L1,float L2)
{
    return CalculateDistanceDifDivSum(L1,L2);
}


float CalculateBias(void *argv)     /*Calculate Bias And Element Type.*/
{
 /*
  * SESensor  : [0]  [1]  [2]  [3]  [4]  [5]  [6]
  * Direction :  |   ---   \   ---   /   ---   |
  * ESensor   : (-100.0,100.0)
  * Distance  : (-1.0,1.0)
  * Problem   : 1.直角切内弯会反方向误判
  *
  * */

    data_t *data = (data_t *)argv;

    static float bias = 0.0;

    data->H_ESensorValue[0] = data->LESensor_NormalizedValue[1];
    data->H_ESensorValue[1] = data->LESensor_NormalizedValue[3];
    data->H_ESensorValue[2] = data->LESensor_NormalizedValue[5];

    data->V_ESensorValue[0] = data->LESensor_NormalizedValue[0];
    data->V_ESensorValue[1] = data->LESensor_NormalizedValue[6];

    data->O_ESensorValue[0] = data->LESensor_NormalizedValue[2];
    data->O_ESensorValue[1] = data->LESensor_NormalizedValue[4];

    /*for H ESensor:*/

    data->h_bias = 100.0 * CalculateDistance(data->H_ESensorValue[0],data->H_ESensorValue[2]);

    data->h_difference = data->H_ESensorValue[0] - data->H_ESensorValue[2];

    data->h_sum = data->H_ESensorValue[0] + data->H_ESensorValue[2];

    /*for V ESensor:*/

    data->v_bias = 100.0 * CalculateDistance(data->V_ESensorValue[0],data->V_ESensorValue[1]);
    data->v_difference = data->V_ESensorValue[0] - data->V_ESensorValue[1];
    data->v_sum = data->V_ESensorValue[0] + data->V_ESensorValue[1];

    /*for O ESensor:*/

    data->o_bias = 100.0 * CalculateDistance(data->O_ESensorValue[0],data->O_ESensorValue[1]);
    data->o_difference = data->O_ESensorValue[0] - data->O_ESensorValue[1];
    data->o_sum = data->O_ESensorValue[0] + data->O_ESensorValue[1];

    /*Calculate Bias:*/

    bias = data->h_bias;

    return bias;
}

float CalculateSpeed(void *argv)
{
    return 0.0;
}
