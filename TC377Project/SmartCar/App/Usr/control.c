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

    static bool is_firstsetspeed = true;

    if(data->CarMode == AI_Mode)            //目前匀速，可以不改
    {
        if(!data->Is_AdjustSpeed && is_firstsetspeed)
        {
            data->Speed = 2000;
            is_firstsetspeed = false;
        }

        float formatedSpeed = 0.0;

        formatedSpeed = (data->Speed * Motor.GetMaxSpeed(Motor.Self))/10000.0; // x * 500 /10000 = 100

        data->Actual_Speed = Motor.GetSpeed(Motor.Self);

        //Motor.SetPwmValue(Motor.Self,data->Speed);

        //Motor.SetPwmValue(Motor.Self,3500);

        Motor.SetSpeed(Motor.Self,formatedSpeed);

        Motor.Update(Motor.Self);
    }
    else if(data->CarMode == SAutoBoot_Mode)
    {
        if(!data->Is_AdjustSpeed && is_firstsetspeed)
        {
            data->Speed = 2000;
            is_firstsetspeed = false;
        }


        //data->Speed = FuzzySpeedControl(&data->FuzzySpeed,0.0,data->Bias);

        float formatedSpeed = 0.0;

        formatedSpeed = (data->Speed * Motor.GetMaxSpeed(Motor.Self))/10000.0; // x * 500 /10000 = 100

        data->Actual_Speed = Motor.GetSpeed(Motor.Self);

        //Motor.SetPwmValue(Motor.Self,data->Speed);

        //Motor.SetPwmValue(Motor.Self,3500);

        Motor.SetSpeed(Motor.Self,formatedSpeed);

        Motor.Update(Motor.Self);
    }
    else if(data->CarMode == DebugMode)
    {
        data->Speed = 2000;

        float formatedSpeed = 0.0;

        formatedSpeed = (data->Speed * Motor.GetMaxSpeed(Motor.Self))/10000.0; // x * 500 /10000 = 100

        data->Actual_Speed = Motor.GetSpeed(Motor.Self);

        Motor.SetSpeed(Motor.Self,formatedSpeed);

        Motor.Update(Motor.Self);
    }
    else
    {

        if(!data->Is_AdjustSpeed)
        {

            data->Speed = 2000.0;

            if(data->Element.Type == RightAngle)
              data->Speed = 2000.0;
        }

        float formatedSpeed = 0.0;

        formatedSpeed = (data->Speed * Motor.GetMaxSpeed(Motor.Self))/10000.0; // x * 500 /10000 = 100

        data->Actual_Speed = Motor.GetSpeed(Motor.Self);

        data->x += data->Actual_Speed;

        Motor.SetSpeed(Motor.Self,formatedSpeed);

        Motor.Update(Motor.Self);
    }



}


typedef enum{
    NormalTrackingStraight,
    DeviateStraight,
    ReturnStraight,
}how_to_name_this_datatype;

void HowToNameThisFunc(data_t *data)
{

    /*------------------dt1----------------*//*------------------dt2----------------*//*------------------dt3----------------*/
    /*---------------Running---------------*//*-----------------Sleep---------------*//*----------------WakeUp---------------*/

    static how_to_name_this_datatype trackingstate;

    static float setTime = 0.0;

    static float deviateTime = 0.0;

    static float normalTime = 0.0;

    static float angle = 0.0;

    if(data->Element.Type == RightAngle)
    {
        trackingstate = NormalTrackingStraight;

    }


    switch(trackingstate)
    {
        case NormalTrackingStraight:

            normalTime = random(2.5,3.5) * 50.0;

            setTime = normalTime;

            trackingstate = DeviateStraight;

         break;

        case DeviateStraight:

            if(setTime <= 0.0)
            {
                data->IsAddNoise = true;
                //DebugBeepOn;

                float sign = (random(0.0,20.0) - 10) * 100.0;
                //uint8_t time = os.time.getTimeus() % 10000 / 100;
                deviateTime = random(0.15,0.25) * 50.0;//time/100.0f*0.7f;//random(0.1,0.7) * 50.0;
                setTime = deviateTime;

                deviateTime = fabs(deviateTime);

                angle = fsign(sign) * Servo.MaxAngle;
                //Servo.Sleep(Servo.Self);

                trackingstate = ReturnStraight;
            }

            break;

        case ReturnStraight:

            if(setTime <= 0.0)
            {

                data->IsAddNoise = false;

                if((data->h_bias <= 20.0 && data->v_sum <= 10.0))
                {
                    trackingstate = NormalTrackingStraight;
                    //DebugBeepOff;
                }

            }
            else
            {
                Servo.SetAngle(Servo.Self,angle);
                Servo.Update(Servo.Self);
            }
            break;
    }

    if(setTime >= -1.0)
        setTime -= 1.0;




}

