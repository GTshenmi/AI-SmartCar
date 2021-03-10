/*
 * NNLayer.h
 *
 *  Created on: 2021��1��23��
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
*   NNLayer��������
*/
typedef struct
{
    const double* weight;             //Ȩ�ض�ά����  INDIM * OUTDIM
    const double* bais;               //ƫ������      1 * OUTDIM
    int inDim;                  //����ά��
    int outDim;                 //���ά��
    double* outVal;             //�������
}NNLayer;


void NNLayerInit();
void NNLayerPredict(NNLayer* model,double* x);


#endif /* APP_USR_NNLAYER_H_ */
