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

void GetESensorData(data_t *data);
void ESensorNormailze(data_t *data);
float CalculateDistance(float L1,float L2);
void LinearFit(float *data,float *k,uint16_t num);


float CalculateBiasLABM(data_t *data);
float CalculateBiasSABM(data_t *data);


void GetESensorData(data_t *data)
{
    for(int i = 0; i < CData.MaxLADCDeviceNum ; i++)
        data->LESensor_SampleValue[i] = LESensor[i].Read(LESensor[i].Self);
    for(int i = 0; i < CData.MaxSADCDeviceNum ; i++)
        data->SESensor_SampleValue[i] = SESensor[i].Read(SESensor[i].Self);
}

void ESensorNormailze(data_t *data)
{
    /*归一化*/
    for(int i = 0 ; i < CData.MaxLADCDeviceNum ; i++)
        data->LESensor_NormalizedValue[i] = 100.0 * NormalizeFloat(data->LESensor_SampleValue[i] * 1.0,ADCx.MinValue * 1.0,ADCx.MaxValue * 1.0);
    for(int i = 0 ; i < CData.MaxSADCDeviceNum ; i++)
        data->SESensor_NormalizedValue[i] = 100.0 * NormalizeFloat(data->SESensor_SampleValue[i] * 1.0,ADCx.MinValue * 1.0,ADCx.MaxValue * 1.0);
}

bool RecordFlags = false;

void ESensorDataAnalysis(void *argv)
{
    data_t *data = (data_t *)argv;

    GetESensorData(data);               //获取电感原数据

    ESensorNormailze(data);             //获取电感归一化数据

    switch(data->CarMode)
    {
        case AI_Mode:

            data->Bias = CalculateBiasSABM(data);

            LinearFit(data->SESensor_NormalizedValue,data->Ke,CData.MaxSADCDeviceNum);

            break;

        case SAutoBoot_Mode:

            data->Bias = CalculateBiasSABM(data);

            LinearFit(data->SESensor_NormalizedValue,data->Ke,CData.MaxSADCDeviceNum);

            break;

        case DebugMode:



            break;

        case LAutoBoot_Mode:

            data->Bias = CalculateBiasLABM(data);

            LinearFit(data->LESensor_NormalizedValue,data->Ke,CData.MaxLADCDeviceNum);

            break;

        default:

            break;

    }

    ElementDetermine(data);

    SpecialElementHandler(data);

    for(int i = 0 ; i < 9 ; i++)
    {
        data->B[i] = data->B[i + 1];
    }

    data->B[9] = data->Bias;

    RecordFlags = true;
}

void LinearFit(float *data,float *k,uint16_t num)
{
    uint16_t sensorNum = num;

    float sumXY[8] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0};
    float sumX2[8] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0};
    float aveX[8] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0};
    float aveY[8] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0};

#define PointNum 5

    static float eSensorData[PointNum][8];

    for(int i = 0 ; i < PointNum - 1 ; i++)
    {
        for(int j = 0 ; j < sensorNum ;j++)
        {
            eSensorData[i][j] = eSensorData[i + 1][j];
        }
    }

    for(int i = 0 ; i < sensorNum ;i++)
    {
        eSensorData[PointNum - 1][i] = data[i];
    }

    for (int i = 0; i < sensorNum; i++)
    {
        for (int j = 0; j < PointNum; j++)
        {
            float x = j * 0.02;
            float y = eSensorData[j][i] * 1.0;

            aveX[i] += x;
            aveY[i] += y;
            sumXY[i] += x * y;
            sumX2[i] += x * x;
        }

        aveX[i] /= (PointNum * 1.0);
        aveY[i] /= (PointNum * 1.0);
    }

    for (int i = 0; i < sensorNum; i++)
    {
        if(Is_Zero(sumX2[i] - PointNum * square(aveX[i])))
        {
            k[i] = 1.0;
        }
        else
        {
            k[i] = (sumXY[i] - PointNum * aveX[i] * aveY[i])/(sumX2[i] - PointNum * square(aveX[i]));
        }
    }
}


float CalculateBiasLABM(data_t *data)     /*Calculate Bias And Element Type.*/
{
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

    if(fabs(data->h_sum) <= 1e-6)
        bias = ((data->h_difference + data->v_difference * 0.78)/data->h_sum) * 100.0;

    return bias;
}

float CalculateBiasSABM(data_t *data)     /*Calculate Bias And Element Type.*/
{
    static float bias = 0.0;

    data->H_ESensorValue[0] = data->SESensor_NormalizedValue[0];
    data->H_ESensorValue[1] = data->SESensor_NormalizedValue[3];
    data->H_ESensorValue[2] = data->SESensor_NormalizedValue[4];
    data->H_ESensorValue[3] = data->SESensor_NormalizedValue[7];

    data->V_ESensorValue[0] = data->SESensor_NormalizedValue[1];
    data->V_ESensorValue[1] = data->SESensor_NormalizedValue[6];

    data->O_ESensorValue[0] = data->SESensor_NormalizedValue[2];
    data->O_ESensorValue[1] = data->SESensor_NormalizedValue[5];

    /*for H ESensor:*/

    data->h_bias = 100.0 * CalculateDistance(data->H_ESensorValue[0],data->H_ESensorValue[3]);

    data->h_difference = data->H_ESensorValue[0] - data->H_ESensorValue[3];

    data->h_sum = data->H_ESensorValue[0] + data->H_ESensorValue[3];

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

#define LESensor_Min 10.0

float CalculateDistance(float L1,float L2)
{
    return CalculateDistanceDifDivSum(L1,L2);
}

float CalculateSpeed(void *argv)
{
    return 0.0;
}
