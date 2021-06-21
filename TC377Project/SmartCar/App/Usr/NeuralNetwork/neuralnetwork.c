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

//    uint16_t sESensorData[MAX_SESENSOR_NUM];
//
//    for(int i = 0 ; i < MAX_SESENSOR_NUM ; i++)
//    {
//        sESensorData[i] = (uint16_t)data->SESensor_NormalizedValue[i];
//    }

    sint16_t *angle = NNForWardReasoning(NULL,data->SESensor_SampleValue,0);

    return ((*angle) >> (14 - 10 - 1) ) * 1.1;
}

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
