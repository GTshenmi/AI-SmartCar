/*
 * neuralnetwork.c
 *
 *  Created on: 2020年12月6日
 *      Author: 936305695
 *  @Brief:
 *          This file is for the Neural Network.
 */
#include <neuralnetwork.h>
#include "NNLayer.h"
#include "include.h"

model_info_struct Model_Info_Element;
model_info_struct Model_Info_Tracking;

void NeuralNetworkInit(void)
{
    //Model_GetInfo(element_model, &Model_Info_Element);
    Model_GetInfo(tracking_model, &Model_Info_Tracking);
}
/*
 * @Brief:神经网络推理接口函数
 * */
void NeuralNetworkReasoning(void *data)
{
  
}

uint16_t ElementModelRun(void *input)
{
    float P[3] = {0.0};

    float *Output = NULL;

    int16_t output[3] = {0};

    Output = (float *)Model_Run(element_model,input,output);

    for(int i = 0; i < 3 ;i++)
    {
        P[i] = *(Output+i);

        //P[i] = output[i] * 1.0;
    }

    return FindMaxIndex(P,3);
}
sint16_t TrackingModelRun(void *input)
{
    sint16_t angle = 0;

    Model_Run(tracking_model,input,&angle);

    return angle;
}

#if defined(Chip) && Chip == RT1064
void get_model_info(const void *in_buf, model_info_struct *inf)
{

}
void* run_model(const void *model_buf, const void *in_buf, signed short int *out1_buf)
{
    return NULL;
}
#endif
