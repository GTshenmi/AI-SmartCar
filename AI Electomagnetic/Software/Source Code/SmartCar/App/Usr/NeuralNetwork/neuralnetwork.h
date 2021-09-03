/*
 * neuralnetwork.h
 *
 *  Created on: 2020Äê12ÔÂ6ÈÕ
 *      Author: 936305695
 */

#ifndef APP_USR_NEURALNETWORK_NEURALNETWORK_H_
#define APP_USR_NEURALNETWORK_NEURALNETWORK_H_

#include "nn_library.h"
#include "sys.h"

typedef struct
{
    int16_t *Output;

    uint ElementType;

    float P;

}NN_Class_Output;

void NeuralNetworkInit(void *argv);
float NeuralNetworkReasoning(void *argv);

#endif /* APP_USR_NEURALNETWORK_NEURALNETWORK_H_ */
