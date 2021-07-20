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

void GetESensorData(void *argv);
void ESensorNormailze(void *argv);
float CalculateDistance(float L1,float L2);
void LinearFit(void *argv,uint16_t argc);


float CalculateBiasLABM(void *argv);
float CalculateBiasSABM(void *argv);


void GetESensorData(void *argv)
{
    data_t *data = (data_t *)argv;
    for(int i = 0; i < CData.MaxLADCDeviceNum ; i++)
        data->LESensor_SampleValue[i] = LESensor[i].Read(LESensor[i].Self);
    for(int i = 0; i < CData.MaxSADCDeviceNum ; i++)
        data->SESensor_SampleValue[i] = SESensor[i].Read(SESensor[i].Self);
}

void ESensorNormailze(void *argv)
{
    data_t *data = (data_t *)argv;

    /*归一化*/
    for(int i = 0 ; i < CData.MaxLADCDeviceNum ; i++)
        data->LESensor_NormalizedValue[i] = 100.0 * NormalizeFloat(data->LESensor_SampleValue[i] * 1.0,ADCx.MinValue * 1.0,ADCx.MaxValue * 1.0);
    for(int i = 0 ; i < CData.MaxSADCDeviceNum ; i++)
        data->SESensor_NormalizedValue[i] = 100.0 * NormalizeFloat(data->SESensor_SampleValue[i] * 1.0,ADCx.MinValue * 1.0,ADCx.MaxValue * 1.0);

    //data->SESensor_NormalizedValue[3] = 0.0;
}

bool RecordFlags = false;

void ESensorDataAnalysis(void *argv)
{
    data_t *data = (data_t *)argv;

    GetESensorData(data);               //获取电感原数据

    ESensorNormailze(data);             //获取电感归一化数据

    //Queue.Puts(data->ESensorQueue,data->LESensor_NormalizedValue,0.7);

    if(data->CarMode == AI_Mode)
    {
        //data->Bias = CalculateBiasLABM(data);

        data->Bias = CalculateBiasSABM(data);

        Queue.Puts(&data->ESensorQueue,data->SESensor_NormalizedValue,0,7);

        LinearFit(data,CData.MaxSADCDeviceNum);
    }
    else if(data->CarMode == LAutoBoot_Mode)
    {
        data->Bias = CalculateBiasLABM(data);// + 20.0;

        Queue.Puts(&data->ESensorQueue,data->LESensor_NormalizedValue,0,7);

        LinearFit(data,CData.MaxLADCDeviceNum);
    }
    else if(data->CarMode == DebugMode)
    {

    }
    else if(data->CarMode == SAutoBoot_Mode)
    {
        data->Bias = CalculateBiasSABM(data);

        Queue.Puts(&data->ESensorQueue,data->SESensor_NormalizedValue,0,8);

        LinearFit(data,CData.MaxSADCDeviceNum);
    }

    Queue.Puts(&data->RawBiasQueue,&data->Bias,0,1);

    Queue.Puts(&data->HESensorQueue,data->H_ESensorValue,0,4);
    Queue.Puts(&data->VESensorQueue,data->V_ESensorValue,0,2);
    Queue.Puts(&data->OESensorQueue,data->O_ESensorValue,0,2);

    Queue.Puts(&data->HBiasQueue,&data->h_bias,0,1);
    Queue.Puts(&data->VBiasQueue,&data->v_bias,0,1);
    Queue.Puts(&data->OBiasQueue,&data->o_bias,0,1);

//    if(data->CarMode != AI_Mode)
//    {
        ElementDetermine(data);

        SpecialElementHandler(data);
//    }

    //if(RecordFlags == false)
    RecordFlags = true;
}

void LinearFit(void *argv,uint16_t argc)
{
    data_t *data = (data_t *)argv;

    uint16_t sensorNum = argc;

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
        eSensorData[PointNum - 1][i] = data->LESensor_NormalizedValue[i];
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
            data->Ke[i] = 1.0;
        }
        else
        {
            data->Ke[i] = (sumXY[i] - PointNum * aveX[i] * aveY[i])/(sumX2[i] - PointNum * square(aveX[i]));
        }
    }
}


float CalculateBiasLABM(void *argv)     /*Calculate Bias And Element Type.*/
{
 /*
  * SESensor  : [0]  [1]  [2]  [3]  [4]  [5]  [6]
  * Direction :  |   ---   \   ---   /   ---   |
  * ESensor   : (-100.0,100.0)
  * Distance  : (-1.0,1.0)
  * Problem   : 1.直角切内弯会反方向误判
  *
  * */
    
    //static float _bias[5] = {0.0,0.0,0.0,0.0,0.0};

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

    //float Ka[5] = {0.3,0.3,0.2,0.2,0.1};

    //bias = FIR_Filter(Ka,_bias,((data->h_difference + data->v_difference) / data->h_sum) * 100.0,5);

    if(fabs(data->h_sum) <= 1e-6)
        bias = ((data->h_difference + data->v_difference * 0.78)/data->h_sum) * 100.0;

    //float weight = ((int)((data->Ke[0] + data->Ke[7]) / 2.0 + 0.5)) * 0.1;

    //bias = (1 - weight) * data->h_bias + weight * data->v_difference;

    return bias;
}

float CalculateBiasSABM(void *argv)     /*Calculate Bias And Element Type.*/
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
