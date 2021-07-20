/*
 * dataprocess.c
 *
 *  Created on: 2020��12��6��
 *      Author: 936305695
 *  @Brief:
 *      This file is for data process(read/process).
 *      ���ݶ�ȡ/���� ����ӳ���
 */
#include <dataprocess.h>

#include "include.h"

/*          Common     ALL Mode                */
/*
 * @Brief:  �����ش�����(������)���ݻ�ȡ
 * @Attention:ALLģʽ
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
 * @Brief:  �����ش�����(����ͷ/���)���ݻ�ȡ
 * @Attention:ALLģʽ
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
 * @Brief:  ���ݴ����ٶȾ���(����/��̬�ٶ�)
 * @Attention:AIģʽ
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
 * @Brief:  ���ݴ����ǶȾ���
 * @Output: Angle
 * @Attention:AIģʽ
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
 * @Brief:  ���ݴ����ٶȾ���(����/��̬�ٶ�)
 * @Output: Speed
 * @Attention:�Զ�����ģʽ
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
 * @Brief:  ���ݴ����ǶȾ���
 * @Output: Angle
 * @Attention:�Զ�����ģʽ
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
 * @Brief:  ���ݴ����ٶȾ���(����/��̬�ٶ�)
 * @Output: Speed
 * @Attention:�ֶ�����ģʽ
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
 * @Brief:  ���ݴ����ǶȾ���
 * @Output: Angle
 * @Attention:�ֶ�����ģʽ
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
 * @Brief:  ���ݴ����ٶȾ���(����/��̬�ٶ�)
 * @Output: Speed
 * @Attention:����ģʽ
 * */
void Motor_DecisionUnitRun_DebugMode(struct unit *self,void *data)
{
   // data_t *usrdata = (data_t *)data;

    if(self->State == CtrlSys_Running)
    {

    }
}
/*
 * @Brief:  ���ݴ����ǶȾ���
 * @Output: Angle
 * @Attention:����ģʽ
 * */
void Servo_DecisionUnitRun_DebugMode(struct unit *self,void *data)
{
 //   data_t *usrdata = (data_t *)data;

    if(self->State == CtrlSys_Running)
    {

    }
}
