/*
 * control.c
 *
 *  Created on: 2020��12��6��
 *      Author: 936305695
 *  @Brief:
 *      This file is for motor and servo close-loop control.
 */
#include <control.h>

#include "include.h"

/*          AI Mode        */
/*
 * @Brief:  ����ջ�ִ��(�ٶȻ�/������) ��Motor.SetSpeed��
 * @Attention:AIģʽ
 * */
void Motor_ExecutionUnitRun_ALLMode(struct unit *self,void *data)
{
    data_t *usrdata =(data_t *) data;

    if(self->State == CtrlSys_Running)
    {
        Motor.SetSpeed(Motor.Self,usrdata->Speed);
    }
}
/*
 * @Brief:  ����ջ�ִ��(�ǶȻ�) ��Servo.SetAngle��
 * @Attention:AIģʽ
 * */
void Servo_ExecutionUnitRun_ALLMode(struct unit *self,void *data)
{
    data_t *usrdata =(data_t *) data;

    if(self->State == CtrlSys_Running)
    {
        Servo.SetAngle(Servo.Self,usrdata->Angle);
    }
}

/*
 * @Brief:  ����ջ����ƺ���(�ٶȻ�/������)
 * @output: PwmValue��[0:10000]
 * @Attention:AIģʽ
 * */
unsigned short MotorCtrlStrategy_ALLMode(struct motor_ctrl *self,signed short target_speed,signed short actual_speed,void *usr)
{
    uint16_t PwmValue = 0;

    data_t *data =(data_t *) usr;

    PID_Ctrl(&data->M_PID,target_speed * 1.0,actual_speed * 1.0);

    PwmValue = (uint16_t)data->M_PID.Result;

    return PwmValue;
}
/*
 * @Brief:  ����ջ����ƺ���(�ǶȻ�)
 * @output: Angle��[-90(��):90(��)]
 * @Attention:AIģʽ
 * */
unsigned short ServoCtrlStrategy_ALLMode(struct servo_ctrl *self,signed short target_angle,float actual_angle,void *usr)
{

    return - target_angle + self->PwmCentValue;

}

