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

/*          ALL Mode        */
/*
 * @Brief:  ����ջ�ִ��(�ٶȻ�/������) ��Motor.SetSpeed��
 * @Attention:ALLģʽ
 * */
void Motor_ExecutionUnitRun(struct unit *self,void *data)
{
    data_t *usrdata =(data_t *) data;

    if(self->State == CtrlSys_Running)
    {
        Motor.SetSpeed(Motor.Self,usrdata->Speed);
    }
}
/*
 * @Brief:  ����ջ�ִ��(�ǶȻ�) ��Servo.SetAngle��
 * @Attention:ALLģʽ
 * */
void Servo_ExecutionUnitRun(struct unit *self,void *data)
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
 * @Attention:ALLģʽ
 * */
unsigned short MotorCtrlStrategy(struct motor_ctrl *self,signed short target_speed,signed short actual_speed,void *usr)
{
    uint16_t PwmValue = 0;

    data_t *data =(data_t *) usr;

    float tspeed,aspeed = 0.0;

    tspeed = 100.0 * NormalizeFloat(target_speed * 1.0,self->MaxSpeed * 1.0,0.0);
    aspeed = 100.0 * NormalizeFloat(actual_speed * 1.0,self->MaxSpeed * 1.0,0.0);

    if(fabs(data->M_PID.PID_Error[2]) > 20.0)
    {
        PwmValue = (uint16_t)(PWMx.MaxPwm * fsign(data->M_PID.PID_Error[2]));
    }
    else
    {
        PID_Ctrl(&data->M_PID,tspeed,aspeed);
        PwmValue = (uint16_t)data->M_PID.Result;
    }

    return PwmValue;
}
/*
 * @Brief:  ����ջ����ƺ���(�ǶȻ�)
 * @output: Angle��[-90(��):90(��)]
 * @Attention:ALLģʽ
 * */
unsigned short ServoCtrlStrategy(struct servo_ctrl *self,signed short target_angle,float actual_angle,void *usr)
{

    return - target_angle + self->PwmCentValue;

}

