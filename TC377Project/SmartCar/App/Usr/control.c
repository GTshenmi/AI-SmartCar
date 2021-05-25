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

void SpeedControl(void *argv)
{
    data_t *data = (data_t *)argv;

    float formatedSpeed = 0.0;

    formatedSpeed = (data->Speed * Motor.GetMaxSpeed(Motor.Self))/10000.0;

    data->Actual_Speed = Motor.GetSpeed(Motor.Self);

    Motor.SetPwmValue(Motor.Self,data->Speed);

    //Motor.SetSpeed(Motor.Self,formatedSpeed);

    //Motor.Update(Motor.Self);
}

void AngleControl(void *argv)
{
    /*Kp = 2.227*/

    /*Kp = 0.9*/

    data_t *data = (data_t *)argv;

//    if(fabs(data->h_bias) >= 20.0 || data->v_bias >= 30.0)
//        data->S_PID.Kp = 0.8 + data->Bias * data->Bias * data->DynamicKp; //待调
//    else
//        data->S_PID.Kp = 0.8;
//
//    /*动态PID限幅*/
//    if(data->S_PID.Kp > 2.227)        //待调
//        data->S_PID.Kp = 2.227;
//
//    if(data->Element.Type != None)
//        data->S_PID.Kp = 2.227;

    PID_Ctrl(&data->S_PID,0.0,data->Bias);

//    static float Ka[5] = {0.3,0.3,0.2,0.1,0.1};
//
//    static float angle[5] = {0.0};

    data->Angle = data->S_PID.Result;

    //data->Angle = (sint16_t)(FIR_Filter(Ka,angle,data->S_PID.Result,5));

    data->Angle = ConstrainFloat(data->Angle,Servo.MinAngle,Servo.MaxAngle);

    //Servo.SetPwmValue(Servo.Self,data->SPwmValue);

    //Servo.SetPwmValue(Servo.Self,750);

    Servo.SetAngle(Servo.Self,data->Angle);

    Servo.Update(Servo.Self);
}


/*
 * @Brief:  电机闭环控制函数(速度环/电流环)
 * @output: PwmValue：[0:10000]
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
    }

    //PwmValue = target_speed;

    return PwmValue;
}
/*
 * @Brief:  舵机闭环控制函数(角度环)
 * @output: Angle：[-90(左):90(右)]
 * */
uint16_t ServoCtrlStrategy(struct servo_ctrl *self,float target_angle,float actual_angle,void *argv,uint16_t argc)
{
    //data_t *data =(data_t *) argv;

    return target_angle + self->PwmCentValue;
}

