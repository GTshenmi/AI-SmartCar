/*
 * NNLayer.c
 *
 *  Created on: 2021年1月23日
 *      Author: 936305695
 */
#include "NNLayer.h"

NNLayer gMyCalculatorModel[LAYER_TOTAL];
double gLayer0OutVal[LAYER_0_OUTDIM],gLayer1OutVal[LAYER_1_OUTDIM];
/*
*   训练好的模型参数
*/
const double W0[LAYER_0_INDIM][LAYER_0_OUTDIM] = {
    //  nn00        nn01        nn02
    {0.4455272  ,0.55392843 ,-0.13121112},
    {0.046779986,0.396654   ,-0.022363363},
    {-0.7579524 ,0.4773703  ,-0.22135651}
};
const double B0 [LAYER_0_OUTDIM] = {0.2265971  ,-0.025297487   ,0.027563533};
const double W1[LAYER_1_INDIM][LAYER_1_OUTDIM] = {
    {-0.13311525},
    {0.29222623},
    {0.4000595}
};
const double B1[LAYER_1_OUTDIM] = {0.02701721};
/*
*   return arr[r][c]
*/
double _getWeight(double* arr,int r,int rNum,int c)
{
    return *(arr + r*rNum + c);
}
/*
*   NNLayer初始化
*   绑定每层的权重矩阵、偏置矩阵
*   设置每层的输入维度，输出维度
*   绑定每层的输出暂存数组
*/
void NNLayerInit()
{
  //  gMyCalculatorModel[0].weight = W0;
    gMyCalculatorModel[0].bais = B0;
    gMyCalculatorModel[0].inDim = LAYER_0_INDIM;
    gMyCalculatorModel[0].outDim = LAYER_0_OUTDIM;
    gMyCalculatorModel[0].outVal = gLayer0OutVal;
 //   gMyCalculatorModel[1].weight = W1;
    gMyCalculatorModel[1].bais =B1;
    gMyCalculatorModel[1].inDim = LAYER_1_INDIM;
    gMyCalculatorModel[1].outDim = LAYER_1_OUTDIM;
    gMyCalculatorModel[1].outVal = gLayer1OutVal;
}
/*
*   NNLayer进行预测
*   model - NNLayer数组
*   x - 输入向量，数目符合LAYER_0_INDIM
*/
void NNLayerPredict(NNLayer* model,double* x)
{
    int _indexW,_indexX,_indexLayer;
    for(_indexLayer = 0 ; _indexLayer < LAYER_TOTAL ; _indexLayer++)
    {
        for(_indexW = 0 ; _indexW < model[_indexLayer].outDim ; _indexW++)
        {
            for(_indexX = 0 ; _indexX < model[_indexLayer].inDim ; _indexX++)
            {
                if(_indexLayer == 0)
                {
        //            model[_indexLayer].outVal[_indexW] += x[_indexX] * _getWeight(model[_indexLayer].weight,_indexX,model[_indexLayer].outDim,_indexW);
                }
                else
                {
          //          model[_indexLayer].outVal[_indexW] += model[_indexLayer - 1].outVal[_indexX] * _getWeight(model[_indexLayer].weight,_indexX,model[_indexLayer].outDim,_indexW);
                }

            }
            model[_indexLayer].outVal[_indexW] += model[_indexLayer].bais[_indexW];
        }
    }
}

