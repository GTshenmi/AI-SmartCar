/*
 * dataprocess.c
 *
 *  Created on: 2020年12月6日
 *      Author: 936305695
 *  @Brief:
 *      This file is for data process(read/process).
 *      数据读取/处理 相关子程序
 */
#include <dataprocess.h>
#include "include.h"

#define LESensor_Min 5.0

float CalculateDistance(float L1,float L2)
{
    return CalculateDistanceDifDivSum(L1,L2);
}

float CalculateBias(void *argv)
{
/*
 * SESensor  : [0]  [1]  [2]  [3]  [4]  [5]  [6]
 * Direction :  |   ---   \   ---   /   ---   |
 * ESensor   : (-100.0,100.0)
 * Distance  : (-1.0,1.0)
 * Problem   : 1.直角切内弯会反方向误判
 *
 * */

    data_t *data = (data_t *)argv;

    static float h_difference = 0.0;
    static float v_difference = 0.0;
    static float o_difference = 0.0;

    static float h_sum = 0.0;
    static float v_sum = 0.0;
    static float o_sum = 0.0;

    static float h_bias = 0.0;
    static float v_bias = 0.0;
    static float o_bias = 0.0;

    static float bias = 0.0;

    static float H_ESensorValue[3];
    static float V_ESensorValue[2];
    static float O_ESensorValue[2];

    H_ESensorValue[0] = data->LESensor_NormalizedValue[1];
    H_ESensorValue[1] = data->LESensor_NormalizedValue[3];
    H_ESensorValue[2] = data->LESensor_NormalizedValue[5];

    V_ESensorValue[0] = data->LESensor_NormalizedValue[0];
    V_ESensorValue[1] = data->LESensor_NormalizedValue[6];

    O_ESensorValue[0] = data->LESensor_NormalizedValue[2];
    O_ESensorValue[1] = data->LESensor_NormalizedValue[4];

    /*for H ESensor:*/

    /*
        水平电感: 直道和小弯
        左  中  右

        if(左 Max)
        {
            if(中 > 右)
                正常：

                中 ~= 右 ~= 0  :    右  严重

                                    bias = 1.0

                中 > 右 :            右  中等   

                                    bias = (左 - 中)/(左 + 中)      
            else(右 > 中)
                异常：有其他元素干扰
        }

        if(中 Max)
        {
            正常:
                if 左 和 右 相差不大：
                    bias = 0；
                else
                    bias = (左 - 右)/(左 + 右)              
        }


        if(右 Max)
        {
            if(中 > 左)
                正常：

                中 ~= 左 ~= 0  :     左  严重

                                    bias = -1.0

                中 > 左 :            左  中等   

                                    bias = (中 - 右)/(中 + 右)

            else(左 > 中)
                异常：有其他元素干扰    
        }

     */
    if(H_ESensorValue[0] > LESensor_Min && H_ESensorValue[1] > LESensor_Min && H_ESensorValue[2] > LESensor_Min) //正常巡线
    {
        if(H_ESensorValue[0] > H_ESensorValue[1] && H_ESensorValue[0] > H_ESensorValue[2]) //左电感最大
        {
            //if(H_ESensorValue[1] > H_ESensorValue[2])
            {
                if(Is_ApproximatelyEqual(H_ESensorValue[1],0.0,10.0) && Is_ApproximatelyEqual(H_ESensorValue[2],0.0,10.0))
                {
                    h_bias = 1.0;
                }
                else
                {
                    h_bias = CalculateDistance(H_ESensorValue[0],H_ESensorValue[2]);
                }

                h_difference = H_ESensorValue[0] - H_ESensorValue[2];

                h_sum = H_ESensorValue[0] + H_ESensorValue[2];
            }
            //else
            {
                //存在元素干扰
            }
        }

        if(H_ESensorValue[2] > H_ESensorValue[1] && H_ESensorValue[2] > H_ESensorValue[0]) //右电感最大
        {
            //if(H_ESensorValue[1] > H_ESensorValue[0])
            {
                if(Is_ApproximatelyEqual(H_ESensorValue[0],0.0,10.0) && Is_ApproximatelyEqual(H_ESensorValue[1],0.0,10.0))
                {
                    h_bias = -1.0;
                }
                else
                {
                    h_bias = CalculateDistance(H_ESensorValue[0],H_ESensorValue[2]);
                }

                h_difference = H_ESensorValue[0] - H_ESensorValue[2];

                h_sum = H_ESensorValue[0] + H_ESensorValue[2];
            }
            //else
            {
                //存在元素干扰
            }
        }

        if(H_ESensorValue[1] > H_ESensorValue[2] && H_ESensorValue[1] > H_ESensorValue[0]) //中电感最大
        {
//            if(Is_ApproximatelyEqual(H_ESensorValue[0],H_ESensorValue[2],10.0))
//            {
//                bias = 0.0;
//            }
//            else
            {
                bias = CalculateDistance(H_ESensorValue[0],H_ESensorValue[2]);
            }
            
            h_difference = H_ESensorValue[0] - H_ESensorValue[2];

            h_sum = H_ESensorValue[0] + H_ESensorValue[2];
        }
    }
    else    //丢线
    {
        
    }
    

    /*for V ESensor:*/

    /*
        垂直电感: 直角弯和大弯
        左  右

        bias = (左 - 右) / (左 + 右)
     */
    if(V_ESensorValue[0] < 15.0 && V_ESensorValue[1] < 15.0)
    {
        V_ESensorValue[0] = 0.0;
        V_ESensorValue[1] = 0.0;
    }

     v_bias = CalculateDistance(V_ESensorValue[0],V_ESensorValue[1]);
     v_difference = V_ESensorValue[0] - V_ESensorValue[1];
     v_sum = V_ESensorValue[0] + V_ESensorValue[1];

    
    /*for O ESensor:*/
    /*
        斜电感: 
        左  右

        bias = (左 - 右) / (左 + 右)
     */
    o_bias = CalculateDistance(O_ESensorValue[0],O_ESensorValue[1]);
    o_difference = O_ESensorValue[0] - O_ESensorValue[1];
    o_sum = O_ESensorValue[0] + O_ESensorValue[1];

    /*Calculate Bias:*/

    float weight = 0.0;


    if(Is_Zero(h_bias) && Is_Zero(v_bias))
    {
        bias = 0.0;
    }
    else
    {
        if(fabs(h_bias) / fabs(v_bias) > 1.2)
        {
            weight = fabs(v_bias)/fabs(h_bias);

            bias = (1 - weight) * h_bias + weight * v_bias;
        }
        else if((v_bias) / fabs(h_bias) > 1.2)
        {
            weight = fabs(h_bias)/fabs(v_bias);

            //h_bias /= 2.0;

            //weight = weight * weight * 0.36;

            bias = weight * h_bias + (1 - weight) * v_bias;
        }
        else
        {
            bias = h_bias * 0.7 + v_bias * 0.3;
        }
//        else
//        {
//            bias = (v_bias + h_bias)/2.0;
//        }
    }

    bias = h_bias;
    /*  Debug  */

    DebugCopy(data->VBias,v_bias);
    DebugCopy(data->HBias,h_bias);
    DebugCopy(data->Weight,weight);

    return bias;
}

