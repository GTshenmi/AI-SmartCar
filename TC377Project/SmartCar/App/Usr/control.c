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

/*
 * @Brief:  ����ջ����ƺ���(�ٶȻ�/������)
 * @output: PwmValue��[0:10000]
 * */
sint16_t MotorCtrlStrategy(struct motor_ctrl *self,float target_speed,float actual_speed,void *argv,uint16_t argc)
{
    sint16_t PwmValue = 0;

    data_t *data =(data_t *) argv;

    float tspeed,aspeed = 0.0;

    tspeed = 100.0 * NormalizeFloat(target_speed,0.0,self->MaxSpeed);

    aspeed = 100.0 * NormalizeFloat(actual_speed,0.0,self->MaxSpeed);

    if(fabs(data->M_PID.PID_Error[2]) > 100.0)
    {
        PwmValue = (sint16_t)(PWMx.MaxPwmDuty * fsign(data->M_PID.PID_Error[2]) * 0.8);
    }
    else
    {
        PID_Ctrl(&data->M_PID,tspeed,aspeed);

        PwmValue = (sint16_t)ConstrainFloat(data->M_PID.Result,-5000.0,5000.0);

        if(data->ReportMotorData)
        {
            /*For Debug.*/
            data->ASpeed = aspeed;

            data->TSpeed = tspeed;

            data->MPwmValue = PwmValue;
        }
    }

    //PwmValue = target_speed;

    return PwmValue;
}
/*
 * @Brief:  ����ջ����ƺ���(�ǶȻ�)
 * @output: Angle��[-90(��):90(��)]
 * */
uint16_t ServoCtrlStrategy(struct servo_ctrl *self,float target_angle,float actual_angle,void *argv,uint16_t argc)
{
    //data_t *data =(data_t *) argv;

    return target_angle + self->PwmCentValue;
}

