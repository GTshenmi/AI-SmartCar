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

    sint16_t *angle = NNForWardReasoning(NULL,data->SESensor_SampleValue,0);

    float res = ((*angle) >> (14 - 10 - 1) ) * 1.0;

    data->NNOutput = res;

    return 1.25 * exp(0.0012 * fabs(res)) * res;
}

/*
 * @Brief:神经网络推理接口函数
 * */
float NeuralNetworkReasoning(void *argv)
{
    /*
        data->SESensor_SampleValue[i];     短前瞻电感原数据;
        data->SESensor_NormalizedValue[i]; 短前瞻电感归一化数据;
        data->Angle;                       舵机最终角度
        data->Element.Type;                赛道元素类型;
        data->Speed;                       电机最终速度;
     */

    float angle;

    angle = NeuralNetworkCalAngle(argv);

    return angle;
}
