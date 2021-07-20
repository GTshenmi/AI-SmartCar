/*
 * NNLib.c
 *
 *  Created on: 2021��3��27��
 *      Author: 936305695
 */
#include "NNLib.h"

double Layer0OutVal[140];

double Layer1OutVal[100];

double Layer2OutVal[40];

double Layer3OutVal[1];

NNLayer_t TestModel[4]; // 4 ��

NNLayer_t TrackingModel[4];

NNLayer_t ELementModel[4];

/*
*   return arr[r][c]
*/
double _getWeight(double* arr,int r,int rNum,int c)
{
    return *(arr + r*rNum + c);
}

double SigmoidActiveFunc(double input)
{
    return 1/(1 + exp(-input));
}

double TanhActiveFunc(double input)
{
    return (exp(input) - exp(-input))/(exp(input) + exp(-input));
}

double ReluActiveFunc(double input)
{
    return max(0,input);
}

double ReluActiveFuncLimit8(double input)
{
    double res = max(0,input);

    res = ConstrainFloat(res,0.0,8.0);

    return res;
}

/*
*   NNLayer��ʼ��
*   ��ÿ���Ȩ�ؾ���ƫ�þ���
*   ����ÿ�������ά�ȣ����ά��
*   ��ÿ�������ݴ�����
*/
void NNInit(NNLayer_t* model)
{
    TestModel[0].weight = (double *)W0;
    TestModel[0].bais = B0;
    TestModel[0].inDim = 7;
    TestModel[0].outDim = 140;
    TestModel[0].outVal = Layer0OutVal;

    TestModel[1].weight = (double *)W1;
    TestModel[1].bais =B1;
    TestModel[1].inDim = 140;
    TestModel[1].outDim = 100;
    TestModel[1].outVal = Layer1OutVal;

    TestModel[2].weight = (double *)W2;
    TestModel[2].bais =B2;
    TestModel[2].inDim = 100;
    TestModel[2].outDim = 40;
    TestModel[2].outVal = Layer2OutVal;

    TestModel[3].weight = (double *)W3;
    TestModel[3].bais =B3;
    TestModel[3].inDim = 40;
    TestModel[3].outDim = 1;
    TestModel[3].outVal = Layer3OutVal;

    TestModel[0].Active = ReluActiveFunc;
    TestModel[1].Active = ReluActiveFunc;
    TestModel[2].Active = ReluActiveFuncLimit8;
    TestModel[3].Active = NULL;
}
/*
 *  @Brief:     NN Forward Reasoning Func.
 *  @Parameter: model:
 *  @Parameter: input:
 *  @Return:    result
 */
float NNForWardReasoning(NNLayer_t* model,double* input,uint32_t layer_num)
{
    int _indexW,_indexX,_indexLayer;

    for(_indexLayer = 0 ; _indexLayer < layer_num ; _indexLayer++)  //����ÿһ��
    {
        for(_indexW = 0 ; _indexW < model[_indexLayer].outDim ; _indexW++)  //�����ò��������Ԫ
        {
            model[_indexLayer].outVal[_indexW] = 0.0;

            for(_indexX = 0 ; _indexX < model[_indexLayer].inDim ; _indexX++)   //����Ȩ�ؼ���ÿ����Ԫ������
            {
                if(_indexLayer == 0)
                {
                    model[_indexLayer].outVal[_indexW] += (double)(input[_indexX] * _getWeight((double *)model[_indexLayer].weight,_indexX,model[_indexLayer].outDim,_indexW));
                }
                else
                {
                    model[_indexLayer].outVal[_indexW] += (double)(model[_indexLayer - 1].outVal[_indexX] * _getWeight((double *)model[_indexLayer].weight,_indexX,model[_indexLayer].outDim,_indexW));
                }
            }

            model[_indexLayer].outVal[_indexW] += model[_indexLayer].bais[_indexW]; //����ƫ��

            if(model[_indexLayer].Active != NULL)
                model[_indexLayer].outVal[_indexW] = model[_indexLayer].Active(model[_indexLayer].outVal[_indexW]);//�����
        }
    }

    return model[_indexLayer - 1].outVal[_indexW - 1]; //Output

    //return Layer3OutVal[0];
}




