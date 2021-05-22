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

void GetESensorData(void *argv)
{
    data_t *data = (data_t *)argv;
    for(int i = 0; i < CData.MaxLADCDeviceNum ; i++)
        data->LESensor_SampleValue[i] = LESensor[i].Read(LESensor[i].Self);
    for(int i = 0; i < CData.MaxSADCDeviceNum ; i++)
        data->SESensor_SampleValue[i] = SESensor[i].Read(SESensor[i].Self);
}

void ESensorDataProcess(void *argv)
{
    data_t *data = (data_t *)argv;

    //static float bias[5] = {0.0,0.0,0.0,0.0,0.0};
    //static float Kb[5] = {0.3,0.3,0.2,0.1,0.1};

    /*归一化*/
    for(int i = 0 ; i < CData.MaxLADCDeviceNum ; i++)
        data->LESensor_NormalizedValue[i] = 100.0 * NormalizeFloat(data->LESensor_SampleValue[i] * 1.0,ADCx.MinValue * 1.0,ADCx.MaxValue * 1.0);
    for(int i = 0 ; i < CData.MaxSADCDeviceNum ; i++)
        data->SESensor_NormalizedValue[i] = 100.0 * NormalizeFloat(data->SESensor_SampleValue[i] * 1.0,ADCx.MinValue * 1.0,ADCx.MaxValue * 1.0);

    data->Bias = CalculateBias(data);

    //data->Bias = FIR_Filter(Kb,bias,data->_Bias,5);
}


#define LESensor_Min 10.0

float CalculateDistance(float L1,float L2)
{
    return CalculateDistanceDifDivSum(L1,L2);
}


float CalculateBias(void *argv)     /*Calculate Bias And Element Type.*/
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

    static float bias = 0.0;

    data->H_ESensorValue[0] = data->LESensor_NormalizedValue[1];
    data->H_ESensorValue[1] = data->LESensor_NormalizedValue[3];
    data->H_ESensorValue[2] = data->LESensor_NormalizedValue[5];

    data->V_ESensorValue[0] = data->LESensor_NormalizedValue[0];
    data->V_ESensorValue[1] = data->LESensor_NormalizedValue[6];

    data->O_ESensorValue[0] = data->LESensor_NormalizedValue[2];
    data->O_ESensorValue[1] = data->LESensor_NormalizedValue[4];

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
    data->h_bias = 100.0 * CalculateDistance(data->H_ESensorValue[0],data->H_ESensorValue[2]);

    data->h_difference = data->H_ESensorValue[0] - data->H_ESensorValue[2];

    data->h_sum = data->H_ESensorValue[0] + data->H_ESensorValue[2];


    if(data->H_ESensorValue[0] > LESensor_Min && data->H_ESensorValue[1] > LESensor_Min && data->H_ESensorValue[2] > LESensor_Min) //正常巡线
    {
        data->HTrackingState = Normal_Tracking;
    }
    else    //丢线
    {

        data->HTrackingState = LoseLine;
    }
    
    //data->h_bias = CalculateDistance(data->H_ESensorValue[0],data->H_ESensorValue[2]);
    /*for V ESensor:*/

    /*
        垂直电感: 直角弯和大弯
        左  右

        bias = (左 - 右) / (左 + 右)
     */

    data->v_bias = 100.0 * CalculateDistance(data->V_ESensorValue[0],data->V_ESensorValue[1]);
    data->v_difference = data->V_ESensorValue[0] - data->V_ESensorValue[1];
    data->v_sum = data->V_ESensorValue[0] + data->V_ESensorValue[1];

    if(data->V_ESensorValue[0] > LESensor_Min && data->V_ESensorValue[1] > LESensor_Min)
    {
        data->VTrackingState = Normal_Tracking;
    }
    else
    {
        data->VTrackingState = LoseLine;
    }

    
    /*for O ESensor:*/
    /*

        斜电感: 
        左  右

        bias = (左 - 右) / (左 + 右)
     */
    data->o_bias = 100.0 * CalculateDistance(data->O_ESensorValue[0],data->O_ESensorValue[1]);
    data->o_difference = data->O_ESensorValue[0] - data->O_ESensorValue[1];
    data->o_sum = data->O_ESensorValue[0] + data->O_ESensorValue[1];

    /*Calculate Bias:*/

    float weight = 0.0;

//    if(Is_Zero(data->h_bias) && Is_Zero(data->v_bias))
//    {
//        bias = 0.0;
//    }
//    else
//    {
//
//
//        if(((fabs(data->v_bias) / fabs(data->h_bias) > 2.0)&& (fabs(data->v_bias) >= 25.0))|| (fabs(data->v_bias) >= 30.0))
//        {
//            weight = fabs(data->h_bias)/fabs(data->v_bias);
//
//            //weight = weight * weight * 0.5;
//
//            bias = weight * data->h_bias + (1.0 - weight) * data->v_bias;
//
//            bias = data->v_bias;
//        }
////        else if(fabs(data->h_bias) / fabs(data->v_bias) > 1.2)
////        {
////            //weight = fabs(data->v_bias)/fabs(data->h_bias);
////
////            bias = data->h_bias;
////        }
//        else
//        {
//            bias = data->h_bias;
//        }
//    }

    /*元素判别*/
    if(data->HTrackingState == LoseLine && data->VTrackingState == LoseLine && data->TrackingState == Normal_Tracking)
    {
        data->TrackingState = LoseLine;
    }

    bias = data->h_bias;

