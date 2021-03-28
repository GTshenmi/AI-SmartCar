/*
 * NNLib.h
 *
 *  Created on: 2021年3月27日
 *      Author: 936305695
 */

#ifndef APP_USR_NEURALNETWORK_NNLIB_H_
#define APP_USR_NEURALNETWORK_NNLIB_H_

#include "sys.h"

#define LAYER_TOTAL         4
#define LAYER_0_INDIM       3
#define LAYER_0_OUTDIM      3
#define LAYER_1_INDIM       3
#define LAYER_1_OUTDIM      1

/*
*   NNLayer变量定义
*/
typedef struct
{
    double* weight;             //权重二维矩阵  INDIM * OUTDIM
    double* bais;               //偏置数组      1 * OUTDIM

    double (*ActiveFunc)(double input);

    int inDim;                  //输入维数
    int outDim;                 //输出维数

    double* outVal;             //输出数组
}NNLayer_t;

extern NNLayer_t TestModel[4]; // 4 层

void NerualNetworkLayerInit(NNLayer_t* model);
float NerualNetworkLayerPredict(NNLayer_t* model,double* input);



#endif /* APP_USR_NEURALNETWORK_NNLIB_H_ */
