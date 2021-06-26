/*
 * motor.c
 *
 *  Created on: 2020年12月6日
 *      Author: 936305695
 */
#include <motor.h>
#include "driver.h"

sint16_t Motor_Update(struct motor_ctrl *self)
{
    /*读取反馈值*/
    float actual_speed = self->SpeedCache;

    self->Protect(self,self->TargetSpeed,self->Argv,self->Argc);

    /*计算PWM Duty*/
    sint16_t PwmValue = 0;

    switch(self->State)
    {
        case Motor_Running:
            PwmValue = self->CtrlStrategy(self,self->TargetSpeed,actual_speed,self->Argv,self->Argc);
            break;
        case Motor_Sleeping:
            PwmValue = self->PwmValue;
            break;

        case Motor_Normal_Stopping:
            PwmValue = self->CtrlStrategy(self,0.0,actual_speed,self->Argv,self->Argc);
            if(self->SpeedCache == 0)
                self->State = Motor_Stopped;
            break;
        case Motor_Force_Stopping:
            PwmValue = 0;
            self->State = Motor_Stopped;
            break;
        case Motor_Stopped:
            PwmValue = 0;
            break;

        case Motor_Stalling:
            self->Break(self);
            break;
    }

    /*驱动电机*/
    self->Driver(self,PwmValue);

    return self->PwmValue;
}

float Motor_SetSpeed(struct motor_ctrl *self,float speed)
{
    /*传参检查*/
    if(speed > self->MaxSpeed)
        speed = self->MaxSpeed;
    else if(speed < self->MinSpeed)
        speed = self->MinSpeed;

    self->TargetSpeed = speed;

    return self->TargetSpeed;
}

float Motor_GetSpeed(struct motor_ctrl *self)
{
    float speed = ENCx.Read(self->Encn) * 1.0;

    if(speed > self->MaxSpeed)
      speed = self->MaxSpeed;
    else if(speed < self->MinSpeed)
      speed = self->MinSpeed;

    return self->SpeedCache = speed;
}

float Motor_GetSpeedFromCache(struct motor_ctrl *self)
{
    return self->SpeedCache;
}

void Motor_Sleep(struct motor_ctrl *self)
{
    if(self->State == Motor_Running)
        self->State = Motor_Sleeping;
}

void Motor_Stop(struct motor_ctrl *self)
{
    if(self->State == Motor_Running || self->State == Motor_Sleeping)
        self->State = Motor_Normal_Stopping;
}

void Motor_Start(struct motor_ctrl *self)
{
    if(self->State == Motor_Stopped)
        self->State = Motor_Running;
}

void Motor_WakeUp(struct motor_ctrl *self)
{
    if(self->State == Motor_Sleeping)
        self->State = Motor_Running;
}

void Motor_Break(struct motor_ctrl *self)
{
    self->State = Motor_Force_Stopping;
}

motor_state_t Motor_GetState(struct motor_ctrl *self)
{
    return self->State;
}


void Motor_Connect(struct motor_ctrl *self,motor_ctrlcallback ctrlstrategy,void *argv,uint16_t argc)
{
    self->CtrlStrategy = ctrlstrategy;

    self->Argv = argv;
    self->Argc = argc;
}

void Motor_SetSpeedLimit(struct motor_ctrl *self,float MaxSpeed,float MinSpeed)
{
    self->MaxSpeed = MaxSpeed;
    self->MinSpeed = MinSpeed;
}

float Motor_GetMaxSpeed(struct motor_ctrl *self)
{
    return self->MaxSpeed;
}

float Motor_GetMinSpeed(struct motor_ctrl *self)
{
    return self->MinSpeed;
}

uint16_t Motor_GetPwmValue(struct motor_ctrl *self)
{
    return self->PwmValue;
}

void Motor_DefaultProtect(struct motor_ctrl *self,float speed,void *argv,uint16_t argc)
{
//    if(self->SpeedCache == 0 && speed != 0)
//        self->StallingTime ++;
//    else if(self->StallingTime > 0)
//        self->StallingTime --;
//
//
//    if(self->StallingTime >= 3000)
//    {
//        self->State = Motor_Stalling;
//    }
}

sint16_t Motor_DefaultCtrlStrategy(struct motor_ctrl *self,float target_speed,float actual_speed,void *argv,uint16_t argc)
{
    sint16_t PwmValue = (sint16_t)target_speed;

    return abs(PwmValue);
}

void Motor_SetPwmValue(struct motor_ctrl *self,sint16_t value)
{
    self->Driver(self,value);
}

void Motor_SetState(struct motor_ctrl *self,motor_state_t state)
{
    self->State = state;
}

void Motor_Driver(struct motor_ctrl *self,sint16_t value)
{
    self->PwmValue = value;

#if defined(DriverChip)
    #if DriverChip == Drv8701
    if(value > 0)
    {
        PWMx.Write(self->Pwmn[0],value);
        PWMx.Write(self->Pwmn[1],PWMx.MinPwmDuty);
    }
    else
    {
        PWMx.Write(self->Pwmn[0],-value);
        PWMx.Write(self->Pwmn[1],PWMx.MaxPwmDuty);
    }
    #elif (DriverChip == IR7843) || (DriverChip ==BTN7971)
    if(value > 0)
    {
        PWMx.Write(self->Pwmn[0],self->PwmValue);
        PWMx.Write(self->Pwmn[1],PWMx.MinPwmDuty);
    }
    else
    {
        //PWMx.Write(self->Pwmn[0],self->Pwmn[0]->Freq + value);
        PWMx.Write(self->Pwmn[0],PWMx.MaxPwmDuty + value);
        PWMx.Write(self->Pwmn[1],PWMx.MaxPwmDuty);
    }
    #else

        #error Unknow DriverChip.Please Write Driver Function in motor.c.

    #endif
#else
    #error Undefined DriverChip.Please Check The Definition in motor.h.
#endif
}

uint8_t Motor_Init(struct motor_ctrl *self)
{
    PWMx.Init(self->Pwmn[0]);
    PWMx.Init(self->Pwmn[1]);
    ENCx.Init(self->Encn);

    self->State = Motor_Stopped;

    self->MaxSpeed = 10000.0;
    self->MinSpeed = -10000.0;
    self->SpeedCache = 0.0;
    self->TargetSpeed = 0.0;

    self->PwmValue = 0;

    self->SetSpeed = Motor_SetSpeed;
    self->GetSpeed = Motor_GetSpeed;
    self->GetSpeedFromCache = Motor_GetSpeedFromCache;
    self->Protect = Motor_DefaultProtect;

    self->Start = Motor_Start;
    self->Stop = Motor_Stop;
    self->Sleep = Motor_Sleep;
    self->WakeUp = Motor_WakeUp;
    self->Break = Motor_Break;

    self->Argv = NULL;
    self->Argc = 0;
    self->CtrlStrategy = Motor_DefaultCtrlStrategy;
    self->Connect = Motor_Connect;

    self->GetState = Motor_GetState;
    self->SetSpeedLimit = Motor_SetSpeedLimit;
    self->GetMaxSpeed = Motor_GetMaxSpeed;
    self->GetMinSpeed = Motor_GetMinSpeed;
    self->GetPwmValue = Motor_GetPwmValue;

    self->SetPwmValue = Motor_SetPwmValue;
    self->SetState = Motor_SetState;
    self->Driver = Motor_Driver;
    self->Update = Motor_Update;

    self->Connect(self,Motor_DefaultCtrlStrategy,NULL,0);

    return 0;
}


