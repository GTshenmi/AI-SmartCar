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
void Motor_ExecutionUnitRun(struct unit *self,void *argv,uint16_t argc)
{
    data_t *data =(data_t *) argv;

    if(self->State == CtrlSys_Running)
    {
        Motor.SetSpeed(Motor.Self,data->Speed);
    }
}
/*
 * @Brief:  ����ջ�ִ��(�ǶȻ�) ��Servo.SetAngle��
 * @Attention:ALLģʽ
 * */
void Servo_ExecutionUnitRun(struct unit *self,void *argv,uint16_t argc)
{
    data_t *data =(data_t *) argv;

    if(self->State == CtrlSys_Running)
    {
        Servo.SetAngle(Servo.Self,data->Angle);
    }
}

/*
 * @Brief:  ����ջ����ƺ���(�ٶȻ�/������)
 * @output: PwmValue��[0:10000]
 * @Attention:ALLģʽ
 * */
sint16_t MotorCtrlStrategy(struct motor_ctrl *self,signed short target_speed,signed short actual_speed,void *argv,uint16_t argc)
{
    sint16_t PwmValue = 0;

    data_t *data =(data_t *) argv;

    float tspeed,aspeed = 0.0;

    tspeed = 100.0 * NormalizeFloat(target_speed * 1.0,0.0,self->MaxSpeed * 1.0);
    aspeed = 100.0 * NormalizeFloat(actual_speed * 1.0,0.0,self->MaxSpeed * 1.0);

    if(fabs(data->M_PID.PID_Error[2]) > 20.0)
    {
        PwmValue = (sint16_t)(PWMx.MaxPwmDuty * fsign(data->M_PID.PID_Error[2]) * 0.8);
    }
    else
    {
        PID_Ctrl(&data->M_PID,tspeed,aspeed);
        PwmValue = (sint16_t)data->M_PID.Result;
    }

    if(data->ReportMotorData)
    {
        ANO.Send("%s%s16%s16",&target_speed,&actual_speed,&PwmValue);
    }

    return PwmValue;
}
/*
 * @Brief:  ����ջ����ƺ���(�ǶȻ�)
 * @output: Angle��[-90(��):90(��)]
 * @Attention:ALLģʽ
 * */
uint16_t ServoCtrlStrategy(struct servo_ctrl *self,float target_angle,float actual_angle,void *argv,uint16_t argc)
{
    data_t *data =(data_t *) argv;

    if(data->ReportServoData)
    {
        uint16_t PwmValue = - target_angle + self->PwmCentValue;

        ANO.Send("%f%f%f%s16%u16",&data->_Bias,&data->Bias,&data->S_PID.Result,&target_angle,&PwmValue);
    }

    return - target_angle + self->PwmCentValue;

}

