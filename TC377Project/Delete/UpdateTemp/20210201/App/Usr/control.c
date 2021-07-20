/*
 * control.c
 *
 *  Created on: 2020年12月6日
 *      Author: 936305695
 *  @Brief:
 *      This file is for motor and servo close-loop control.
 */
#include <control.h>

#include "include.h"

/*          AI Mode        */
/*
 * @Brief:  电机闭环执行(速度环/电流环) 在Motor.SetSpeed中
 * @Attention:AI模式
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
 * @Brief:  舵机闭环执行(角度环) 在Servo.SetAngle中
 * @Attention:AI模式
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
 * @Brief:  电机闭环控制函数(速度环/电流环)
 * @output: PwmValue：[0:10000]
 * @Attention:AI模式
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
 * @Brief:  舵机闭环控制函数(角度环)
 * @output: Angle：[-90(左):90(右)]
 * @Attention:AI模式
 * */
unsigned short ServoCtrlStrategy_ALLMode(struct servo_ctrl *self,signed short target_angle,float actual_angle,void *usr)
{

    return - target_angle + self->PwmCentValue;

}

