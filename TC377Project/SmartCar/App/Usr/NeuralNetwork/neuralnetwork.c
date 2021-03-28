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


void NeuralNetworkInit(void)
{

}
/*
 * @Brief:神经网络推理接口函数
 * */
void NeuralNetworkReasoning(void *data)
{
    data_t *usrdata = (data_t *)data;

    if(usrdata->AI_State == AI_Start)
    {
        usrdata->AI_State = AI_Busy;

        usrdata->AI_State = AI_Fin;
    }
}