// float CalculateBias(void *argv)
// {
// /*
//  * SESensor  : [0]  [1]  [2]  [3]  [4]  [5]  [6]
//  * Direction :  |   ---   \   ---   /   ---   |
//  * N_ADC     : (-100.0,100.0)
//  * Distance  : (-1.0,1.0)
//  *
//  * */

//     data_t *data = (data_t *)argv;

//     static float difference_h = 0.0;
//     static float difference_v = 0.0;
//     static float difference_o = 0.0;

//     static float sum_h = 0.0;
//     static float sum_v = 0.0;
//     static float sum_o = 0.0;

//     static float distance_h = 0.0;
//     static float distance_v = 0.0;
//     static float distance_o = 0.0;

//     static float distance = 0.0;

//     if(data->N_LADC[1] > LESensor_Min && data->N_LADC[5] > LESensor_Min) //横电感差值
//     {
//         sum_h = data->N_LADC[1] + data->N_LADC[5];
//         difference_h = data->N_LADC[1] - data->N_LADC[5];

//         distance_h = CalculateDistance(data->N_LADC[1],data->N_LADC[5]);
//     }

//     if(data->N_LADC[0] > LESensor_Min && data->N_LADC[6] > LESensor_Min) //竖电感差值
//     {
//         sum_v = data->N_LADC[0] + data->N_LADC[6];
//         difference_v = data->N_LADC[0] - data->N_LADC[6];

//         distance_v = CalculateDistance(data->N_LADC[0],data->N_LADC[6]);
//     }

//     if(data->N_LADC[2] > LESensor_Min && data->N_LADC[4] > LESensor_Min) //斜电感差值
//     {
//         sum_o = data->N_LADC[2] + data->N_LADC[4];
//         difference_o = data->N_LADC[2] - data->N_LADC[4];
//         distance_o = CalculateDistance(data->N_LADC[2],data->N_LADC[4]);
//     }

//     float weight = 0.0;


//     if(Is_Zero(difference_h) && Is_Zero(difference_v))
//     {
//         distance = 0.0;
//     }
//     else
//     {
//         if(fabs(difference_h) > fabs(difference_v))
//         {

//             weight = fabs(difference_v)/fabs(difference_h);

//             distance = (1 - weight) * distance_h + weight * distance_v;
//         }
//         else
//         {
//             weight = fabs(difference_h)/fabs(difference_v);

//             weight = weight * weight * 0.5;

//             distance = weight * distance_h + (1 - weight) * distance_v;
//         }
//     }

//     if(data->N_LADC[3] > 80.0)
//     {
//         //环岛判别
//     }

//     uint index = FindMaxIndex(data->N_LADC,CData.MaxLADCDeviceNum);

//     switch(index)
//     {
//         case 0:
//             //distance = -1.0;
//             break;
//         case 1:
//             break;
//         case 2:
//             break;
//         case 3:
//             break;
//         case 4:
//             break;
//         case 5:
//             //distance = 1.0;
//             break;
//     }

//     return distance;
// }