//    float trackingState = data->TrackingState * 1.0;



//    EQueue.Puts(&data->EQueue,data->LESensor_NormalizedValue,0,7,false);
//    EQueue.Puts(&data->EQueue,&bias,7,8,false);
//    EQueue.Puts(&data->EQueue,&trackingState,8,9,true);
//
//    float *edata = EQueue.Gets(&data->EQueue,0,NULL,0,7);
//
//    Console.WriteArray("float",edata,7);

    /*  Debug  */

//    DebugCopy(data->VBias,data->v_bias);
//    DebugCopy(data->HBias,data->h_bias);
//    DebugCopy(data->Weight,weight);

    return bias;
}

float CalculateSpeed(void *argv)
{

    return 0.0;
}

//        if(data->H_ESensorValue[0] > data->H_ESensorValue[1] && data->H_ESensorValue[0] > data->H_ESensorValue[2]) //左电感最大
//        {
//                data->h_bias = CalculateDistance(data->H_ESensorValue[0],data->H_ESensorValue[2]);
//
//                data->h_difference = data->H_ESensorValue[0] - data->H_ESensorValue[2];
//
//                data->h_sum = data->H_ESensorValue[0] + data->H_ESensorValue[2];
//        }
//
//        if(data->H_ESensorValue[2] > data->H_ESensorValue[1] && data->H_ESensorValue[2] > data->H_ESensorValue[0]) //右电感最大
//        {
//
//                data->h_bias = CalculateDistance(data->H_ESensorValue[0],data->H_ESensorValue[2]);
//
//                data->h_difference = data->H_ESensorValue[0] - data->H_ESensorValue[2];
//
//                data->h_sum = data->H_ESensorValue[0] + data->H_ESensorValue[2];
//
//        }
//
//        if(data->H_ESensorValue[1] > data->H_ESensorValue[2] && data->H_ESensorValue[1] > data->H_ESensorValue[0]) //中电感最大
//        {
//                data->h_bias = CalculateDistance(data->H_ESensorValue[0],data->H_ESensorValue[2]);
//
//                data->h_difference = data->H_ESensorValue[0] - data->H_ESensorValue[2];
//
//                data->h_sum = data->H_ESensorValue[0] + data->H_ESensorValue[2];
//        }

// float CalculateBias(void *argv)
// {
// /*
//  * SESensor  : [0]  [1]  [2]  [3]  [4]  [5]  [6]
//  * Direction :  |   ---   \   ---   /   ---   |
//  * ESensor   : (-100.0,100.0)
//  * Distance  : (-1.0,1.0)
//  * Problem   : 1.直角切内弯会反方向误判
//  *
//  * */

//     data_t *data = (data_t *)argv;

//     static float h_difference = 0.0;
//     static float v_difference = 0.0;
//     static float o_difference = 0.0;

//     static float h_sum = 0.0;
//     static float v_sum = 0.0;
//     static float o_sum = 0.0;

//     static float h_bias = 0.0;
//     static float v_bias = 0.0;
//     static float o_bias = 0.0;

//     static float bias = 0.0;

//     static float H_ESensorValue[3];
//     static float V_ESensorValue[2];
//     static float O_ESensorValue[2];

//     H_ESensorValue[0] = data->LESensor_NormalizedValue[1];
//     H_ESensorValue[1] = data->LESensor_NormalizedValue[3];
//     H_ESensorValue[2] = data->LESensor_NormalizedValue[5];

//     V_ESensorValue[0] = data->LESensor_NormalizedValue[0];
//     V_ESensorValue[1] = data->LESensor_NormalizedValue[6];

//     O_ESensorValue[0] = data->LESensor_NormalizedValue[2];
//     O_ESensorValue[1] = data->LESensor_NormalizedValue[4];

//     /*for H ESensor:*/

//     /*
//         水平电感: 直道和小弯
//         左  中  右

//         if(左 Max)
//         {
//             if(中 > 右)
//                 正常：

//                 中 ~= 右 ~= 0  :    右  严重

//                                     bias = 1.0

//                 中 > 右 :            右  中等   

//                                     bias = (左 - 中)/(左 + 中)      
//             else(右 > 中)
//                 异常：有其他元素干扰
//         }

//         if(中 Max)
//         {
//             正常:
//                 if 左 和 右 相差不大：
//                     bias = 0；
//                 else
//                     bias = (左 - 右)/(左 + 右)              
//         }


//         if(右 Max)
//         {
//             if(中 > 左)
//                 正常：

//                 中 ~= 左 ~= 0  :     左  严重

//                                     bias = -1.0

//                 中 > 左 :            左  中等   

//                                     bias = (中 - 右)/(中 + 右)

