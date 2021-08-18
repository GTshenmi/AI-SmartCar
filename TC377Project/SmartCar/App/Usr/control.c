/*
 * control.c
 *
 *  Created on: 2020年12月06日
 *      Author: 936305695
 *  @Brief:
 *      This file is for motor and servo close-loop control.
 */
#include <control.h>
#include "include.h"

void RightAngleSpeedCtrl(data_t *data);

void SpeedControl(void *argv)
{
    data_t *data = (data_t *)argv;

    switch(data->CarMode)
    {
        case AI_Mode:

            break;

        case SAutoBoot_Mode:

            if(Is_LoseLine(data))
            {
                data->Speed = 2000.0;
            }
            else if(Is_Boundary(data))
            {
                data->Speed = 2100.0;
            }
//            else if(fabs(data->Angle) >= 180.0)
//            {
//                data->Speed = 2000.0;
//            }
            else
            {
                data->Speed = FuzzySpeedControl(&data->M_FuzzySpeed,0.0,data->Angle * 100.0/210.0);
            }

            RightAngleSpeedCtrl(data);

            break;

        case DebugMode:

            Motor.SetPwmValue(Motor.Self,data->Speed);

            break;

        case LAutoBoot_Mode:default:

            break;

    }

    /*set speed*/
    if(data->CarMode != DebugMode)
    {
        float formatedSpeed = 0.0;

        formatedSpeed = (data->Speed * Motor.GetMaxSpeed(Motor.Self))/10000.0; // target * 0.055

        data->ActualSpeed = Motor.GetSpeed(Motor.Self);                        

        data->x += data->ActualSpeed * 0.000028 * 100.0;

        Motor.SetSpeed(Motor.Self,formatedSpeed);

        Motor.Update(Motor.Self);
    }
}

void AngleControl(void *argv)
{
    data_t *data = (data_t *)argv;
    
    /*calculate angle according to bias or use nn.*/
    switch(data->CarMode)
    {
        case AI_Mode:

            data->AIAngle = NeuralNetworkReasoning(data);

            data->AIAngle = ConstrainFloat(data->AIAngle,Servo.MinAngle,Servo.MaxAngle);

            if(!data->Is_AdjustAngle)
                data->CorAngle = FuzzyControl(&data->S_Fuzzy,0.0,data->Bias) * Servo.MaxAngle;

            data->CorAngle = ConstrainFloat(data->CorAngle,Servo.MinAngle,Servo.MaxAngle);

            data->Angle = data->AIAngle;

            //data->Angle = data->AIAngle + data->CorAngle;

            break;

        case SAutoBoot_Mode:

            data->AIAngle = NeuralNetworkReasoning(data);

            data->AIAngle = ConstrainFloat(data->AIAngle,Servo.MinAngle,Servo.MaxAngle);

            if(!data->Is_AdjustAngle)
                data->CorAngle = FuzzyControl(&data->S_Fuzzy,0.0,data->Bias) * Servo.MaxAngle;

            data->CorAngle = ConstrainFloat(data->CorAngle,Servo.MinAngle,Servo.MaxAngle);

            if(Is_LoseLine(data))
            {
                data->Angle = fsign(data->A[9] * 0.6 + data->A[8] * 0.4) * Servo.MaxAngle;//LowPass Filter.
            }
            else if(fabs(data->AIAngle) <= 20.0)
            {
                data->Angle = data->CorAngle;
            }
            else
            {
                data->Angle = data->AIAngle;
            }

            break;

        case DebugMode:

            Servo.SetPwmValue(Servo.Self,data->SPwmValue);

            break;

        case LAutoBoot_Mode:default:

            if(!data->Is_AdjustAngle)
                data->Angle = FuzzyControl(&data->S_Fuzzy,0.0,data->Bias) * Servo.MaxAngle;

            data->Angle = ConstrainFloat(data->Angle,Servo.MinAngle,Servo.MaxAngle);

            break;

    }

    /*record historical angle*/
    for(int i = 0 ; i < 9 ;i++)
    {
         data->A[i] = data->A[i + 1];
    }

    data->A[9] = data->Angle;

    /*set angle*/
    if(data->CarMode != DebugMode)
    {
        Servo.SetAngle(Servo.Self,data->Angle);

        Servo.Update(Servo.Self);
    }

}

void RightAngleSpeedCtrl(data_t *data)
{
    rightangle_speedctrl RASCState = RASC_Wait;

    switch(RASCState)
    {
        case RASC_Wait:

            if((data->A[8] - data->A[7]) >= 90.0 && data->Angle >= 140.0)
            {
                RASCState = RASC_Tracking;
            }

            break;
        case RASC_Tracking:
            data->Speed = 2000.0;

            if(data->h_bias <= 30.0 || data->Angle <= 20.0)
            {
                RASCState = RASC_Out;
            }
            break;

        case RASC_Out:
            RASCState = RASC_Wait;

            break;

        default:
            break;
    }
}

/*speed close loop control.*/
sint16_t MotorCtrlStrategy(struct motor_ctrl *self,float target_speed,float actual_speed,void *argv,uint16_t argc)
{
    sint16_t PwmValue = 0;

    data_t *data =(data_t *)argv;

    float tspeed,aspeed = 0.0;

    data->SpeedLoopCtrlType = PID;

    tspeed = 100.0 * NormalizeFloat(target_speed,0.0,self->MaxSpeed);

    aspeed = 100.0 * NormalizeFloat(actual_speed,0.0,self->MaxSpeed);

    //aspeed = 100.0 * random(0.0,1.0);

    if(0)
    {
        PwmValue = (sint16_t)(PWMx.MaxPwmDuty * fsign(data->M_PID.PID_Error[2]) * 0.6);
    }
    else
    {
        switch(data->SpeedLoopCtrlType)
        {
            case PID:

                PID_Ctrl(&data->M_PID,tspeed,aspeed);

                PwmValue = (sint16_t)ConstrainFloat(data->M_PID.Result,-10000.0,10000.0);

                break;

            case FuzzyPID:

                FuzzyPIDCtrl(&data->M_FuzzyKp,&data->M_FuzzyKi,tspeed,aspeed);

                data->M_PID.Kp = 3.2 + data->M_FuzzyKp.U;
                data->M_PID.Ki = 0.5 + data->M_FuzzyKi.U;

                PID_Ctrl(&data->M_PID,tspeed,aspeed);

                PwmValue = (sint16_t)ConstrainFloat(data->M_PID.Result,-10000.0,10000.0);

                break;

            case ADRC:

                ADRC_Control(&data->M_ADRC,target_speed,actual_speed);

                PwmValue = (sint16_t)ConstrainFloat(data->M_ADRC.u,0,10000.0);

                break;

            case OpeningLoop:

                PwmValue = (sint16_t)ConstrainFloat(target_speed,-10000.0,10000.0);

                break;

        }

        //Console.WriteLine("MPID:%.3f,%.3f",data->ActualSpeed,data->Speed * 550.0 /10000.0);


    }

    //PwmValue = 3000;

    return PwmValue;
}

uint16_t ServoCtrlStrategy(struct servo_ctrl *self,float target_angle,float actual_angle,void *argv,uint16_t argc)
{
    float angle = 0.0;

    angle = target_angle + self->PwmCentValue;

    return (uint16_t)angle;
}

