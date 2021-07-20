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

float CalculateDistance(void *argv)
{
/*
 * SESensor  : [0]  [1]  [2]  [3]  [4]  [5]  [6]
 * Direction :  |   ---   \   ---   /   ---   |
 * N_ADC     : (-100.0,100.0)
 * Distance  : (-1.0,1.0)
 *
 * */

    data_t *data = (data_t *)argv;

    static float difference_h = 0.0;
    static float difference_v = 0.0;
    static float difference_o = 0.0;

    static float sum_h = 0.0;
    static float sum_v = 0.0;
    static float sum_o = 0.0;

    static float distance_h = 0.0;
    static float distance_v = 0.0;
    static float distance_o = 0.0;

    static float distance = 0.0;

    if(data->N_LADC[1] > LESensor_Min && data->N_LADC[5] > LESensor_Min) //横电感差值
    {
        sum_h = data->N_LADC[1] + data->N_LADC[5];
        difference_h = data->N_LADC[1] - data->N_LADC[5];

        distance_h = CalculateDistanceDifDivSum(data->N_LADC[1],data->N_LADC[5]);
    }

    if(data->N_LADC[0] > LESensor_Min && data->N_LADC[6] > LESensor_Min) //竖电感差值
    {
        sum_v = data->N_LADC[0] + data->N_LADC[6];
        difference_v = data->N_LADC[0] - data->N_LADC[6];

        distance_v = CalculateDistanceDifDivSum(data->N_LADC[0],data->N_LADC[6]);
    }

    if(data->N_LADC[2] > LESensor_Min && data->N_LADC[4] > LESensor_Min) //斜电感差值
    {
        sum_o = data->N_LADC[2] + data->N_LADC[4];
        difference_o = data->N_LADC[2] - data->N_LADC[4];
        distance_o = CalculateDistanceDifDivSum(data->N_LADC[2],data->N_LADC[4]);
    }

    float weight = 0.0;


    if(Is_Zero(difference_h) && Is_Zero(difference_v))
    {
        distance = 0.0;
    }
    else
    {
        if(fabs(difference_h) > fabs(difference_v))
        {

            weight = fabs(difference_v)/fabs(difference_h);

            distance = (1 - weight) * distance_h + weight * distance_v;
        }
        else
        {
            weight = fabs(difference_h)/fabs(difference_v);

            distance = weight * distance_h + (1 - weight) * distance_v;
        }
    }

    if(data->N_LADC[3] > 80.0)
    {
        //环岛判别
    }

    uint index = FindMaxIndex(data->N_LADC,CData.MaxLADCDeviceNum);

    switch(index)
    {
        case 0:
            //distance = -1.0;
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            //distance = 1.0;
            break;
    }

    return distance;
}


/*
 * @Brief:  数据处理及角度决策
 * @Output: Angle
 * @Attention:AI模式
 * */
void Servo_DecisionUnitRun0(struct unit *self,void *argv,uint16_t argc)
{
    data_t *data = (data_t *)argv;

    if(self->State == CtrlSys_Running)
    {
        static float Ka[5] = {0.3,0.3,0.2,0.1,0.1};

        static float angle[5] = {0.0};

        float systimenow = os.time.getTime(ms);
        float dt = 0.0;

        /*等待神经网络解算完成*/
        while(data->AI_State != AI_Fin)
        {
            dt = os.time.getTime(ms) - systimenow;
            if(dt > 2.0)
                break;
        }

        if(data->AI_State == AI_Fin)
        {
            /*中线偏差大于20.0时(弯道)使用动态PID*/
            if(fabs(data->Bias) >= 20.0)
                data->S_PID.Kp += data->Bias * data->Bias * 0.000332;
            /*动态PID限幅*/
            if(data->S_PID.Kp > 0.8)
                data->S_PID.Kp = 0.8;

            PID_Ctrl(&data->S_PID,0,data->Bias);

            data->Angle = (uint16_t)(FIR_Filter(Ka,angle,data->S_PID.Result,5));

            /*神经网络重新解算*/
            data->AI_State = AI_Free;

        }
    }
}


/*
 * @Brief:  数据处理及角度决策
 * @Output: Angle
 * @Attention:自动引导模式
 * */
void Servo_DecisionUnitRun1(struct unit *self,void *argv,uint16_t argc)
{
    data_t *data = (data_t *)argv;

    if(self->State == CtrlSys_Running)
    {
        static float dis[33] = {0.0};
        static float Ka[5] = {0.3,0.3,0.2,0.1,0.1};
        static float Kb[33] = {
           0.004310506862, 0.006396806333,-0.008907295763,-0.006022877526,  0.01509535592,
           0.002957194578, -0.02246881649,  0.00418050494,   0.0303422641, -0.01742442511,
           -0.03784247488,  0.04116194695,  0.04404707253, -0.09106717259, -0.04814339057,
             0.3107235134,    0.545322597,   0.3107235134, -0.04814339057, -0.09106717259,
            0.04404707253,  0.04116194695, -0.03784247488, -0.01742442511,   0.0303422641,
            0.00418050494, -0.02246881649, 0.002957194578,  0.01509535592,-0.006022877526,
          -0.008907295763, 0.006396806333, 0.004310506862
        };
//        static float Kb2[33] = {
//                                0.00431, 0.006396, -0.0089, -0.00602, 0.015095,
//                                0.002957, -0.02246, 0.00418, 0.030342, -0.01742,
//                                -0.03784, 0.041161, 0.044047, -0.09106, -0.04814,
//                                0.310723, 0.545322, 0.310723, -0.04814, -0.09106,
//                                0.044047, 0.041161, -0.03784, -0.01742, 0.030342,
//                                0.00418, -0.02246, 0.002957, 0.015095, -0.00602,
//                                -0.0089, 0.006396, 0.00431
//        };
        static float angle[5] = {0.0};

        data->_Bias = 100.0 * CalculateDistance(argv);

        data->Bias = FIR_Filter(Kb,dis,data->_Bias,33);

        /*中线偏差大于20.0时(弯道)使用动态PID*/
        if(fabs(dis[4]) >= 20.0)
            data->S_PID.Kp += data->Bias * data->Bias * 0.000332;
        /*动态PID限幅*/
        if(data->S_PID.Kp > 0.8)
            data->S_PID.Kp = 0.8;

        PID_Ctrl(&data->S_PID,0,data->Bias);

        data->Angle = (uint16_t)(FIR_Filter(Ka,angle,data->S_PID.Result,5));

        SaveAutoBootModeDataToExcel(argv);
    }
}
