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
void Motor_SensorUnitRun_ALLMode(struct unit *self,void *data)
{
    data_t *usrdata = (data_t *)data;

    if(self->State == CtrlSys_Running)
    {
        usrdata->Actual_Speed = Motor.GetSpeed(Motor.Self);
    }
}
/*
 * @Brief:  舵机相关传感器(摄像头/电感)数据获取
 * @Attention:ALL模式
 * */
void Servo_SensorUnitRun_ALLMode(struct unit *self,void *data)
{
    data_t *usrdata = (data_t *)data;
    if(self->State == CtrlSys_Running)
    {
        for(int i = 0; i < CData.MaxLADCDeviceNum ; i++)
            usrdata->LADC_Value[i] = LESensor[i].Read(LESensor[i].Self);
        for(int i = 0; i < CData.MaxSADCDeviceNum ; i++)
            usrdata->SADC_Value[i] = SESensor[i].Read(SESensor[i].Self);
    }
}









/*
 * @Brief:  数据处理及速度决策(差速/动态速度)
 * @Attention:AI模式
 * @Output: Speed
 * */
void Motor_DecisionUnitRun_AIMode(struct unit *self,void *data)
{
    data_t *usrdata = (data_t *)data;

    if(self->State == CtrlSys_Running)
    {
        usrdata->Speed = 2000;
    }
}
/*
 * @Brief:  数据处理及角度决策
 * @Output: Angle
 * @Attention:AI模式
 * */
void Servo_DecisionUnitRun_AIMode(struct unit *self,void *data)
{
    data_t *usrdata = (data_t *)data;

    if(self->State == CtrlSys_Running)
    {
        float dis = 0.0;

        /*Nerual Network Reasoning*/

        usrdata->S_PID.Kp += dis * dis * 0.1;

        PID_Ctrl(&usrdata->S_PID,0,usrdata->Position);

        usrdata->Angle = (uint16_t)usrdata->S_PID.Result;
    }
}











/*               AutoBoot Mode                  */
/*
 * @Brief:  数据处理及速度决策(差速/动态速度)
 * @Output: Speed
 * @Attention:自动引导模式
 * */
void Motor_DecisionUnitRun_AutoBootMode(struct unit *self,void *data)
{
    data_t *usrdata = (data_t *)data;

    if(self->State == CtrlSys_Running)
    {
        usrdata->Speed = 2000;
    }
}
/*
 * @Brief:  数据处理及角度决策
 * @Output: Angle
 * @Attention:自动引导模式
 * */
void Servo_DecisionUnitRun_AutoBootMode(struct unit *self,void *data)
{
    data_t *usrdata = (data_t *)data;

    if(self->State == CtrlSys_Running)
    {
        float dis = 0.0;

        /*AD Value -> dis*/

        usrdata->S_PID.Kp += dis * dis * 0.1;

        PID_Ctrl(&usrdata->S_PID,0,usrdata->Position);

        usrdata->Angle = (uint16_t)usrdata->S_PID.Result;
    }
}













/*         ManualBoot Mode                */
/*
 * @Brief:  数据处理及速度决策(差速/动态速度)
 * @Output: Speed
 * @Attention:手动引导模式
 * */
void Motor_DecisionUnitRun_ManualBootMode(struct unit *self,void *data)
{
    data_t *usrdata = (data_t *)data;

    if(self->State == CtrlSys_Running)
    {
        usrdata->Speed = 2000;
    }
}
/*
 * @Brief:  数据处理及角度决策
 * @Output: Angle
 * @Attention:手动引导模式
 * */
void Servo_DecisionUnitRun_ManualBootMode(struct unit *self,void *data)
{
    data_t *usrdata = (data_t *)data;

    if(self->State == CtrlSys_Running)
    {
        float dis = 0.0;

        /*ADC Value -> dis*/
        usrdata->S_PID.Kp += dis * dis * 0.1;

        PID_Ctrl(&usrdata->S_PID,0,usrdata->Position);

        usrdata->Angle = (uint16_t)usrdata->S_PID.Result;
    }
}












/*         Debug Mode                */
/*
 * @Brief:  数据处理及速度决策(差速/动态速度)
 * @Output: Speed
 * @Attention:调试模式
 * */
void Motor_DecisionUnitRun_DebugMode(struct unit *self,void *data)
{
   // data_t *usrdata = (data_t *)data;

    if(self->State == CtrlSys_Running)
    {

    }
}
/*
 * @Brief:  数据处理及角度决策
 * @Output: Angle
 * @Attention:调试模式
 * */
void Servo_DecisionUnitRun_DebugMode(struct unit *self,void *data)
{
 //   data_t *usrdata = (data_t *)data;

    if(self->State == CtrlSys_Running)
    {

    }
}
