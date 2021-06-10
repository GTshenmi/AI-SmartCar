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

    if(data->CarMode == AI_Mode)            //目前匀速，可以不改
    {
        if(!data->Is_AdjustSpeed)
        {
            data->Speed = 3200;
        }

        float formatedSpeed = 0.0;

        formatedSpeed = (data->Speed * Motor.GetMaxSpeed(Motor.Self))/10000.0;

        data->Actual_Speed = Motor.GetSpeed(Motor.Self);

        Motor.SetPwmValue(Motor.Self,data->Speed);
    }
    else
    {

        if(!data->Is_AdjustSpeed)
            data->Speed = 3500;
            //data->Speed = FuzzySpeedControl(&data->FuzzySpeed,0.0,data->Bias);


        float formatedSpeed = 0.0;

        formatedSpeed = (data->Speed * Motor.GetMaxSpeed(Motor.Self))/10000.0;

        data->Actual_Speed = Motor.GetSpeed(Motor.Self);

        Motor.SetPwmValue(Motor.Self,data->Speed);

        //Motor.SetPwmValue(Motor.Self,7000);

        //Motor.SetSpeed(Motor.Self,formatedSpeed);

        //Motor.Update(Motor.Self);        
    }
}

void AngleControl(void *argv)
{
    /*Kp = 2.227*/

    /*Kp = 0.9*/

    data_t *data = (data_t *)argv;

    if(data->CarMode == AI_Mode)
    {
        /*神经网络计算角度(范围(Servo.MinAngle,Servo.MaxAngle) --- (-175,175)) Angle < 0:左  Angle > 0:右*/

        data->Angle = NeuralNetworkReasoning(data); 
        
        /*舵机设置角度*/
        Servo.SetAngle(Servo.Self,data->Angle);         

        Servo.Update(Servo.Self);
    }
    else if(data->CarMode == LAutoBoot_Mode)
    {
    //    if(fabs(data->h_bias) >= 20.0 || data->v_bias >= 30.0)
    //        data->S_PID.Kp = 0.8 + data->Bias * data->Bias * data->DynamicKp;
    //    else
    //        data->S_PID.Kp = 0.8;
    //
    //
    //    if(data->S_PID.Kp > 2.227)
    //        data->S_PID.Kp = 2.227;
    //
    //    if(data->Element.Type != None)
    //        data->S_PID.Kp = 2.227;

        //PID_Ctrl(&data->S_PID,0.0,data->Bias);

        data->Angle = FuzzyControl(&data->S_Fuzzy,0.0,data->Bias) * Servo.MaxAngle;

    //    static float Ka[5] = {0.3,0.3,0.2,0.1,0.1};
    //
    //    static float angle[5] = {0.0};

        //data->Angle = data->S_PID.Result;

        //data->Angle = (sint16_t)(FIR_Filter(Ka,angle,data->S_PID.Result,5));

        data->Angle = ConstrainFloat(data->Angle,Servo.MinAngle,Servo.MaxAngle);

        //Servo.SetPwmValue(Servo.Self,data->SPwmValue);

        //Servo.SetPwmValue(Servo.Self,750);

        Servo.SetAngle(Servo.Self,data->Angle);

        Servo.Update(Servo.Self);
    }
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

    FuzzyPID(&data->M_FuzzyKp,&data->M_FuzzyKi,target_speed,actual_speed);

    data->M_PID.Kp += data->M_FuzzyKp.U;
    data->M_PID.Ki += data->M_FuzzyKi.U;

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