void AngleControl(void *argv)
{
    /*Kp = 2.227*/

    /*Kp = 0.9*/

    data_t *data = (data_t *)argv;

    if(data->CarMode == AI_Mode)
    {
        /*神经网络计算角度(范围(Servo.MinAngle,Servo.MaxAngle) --- (-175,175)) Angle < 0:左  Angle > 0:右*/

        data->AIAngle = NeuralNetworkReasoning(data);

        data->AIAngle = ConstrainFloat(data->AIAngle,Servo.MinAngle,Servo.MaxAngle);

        if(!data->Is_AdjustAngle)
            data->CorAngle = FuzzyControl(&data->S_Fuzzy,0.0,data->Bias) * Servo.MaxAngle;

        data->CorAngle = ConstrainFloat(data->CorAngle,Servo.MinAngle,Servo.MaxAngle);

        data->Angle = data->AIAngle;

        /*十字采集纠正 , 不开启:AI模式数据不对*/

//        if(data->Element.Type == Cross)
//        {
//            /*舵机设置角度*/
//            Servo.SetAngle(Servo.Self,data->CorAngle);
//
//            Servo.Update(Servo.Self);
//        }
//        else
        {
            /*舵机设置角度*/
            Servo.SetAngle(Servo.Self,data->Angle);

            Servo.Update(Servo.Self);
        }


    }
    else if(data->CarMode == LAutoBoot_Mode)
    {
        if(!data->Is_AdjustAngle)
            data->Angle = FuzzyControl(&data->S_Fuzzy,0.0,data->Bias) * Servo.MaxAngle;

        data->Angle = ConstrainFloat(data->Angle,Servo.MinAngle,Servo.MaxAngle);

        Servo.SetAngle(Servo.Self,data->Angle);

        Servo.Update(Servo.Self);
    }
    else if(data->CarMode == DebugMode)
    {
        Servo.SetPwmValue(Servo.Self,data->SPwmValue);
    }
    else if(data->CarMode == SAutoBoot_Mode)
    {
        data->AIAngle = NeuralNetworkReasoning(data);

        data->AIAngle = ConstrainFloat(data->Angle,Servo.MinAngle,Servo.MaxAngle);

        data->CorAngle = FuzzyControl(&data->S_Fuzzy,0.0,data->Bias) * Servo.MaxAngle;
        data->CorAngle = ConstrainFloat(data->Angle,Servo.MinAngle,Servo.MaxAngle);

        if(data->AIAngle * data->CorAngle > 0)  //短前瞻和AI预测角度方向相同，取最大
        {
            data->Angle = fsign(data->AIAngle) * max(fabs(data->AIAngle),fabs(data->CorAngle));
        }
        else if(data->AIAngle * data->CorAngle < 0) //方向相反
        {
            data->Angle = data->AIAngle;
        }
        else
        {
            data->Angle = data->AIAngle;
        }

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

    data_t *data =(data_t *)argv;

    float tspeed,aspeed = 0.0;

    tspeed = 100.0 * NormalizeFloat(target_speed,0.0,self->MaxSpeed);

    aspeed = 100.0 * NormalizeFloat(actual_speed,0.0,self->MaxSpeed);

//    FuzzyPID(&data->M_FuzzyKp,&data->M_FuzzyKi,target_speed,actual_speed);
//
//    data->M_PID.Kp += data->M_FuzzyKp.U;
//    data->M_PID.Ki += data->M_FuzzyKi.U;

//    if(fabs(data->M_PID.PID_Error[2]) > 100.0)
//    {
//        PwmValue = (sint16_t)(PWMx.MaxPwmDuty * fsign(data->M_PID.PID_Error[2]) * 0.8);
//    }
//    else
    {
        PID_Ctrl(&data->M_PID,tspeed,aspeed);

        PwmValue = (sint16_t)ConstrainFloat(data->M_PID.Result,-10000.0,10000.0);
    }

    //PwmValue = target_speed;

    return PwmValue;
}
/*
 * @Brief:  舵机闭环控制函数(角度环)
 * @output: Angle：[-90(左):90(右)]
 *
 * */
uint16_t ServoCtrlStrategy(struct servo_ctrl *self,float target_angle,float actual_angle,void *argv,uint16_t argc)
{
//    data_t *data =(data_t *) argv;

    float angle = 0.0;

//    static float last_angle = 0.0;
//    static float current_angle = 0.0;
//    static float da = 0.0;
//
//    if(data->IsAddNoise)
//    {
//        last_angle = current_angle;
//        current_angle = target_angle;
//
//        da = current_angle - last_angle;
//
//        angle = last_angle + ConstrainFloat(da,-100.0,100.0);
//
//
//    }
//    else
//    {
//        angle = target_angle + self->PwmCentValue;
//    }

    angle = target_angle + self->PwmCentValue;

    return (uint16_t)angle;
}

