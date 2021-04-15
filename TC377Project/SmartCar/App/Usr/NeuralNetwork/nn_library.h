/*
 * NNLib.h
 *
 *  Created on: 2021��3��27��
 *      Author: 936305695
 */

#ifndef APP_USR_NEURALNETWORK_NN_LIBRARY_H_
#define APP_USR_NEURALNETWORK_NN_LIBRARY_H_

#include "sys.h"
#include "nn_parameter.h"
#include "tricore_nn.h"

#define LAYER_TOTAL         4
#define LAYER_0_INDIM       3
#define LAYER_0_OUTDIM      3
#define LAYER_1_INDIM       3
#define LAYER_1_OUTDIM      1

#define USING_NNCU 1

/*
*   NNLayer��������
*/
typedef struct
{
    double* weight;             //Ȩ�ض�ά����  INDIM * OUTDIM
    double* bais;               //ƫ������      1 * OUTDIM

    double (*Active)(double input);

    int inDim;                  //����ά��
    int outDim;                 //���ά��

    double* outVal;             //�������
}NNLayer_t;

extern NNLayer_t TestModel[4]; // 4 ��

void NNInit(NNLayer_t* model);
void *NNForWardReasoning(NNLayer_t* model,void* input,uint32_t layer_num);



#endif /* APP_USR_NEURALNETWORK_NN_LIBRARY_H_ */
