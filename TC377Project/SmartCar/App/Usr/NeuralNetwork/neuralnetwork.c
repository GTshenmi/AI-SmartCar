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


void NeuralNetworkInit(void)
{

}
/*
 * @Brief:����������ӿں���
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
