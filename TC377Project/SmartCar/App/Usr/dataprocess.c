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

float LineFitLeastSquares(float *data_x, float *data_y, int data_n)
{
    float A = 0.0;
    float B = 0.0;
    float C = 0.0;
    float D = 0.0;
    float E = 0.0;
    float F = 0.0;

    for (int i=0; i<data_n; i++)
    {
        A += data_x[i] * data_x[i];
        B += data_x[i];
        C += data_x[i] * data_y[i];
        D += data_y[i];
    }

    // 计算斜率a和截距b
    float a, b, temp = 0;

    temp = (data_n*A - B*B);

    if(fabs(temp) < 1e-6)// 判断分母不为0
    {
        a = (data_n*C - B*D) / temp;
        b = (A*D - B*C) / temp;
    }
    else
    {
        a = 1;
        b = 0;
    }

    // 计算相关系数r
    float Xmean, Ymean;
    Xmean = B / data_n;
    Ymean = D / data_n;

    float tempSumXX = 0.0, tempSumYY = 0.0;
    for (int i=0; i<data_n; i++)
    {
        tempSumXX += (data_x[i] - Xmean) * (data_x[i] - Xmean);
        tempSumYY += (data_y[i] - Ymean) * (data_y[i] - Ymean);
        E += (data_x[i] - Xmean) * (data_y[i] - Ymean);
    }
    F = sqrt(tempSumXX) * sqrt(tempSumYY);

    float r;
    r = E / F;
    return a;
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

    float sumXY[7] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0};
    float sumX2[7] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0};
    float aveX[7] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0};
    float aveY[7] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0};

#define PointNum 5

    static float eSensorData[PointNum][7];

    for(int i = 0 ; i < PointNum - 1 ; i++)
    {
        for(int j = 0 ; j < 7 ;j++)
        {
            eSensorData[i][j] = eSensorData[i + 1][j];
        }
    }

    for(int i = 0 ; i < 7 ;i++)
    {
        eSensorData[PointNum - 1][i] = data->LESensor_NormalizedValue[i];
    }

//    float *(eSensorData)[PointNum];
//
//    eSensorData[0] = data->LESensor_NormalizedValue;

//    for(int i = 1 ; i < PointNum;i++)
//    {
//        eSensorData[i] = Queue.Gets(&data->ESensorQueue,-i,NULL,0,7);
//    }

    for (int i = 0; i < 7; i++)
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

    for (int i = 0; i < 7; i++)
    {
        //data->Ke[i] = (sumXY[i] - PointNum * aveX[i] * aveY[i])/(sumX2[i] - PointNum * square(aveX[i]));
        //data->Ke[i] = LineFitLeastSquares(eSensorData[i]);
        if(Is_Zero(sumX2[i] - PointNum * square(aveX[i])))
        {
            data->Ke[i] = 1.0;
        }
        else
        {
            data->Ke[i] = (sumXY[i] - PointNum * aveX[i] * aveY[i])/(sumX2[i] - PointNum * square(aveX[i]));
        }
        //data->Ke[i] = fdiv((sumXY[i] - PointNum * aveX[i] * aveY[i]),(sumX2[i] - PointNum * square(aveX[i])));
    }

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
