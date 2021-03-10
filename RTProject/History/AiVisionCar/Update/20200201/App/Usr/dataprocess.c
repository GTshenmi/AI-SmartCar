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
void Motor_SensorUnitRun(struct unit *self,void *data)
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
void Servo_SensorUnitRun(struct unit *self,void *data)
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

        PID_Ctrl(&usrdata->S_PID,0,usrdata->Bias);

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

        /*��һ��*/
        for(int i = 0 ; i < CData.MaxLADCDeviceNum ; i++)
            usrdata->N_LADC[i] = NormalizeFloat(usrdata->LADC_Value[i] * 1.0,ADCx.MaxValue * 1.0,ADCx.MinValue * 1.0);
        for(int i = 0 ; i < CData.MaxSADCDeviceNum ; i++)
            usrdata->N_SADC[i] = NormalizeFloat(usrdata->SADC_Value[i] * 1.0,ADCx.MaxValue * 1.0,ADCx.MinValue * 1.0);

        usrdata->Bias = (sint16_t)FIR_Filter(Kb,dis,100.0 * CalculateDistanceDifDivSum(usrdata->N_LADC[0],usrdata->N_LADC[4]),33);

        /*����ƫ�����20.0ʱ(���)ʹ�ö�̬PID*/
        if(fabs(dis[4]) >= 20.0)
            usrdata->S_PID.Kp += usrdata->Bias * usrdata->Bias * 0.000332;
        /*��̬PID�޷�*/
        if(usrdata->S_PID.Kp > 0.8)
            usrdata->S_PID.Kp = 0.8;

        PID_Ctrl(&usrdata->S_PID,0,usrdata->Bias);

        usrdata->Angle = (uint16_t)(FIR_Filter(Ka,angle,usrdata->S_PID.Result,5));

        SaveAutoBootModeData(data);
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

        PID_Ctrl(&usrdata->S_PID,0,usrdata->Bias);

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

