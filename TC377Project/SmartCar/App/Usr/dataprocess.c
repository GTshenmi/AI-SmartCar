/*
 * dataprocess.c
 *
 *  Created on: 2020骞�12鏈�6鏃�
 *      Author: 936305695
 *  @Brief:
 *      This file is for data process(read/process).
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
    for(int i = 0 ; i < CData.MaxLADCDeviceNum ; i++)
        data->LESensor_NormalizedValue[i] = 100.0 * NormalizeFloat(data->LESensor_SampleValue[i] * 1.0,ADCx.MinValue * 1.0,ADCx.MaxValue * 1.0);
    for(int i = 0 ; i < CData.MaxSADCDeviceNum ; i++)
        data->SESensor_NormalizedValue[i] = 100.0 * NormalizeFloat(data->SESensor_SampleValue[i] * 1.0,ADCx.MinValue * 1.0,ADCx.MaxValue * 1.0);
}

bool RecordFlags = false;

void ESensorDataAnalysis(void *argv)
{
    data_t *data = (data_t *)argv;

    /*get raw data*/
    GetESensorData(data);               

    /*normalize.*/
    ESensorNormailze(data);           

    /*calculate bias and change rate of all inductance.*/
    switch(data->CarMode)            
    {
        case AI_Mode:

            LinearFit(data->SESensor_NormalizedValue,data->Ke,CData.MaxSADCDeviceNum);

            data->Bias = CalculateBiasSABM(data);

            break;

        case SAutoBoot_Mode:

            LinearFit(data->SESensor_NormalizedValue,data->Ke,CData.MaxSADCDeviceNum);

            data->Bias = CalculateBiasSABM(data);

            break;

        case DebugMode:



            break;

        case LAutoBoot_Mode:

            LinearFit(data->LESensor_NormalizedValue,data->Ke,CData.MaxLADCDeviceNum);

            data->Bias = CalculateBiasLABM(data);

            HowToNameThisFunc2(data);

//            if(data->Element.Type == Cycle)
//            {
//                data->Bias = CalculateBiasLABM(data);
//            }
//            else
//            {
//                data->Bias = CalculateBiasLABM(data) + random(-35.0,35.0);
//            }

            break;

        default:

            break;

    }

    /*For special elements, correct the bias.*/
    if(data->CarMode != DebugMode)
    {
        ElementDetermine(data);

        SpecialElementHandler(data);
    }

    /*record historical bias.*/
    for(int i = 0 ; i < 9 ; i++)
    {
        data->B[i] = data->B[i + 1];
    }

    data->B[9] = data->Bias;

    RecordFlags = true;
}

void LinearFit(float *data,float *k,uint16_t num)/*Use Least Squares*/
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


float CalculateBiasLABM(data_t *data)     /*Calculate Bias.*/
{
    /*
     * LESensor [0] [1] [2] [3] [4] [5] [6]
     *           |   -   \   -   /   -   |
     * */

    static float bias = 0.0;

    data->HESensor[0].Value = data->LESensor_NormalizedValue[1];
    data->HESensor[1].Value = data->LESensor_NormalizedValue[3];
    data->HESensor[2].Value = data->LESensor_NormalizedValue[3];//C
    data->HESensor[3].Value = data->LESensor_NormalizedValue[5];//C

    data->VESensor[0].Value = data->LESensor_NormalizedValue[0];
    data->VESensor[1].Value = data->LESensor_NormalizedValue[6];

    data->OESensor[0].Value = data->LESensor_NormalizedValue[2];
    data->OESensor[1].Value = data->LESensor_NormalizedValue[4];

    data->HESensor[0].K = data->Ke[1];
    data->HESensor[1].K = data->Ke[3];
    data->HESensor[2].K = data->Ke[3];//C
    data->HESensor[3].K = data->Ke[5];//C

    data->VESensor[0].K = data->Ke[0];
    data->VESensor[1].K = data->Ke[6];

    data->OESensor[0].K = data->Ke[2];
    data->OESensor[1].K = data->Ke[4];

    /*for H ESensor:*/

    data->h_bias = 100.0 * CalculateDistance(data->HESensor[0].Value,data->HESensor[3].Value);

    data->h_difference = data->HESensor[0].Value - data->HESensor[3].Value;

    data->h_sum = data->HESensor[0].Value + data->HESensor[3].Value;

    data->h_suml2 = data->HESensor[0].Value + data->HESensor[1].Value;

    data->h_sumr2 = data->HESensor[2].Value + data->HESensor[3].Value;

    data->h_sum2 = data->HESensor[0].Value + data->HESensor[3].Value;

    data->h_suml3 = data->HESensor[0].Value + data->HESensor[3].Value + data->HESensor[1].Value;

    data->h_sumr3 = data->HESensor[0].Value + data->HESensor[3].Value + data->HESensor[2].Value;

    data->h_sum3 = data->HESensor[0].Value + data->HESensor[3].Value + ((data->HESensor[1].Value + data->HESensor[2].Value)/2.0);

    data->h_sum4 = data->HESensor[0].Value + data->HESensor[3].Value + data->HESensor[1].Value + data->HESensor[2].Value;

    /*for V ESensor:*/

    data->v_bias = 100.0 * CalculateDistance(data->VESensor[0].Value,data->VESensor[1].Value);
    data->v_difference = data->VESensor[0].Value - data->VESensor[1].Value;
    data->v_sum = data->VESensor[0].Value + data->VESensor[1].Value;

    /*for O ESensor:*/

    data->o_bias = 100.0 * CalculateDistance(data->OESensor[0].Value,data->OESensor[1].Value);
    data->o_difference = data->OESensor[0].Value - data->OESensor[1].Value;
    data->o_sum = data->OESensor[0].Value + data->OESensor[1].Value;

    /*Calculate Bias:*/

    bias = data->h_bias;

    if(fabs(data->h_sum) <= 1e-6 && fabs(data->v_sum) <= 1e-6)
        bias = 0.0;
    else if(fabs(data->h_sum) <= 1e-6)
        bias = 100.0;
    else
        bias = ((data->h_difference + data->v_difference * 0.78)/data->h_sum) * 100.0;

    return bias;
}

