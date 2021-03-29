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


/*          Common     ALL Mode                */
/*
 * @Brief:  电机相关传感器(编码器)数据获取
 * @Attention:ALL模式
 * */
void Motor_SensorUnitRun(struct unit *self,void *argv,uint16_t argc)
{
    data_t *data = (data_t *)argv;

    if(self->State == CtrlSys_Running)
    {
        data->Actual_Speed = Motor.GetSpeed(Motor.Self);
    }
}
/*
 * @Brief:  舵机相关传感器(摄像头/电感)数据获取
 * @Attention:ALL模式
 * */
void Servo_SensorUnitRun(struct unit *self,void *argv,uint16_t argc)
{
    data_t *data = (data_t *)argv;
    if(self->State == CtrlSys_Running)
    {
        for(int i = 0; i < CData.MaxLADCDeviceNum ; i++)
            data->LADC_Value[i] = LESensor[i].Read(LESensor[i].Self);
        for(int i = 0; i < CData.MaxSADCDeviceNum ; i++)
            data->SADC_Value[i] = SESensor[i].Read(SESensor[i].Self);

        /*归一化*/
        for(int i = 0 ; i < CData.MaxLADCDeviceNum ; i++)
            data->N_LADC[i] = 100.0 * NormalizeFloat(data->LADC_Value[i] * 1.0,ADCx.MinValue * 1.0,ADCx.MaxValue * 1.0);
        for(int i = 0 ; i < CData.MaxSADCDeviceNum ; i++)
            data->N_SADC[i] = 100.0 * NormalizeFloat(data->SADC_Value[i] * 1.0,ADCx.MinValue * 1.0,ADCx.MaxValue * 1.0);


        //ANO.Send("%f",data->N_LADC[0]);

        /*通知神经网络开始解算*/
        if(data->AI_State == AI_Free)
            data->AI_State = AI_Start;

        switch(data->ReportSensorData)
        {
            case 1:
                ANO.SendFloats(7,&data->N_SADC[0],&data->N_SADC[1],&data->N_SADC[2],&data->N_SADC[3],&data->N_SADC[4],&data->N_SADC[5],&data->N_SADC[6]);
                break;
            case 2:
                ANO.SendFloats(5,&data->N_LADC[0],&data->N_LADC[1],&data->N_LADC[2],&data->N_LADC[3],&data->N_LADC[4]);
                break;
            case 3:
                ANO.SendUint16s(7,&data->SADC_Value[0],&data->SADC_Value[1],&data->SADC_Value[2],&data->SADC_Value[3],&data->SADC_Value[4],&data->SADC_Value[5],&data->SADC_Value[6]);
                break;
            case 4:
                ANO.SendUint16s(5,&data->LADC_Value[0],&data->LADC_Value[1],&data->LADC_Value[2],&data->LADC_Value[3],&data->LADC_Value[4]);
                break;
            default:
                break;
        }
    }
}









/*
 * @Brief:  数据处理及速度决策(差速/动态速度)
 * @Attention:AI模式
 * @Output: Speed
 * */
void Motor_DecisionUnitRun_AIMode(struct unit *self,void *argv,uint16_t argc)
{
    data_t *data = (data_t *)argv;

    if(self->State == CtrlSys_Running)
    {
        data->Speed = 2000;
    }
}
/*
 * @Brief:  数据处理及角度决策
 * @Output: Angle
 * @Attention:AI模式
 * */
void Servo_DecisionUnitRun_AIMode(struct unit *self,void *argv,uint16_t argc)
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











/*               AutoBoot Mode                  */
/*
 * @Brief:  数据处理及速度决策(差速/动态速度)
 * @Output: Speed
 * @Attention:自动引导模式
 * */
void Motor_DecisionUnitRun_AutoBootMode(struct unit *self,void *argv,uint16_t argc)
{
    data_t *data = (data_t *)argv;

    if(self->State == CtrlSys_Running)
    {
        data->Speed = 2000;
    }
}

float CalculateDistance(void *argv)
{
    data_t *data = (data_t *)argv;

    float distance = 0.0;

    distance = CalculateDistanceDifDivSum(data->N_LADC[0],data->N_LADC[4]);

    return distance;
}
/*
 * @Brief:  数据处理及角度决策
 * @Output: Angle
 * @Attention:自动引导模式
 * */
void Servo_DecisionUnitRun_AutoBootMode(struct unit *self,void *argv,uint16_t argc)
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













/*         ManualBoot Mode                */
/*
 * @Brief:  数据处理及速度决策(差速/动态速度)
 * @Output: Speed
 * @Attention:手动引导模式
 * */
void Motor_DecisionUnitRun_ManualBootMode(struct unit *self,void *argv,uint16_t argc)
{
    data_t *data = (data_t *)argv;

    if(self->State == CtrlSys_Running)
    {
        data->Speed = 2000;
    }
}
/*
 * @Brief:  数据处理及角度决策
 * @Output: Angle
 * @Attention:手动引导模式
 * */
void Servo_DecisionUnitRun_ManualBootMode(struct unit *self,void *argv,uint16_t argc)
{
    data_t *data = (data_t *)argv;

    if(self->State == CtrlSys_Running)
    {
        float dis = 0.0;

        /*ADC Value -> dis*/
        data->S_PID.Kp += dis * dis * 0.1;

        PID_Ctrl(&data->S_PID,0,data->Bias);

        data->Angle = (uint16_t)data->S_PID.Result;
    }
}












/*         Debug Mode                */
/*
 * @Brief:  数据处理及速度决策(差速/动态速度)
 * @Output: Speed
 * @Attention:调试模式
 * */
void Motor_DecisionUnitRun_DebugMode(struct unit *self,void *argv,uint16_t argc)
{
   // data_t *data = (data_t *)argv;

    if(self->State == CtrlSys_Running)
    {

    }
}
/*
 * @Brief:  数据处理及角度决策
 * @Output: Angle
 * @Attention:调试模式
 * */
void Servo_DecisionUnitRun_DebugMode(struct unit *self,void *argv,uint16_t argc)
{
 //   data_t *data = (data_t *)argv;

    if(self->State == CtrlSys_Running)
    {

    }
}

