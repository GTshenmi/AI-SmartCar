/*
 * NNLayer.h
 *
 *  Created on: 2021年1月23日
 *      Author: 936305695
 */

#ifndef APP_USR_NNLAYER_H_
#define APP_USR_NNLAYER_H_


#define LAYER_TOTAL         2
#define LAYER_0_INDIM       3
#define LAYER_0_OUTDIM      3
#define LAYER_1_INDIM       3
#define LAYER_1_OUTDIM      1

/*
*   NNLayer变量定义
*/
typedef struct
{
    const double* weight;             //权重二维矩阵  INDIM * OUTDIM
    const double* bais;               //偏置数组      1 * OUTDIM
    int inDim;                  //输入维数
    int outDim;                 //输出维数
    double* outVal;             //输出数组
}NNLayer;


void NNLayerInit();
void NNLayerPredict(NNLayer* model,double* x);


#endif /* APP_USR_NNLAYER_H_ */
