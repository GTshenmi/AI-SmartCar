/*
 * neuralnetwork.h
 *
 *  Created on: 2020Äê12ÔÂ6ÈÕ
 *      Author: 936305695
 */

#ifndef APP_USR_NEURALNETWORK_NEURALNETWORK_H_
#define APP_USR_NEURALNETWORK_NEURALNETWORK_H_

#include "sys.h"
#include "model.nncu.h"
#include "NNLib.h"

#define Model_GetInfo get_model_info
#define Model_Run run_model

typedef struct _model_info_struct
{
    char author[30];
    int quant_bits;
    int frac_bits;
    const char* name_string;
}model_info_struct;

extern model_info_struct Model_Info_Element;
extern model_info_struct Model_Info_Tracking;

extern void get_model_info(const void *in_buf, model_info_struct *inf);
extern void* run_model(const void *model_buf, const void *in_buf, signed short int *out1_buf);

uint16_t ElementModelRun(void *input);
sint16_t TrackingModelRun(void *input);

void NeuralNetworkReasoning(void *data);
void NeuralNetworkInit(void);

#endif /* APP_USR_NEURALNETWORK_NEURALNETWORK_H_ */
