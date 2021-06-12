/*
 * neuralnetwork.c
 *
 *  Created on: 2020年12月6日
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

    uint16_t sESensorData[MAX_SESENSOR_NUM];

    for(int i = 0 ; i < MAX_SESENSOR_NUM ; i++)
    {
        sESensorData[i] = (uint16_t)data->SESensor_NormalizedValue[i];
    }

    uint16_t *angle = NNForWardReasoning(NULL,sESensorData,0);

    return (*angle) * 1.0/32768;
}

/*
 * @Brief:神经网络推理接口函数
 * */
float NeuralNetworkReasoning(void *argv)
{
    data_t *data = (data_t *)argv;

    /*
        data->SESensor_SampleValue[i];     短前瞻电感原数据;
        data->SESensor_NormalizedValue[i]; 短前瞻电感归一化数据;
        data->Angle;                       舵机最终角度
        data->Element.Type;                赛道元素类型;
        data->Speed;                       电机最终速度;
     */

    float angle;

    angle = NeuralNetworkCalAngle(data);

    return angle;
}
