/*
 * neuralnetwork.c
 *
 *  Created on: 2020��12��6��
 *      Author: 936305695
 *  @Brief:
 *          This file is for the Neural Network.
 */
#include <neuralnetwork.h>
#include "include.h"


void NeuralNetworkInit(void *argv)
{
    //NNInit(TestModel);
}


float NeuralNetworkCalAngle(void *argv)
{
    data_t *data = (data_t *)argv;

    sint16_t *angle = NNForWardReasoning(NULL,data->SESensor_SampleValue,0);

    float res = ((*angle) >> (14 - 10 - 1) ) * 1.0;

    data->NNOutput = res;

    //return res;

    return 1.25 * exp(0.0012 * fabs(res)) * res;
    //return res * 1.25;
}

//float NeuralNetworkCalAngle(void *argv)
//{
//    data_t *data = (data_t *)argv;
//
//    uint16_t Input[9] = {0,0,0,0,0,0,0,0,0};
//
//    for(int i = 0 ; i < 8 ;i++)
//    {
//        Input[i] = data->SESensor_SampleValue[i];
//    }
//
//    Input[8] = (uint16_t)(data->Speed / 100.0);
//
//    sint16_t *angle = NNForWardReasoning(NULL,Input,0);
//
//    float res = ((*angle) >> (14 - 10 - 1) ) * 1.0;
//
//    data->NNOutput = res;
//
//    //return 1.25 * exp(0.0012 * fabs(res)) * res;
//
//    return  1.4 * exp(0.0010 * fabs(res)) * res;
//
//}

/*
 * @Brief:����������ӿں���
 * */
float NeuralNetworkReasoning(void *argv)
{
    /*
        data->SESensor_SampleValue[i];     ��ǰհ���ԭ����;
        data->SESensor_NormalizedValue[i]; ��ǰհ��й�һ������;
        data->Angle;                       ������սǶ�
        data->Element.Type;                ����Ԫ������;
        data->Speed;                       ��������ٶ�;
     */

    float angle;

    angle = NeuralNetworkCalAngle(argv);

    return angle;
}