float CalculateBiasSABM(data_t *data)     /*Calculate Bias.*/
{
    /*
     * SESensor [0] [1] [2] [3] [4] [5] [6] [7]
     *           -   |   \   -   -   /   |   -
     * */

    static float bias = 0.0;

    data->HESensor[0].Value = data->SESensor_NormalizedValue[0];
    data->HESensor[1].Value = data->SESensor_NormalizedValue[3];
    data->HESensor[2].Value = data->SESensor_NormalizedValue[4];
    data->HESensor[3].Value = data->SESensor_NormalizedValue[7];

    data->VESensor[0].Value = data->SESensor_NormalizedValue[1];
    data->VESensor[1].Value = data->SESensor_NormalizedValue[6];

    data->OESensor[0].Value = data->SESensor_NormalizedValue[2];
    data->OESensor[1].Value = data->SESensor_NormalizedValue[5];

    data->HESensor[0].K = data->Ke[0];
    data->HESensor[1].K = data->Ke[3];
    data->HESensor[2].K = data->Ke[4];
    data->HESensor[3].K = data->Ke[7];

    data->VESensor[0].K = data->Ke[1];
    data->VESensor[1].K = data->Ke[6];

    data->OESensor[0].K = data->Ke[2];
    data->OESensor[1].K = data->Ke[5];

    /*for H ESensor:*/

    data->h_bias = 100.0 * CalculateDistance(data->HESensor[0].Value,data->HESensor[3].Value);

    data->h_difference = data->HESensor[0].Value - data->HESensor[3].Value;

    data->h_sum = data->HESensor[0].Value + data->HESensor[3].Value;

    data->h_suml2 = data->HESensor[0].Value + data->HESensor[1].Value;

    data->h_sumr2 = data->HESensor[2].Value + data->HESensor[3].Value;

    data->h_sum2 = data->HESensor[0].Value + data->HESensor[3].Value;

    data->h_suml3 = data->HESensor[0].Value + data->HESensor[3].Value + data->HESensor[1].Value;

    data->h_sumr3 = data->HESensor[0].Value + data->HESensor[3].Value + data->HESensor[2].Value;

    data->h_sum3 = data->HESensor[0].Value + data->HESensor[3].Value + ((data->HESensor[1].Value + data->HESensor[2].Value)/2.0);

    data->h_sum4 = data->HESensor[0].Value + data->HESensor[3].Value + data->HESensor[1].Value + data->HESensor[2].Value;

    /*for V ESensor:*/

    data->v_bias = 100.0 * CalculateDistance(data->VESensor[0].Value,data->VESensor[1].Value);
    data->v_difference = data->VESensor[0].Value - data->VESensor[1].Value;
    data->v_sum = data->VESensor[0].Value + data->VESensor[1].Value;

    /*for O ESensor:*/

    data->o_bias = 100.0 * CalculateDistance(data->OESensor[0].Value,data->OESensor[1].Value);
    data->o_difference = data->OESensor[0].Value - data->OESensor[1].Value;
    data->o_sum = data->OESensor[0].Value + data->OESensor[1].Value;

    /*Calculate Bias:*/

    bias = data->h_bias;

    if(fabs(data->h_sum) <= 1e-6 && fabs(data->v_sum) <= 1e-6)
        bias = 0.0;
    else if(fabs(data->h_sum) <= 1e-6)
        bias = 100.0;
    else
        bias = ((data->h_difference + data->v_difference * 0.78)/data->h_sum) * 100.0;

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