//             else(左 > 中)
//                 异常：有其他元素干扰    
//         }

//      */
//     if(H_ESensorValue[0] > LESensor_Min && H_ESensorValue[1] > LESensor_Min && H_ESensorValue[2] > LESensor_Min) //正常巡线
//     {
//         if(H_ESensorValue[0] > H_ESensorValue[1] && H_ESensorValue[0] > H_ESensorValue[2]) //左电感最大
//         {
//             //if(H_ESensorValue[1] > H_ESensorValue[2])
//             {
// //                if(Is_ApproximatelyEqual(H_ESensorValue[1],0.0,10.0) && Is_ApproximatelyEqual(H_ESensorValue[2],0.0,10.0))
// //                {
// //                    h_bias = 1.0;
// //                }
// //                else
//                 {
//                     h_bias = CalculateDistance(H_ESensorValue[0],H_ESensorValue[2]);
//                 }

//                 h_difference = H_ESensorValue[0] - H_ESensorValue[2];

//                 h_sum = H_ESensorValue[0] + H_ESensorValue[2];
//             }
//             //else
//             {
//                 //存在元素干扰
//             }
//         }

//         if(H_ESensorValue[2] > H_ESensorValue[1] && H_ESensorValue[2] > H_ESensorValue[0]) //右电感最大
//         {
//             //if(H_ESensorValue[1] > H_ESensorValue[0])
//             {
// //                if(Is_ApproximatelyEqual(H_ESensorValue[0],0.0,10.0) && Is_ApproximatelyEqual(H_ESensorValue[1],0.0,10.0))
// //                {
// //                    h_bias = -1.0;
// //
// //                }
// //                else
//                 {
//                     h_bias = CalculateDistance(H_ESensorValue[0],H_ESensorValue[2]);
//                 }

//                 h_difference = H_ESensorValue[0] - H_ESensorValue[2];

//                 h_sum = H_ESensorValue[0] + H_ESensorValue[2];
//             }
//             //else
//             {
//                 //存在元素干扰
//             }
//         }

//         if(H_ESensorValue[1] > H_ESensorValue[2] && H_ESensorValue[1] > H_ESensorValue[0]) //中电感最大
//         {
// //            if(Is_ApproximatelyEqual(H_ESensorValue[0],H_ESensorValue[2],10.0))
// //            {
// //                bias = 0.0;
// //            }
// //            else
//             {
//                 bias = CalculateDistance(H_ESensorValue[0],H_ESensorValue[2]);
//             }
            
//             h_difference = H_ESensorValue[0] - H_ESensorValue[2];

//             h_sum = H_ESensorValue[0] + H_ESensorValue[2];
//         }

//         //data->Speed = 3000;
//     }
//     else    //丢线
//     {
//         //data->Speed = -3000;
//     }
    

//     /*for V ESensor:*/

//     /*
//         垂直电感: 直角弯和大弯
//         左  右

//         bias = (左 - 右) / (左 + 右)
//      */
//     if(V_ESensorValue[0] < 15.0 && V_ESensorValue[1] < 15.0)
//     {
//         V_ESensorValue[0] = 0.0;
//         V_ESensorValue[1] = 0.0;
//     }

//      v_bias = CalculateDistance(V_ESensorValue[0],V_ESensorValue[1]);
//      v_difference = V_ESensorValue[0] - V_ESensorValue[1];
//      v_sum = V_ESensorValue[0] + V_ESensorValue[1];

    
//     /*for O ESensor:*/
//     /*
//         斜电感: 
//         左  右

//         bias = (左 - 右) / (左 + 右)
//      */
//     o_bias = CalculateDistance(O_ESensorValue[0],O_ESensorValue[1]);
//     o_difference = O_ESensorValue[0] - O_ESensorValue[1];
//     o_sum = O_ESensorValue[0] + O_ESensorValue[1];

//     /*Calculate Bias:*/

//     float weight = 0.0;


//     if(Is_Zero(h_bias) && Is_Zero(v_bias))
//     {
//         bias = 0.0;
//     }
//     else
//     {
//         if(fabs(h_bias) / fabs(v_bias) > 1.2)
//         {
//             weight = fabs(v_bias)/fabs(h_bias);

//             bias = (1 - weight) * h_bias + weight * v_bias;
//         }
//         else if((v_bias) / fabs(h_bias) > 1.2)
//         {
//             weight = fabs(h_bias)/fabs(v_bias);

//             //h_bias /= 2.0;

//             //weight = weight * weight * 0.36;

//             bias = weight * h_bias + (1 - weight) * v_bias;
//         }
//         else
//         {
//             bias = h_bias * 0.7 + v_bias * 0.3;
//         }
// //        else
// //        {
// //            bias = (v_bias + h_bias)/2.0;
// //        }
//     }

//     //if()
//     {

//     }
//     bias = h_bias;
//     /*  Debug  */

//     DebugCopy(data->VBias,v_bias);
//     DebugCopy(data->HBias,h_bias);
//     DebugCopy(data->Weight,weight);

//     return bias;
// }

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
