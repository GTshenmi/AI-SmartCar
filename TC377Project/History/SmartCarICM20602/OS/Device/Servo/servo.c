/*
 * servo.c
 *
 *  Created on: 2020Äê12ÔÂ6ÈÕ
 *      Author: 936305695
 */
#include <servo.h>
#include "driver.h"

uint16_t Servo_Update(struct servo_ctrl *self)
{
    float actual_angle = self->AngleCache;

    self->Protect(self,self->TargetAngle,self->Argv,self->Argc);

    uint16_t PwmValue = 0;

    switch(self->State)
    {
        case Servo_Running:
            PwmValue = self->CtrlStrategy(self,self->TargetAngle,actual_angle,self->Argv,self->Argc);
            break;
        case Servo_Sleeping:
            PwmValue = self->PwmValue;
            break;

        case Servo_Normal_Stopping:
            PwmValue = self->CtrlStrategy(self,0,actual_angle,self->Argv,self->Argc);
            if(fabs(self->AngleCache)<1e-6)
                self->State = Servo_Stopped;
            break;
        case Servo_Force_Stopping:
            PwmValue = self->PwmCentValue;
            self->State = Servo_Stopped;
            break;
        case Servo_Stopped:
            PwmValue = self->PwmCentValue;
            break;

        case Servo_Stalling:
            self->Break(self);
            break;
    }

    self->Driver(self,PwmValue);

    return self->PwmValue;
}

float Servo_SetAngle(struct servo_ctrl *self,float angle)
{
    if(angle > self->MaxAngle)
        angle = self->MaxAngle;
    else if(angle < self->MinAngle)
        angle = self->MinAngle;

    return self->TargetAngle = angle;
}

float Servo_GetAngle(struct servo_ctrl *self)
{
    return self->AngleCache = (float)(self->PwmCentValue - self->PwmValue);
}

float Servo_GetAngleFromCache(struct servo_ctrl *self)
{
    return self->AngleCache;
}

void Servo_Sleep(struct servo_ctrl *self)
{
    if(self->State == Servo_Running)
        self->State = Servo_Sleeping;
}

void Servo_Stop(struct servo_ctrl *self)
{
    if(self->State == Servo_Running || self->State == Servo_Sleeping)
        self->State = Servo_Normal_Stopping;
}

void Servo_Start(struct servo_ctrl *self)
{
    if(self->State == Servo_Stopped)
        self->State = Servo_Running;
}

void Servo_WakeUp(struct servo_ctrl *self)
{
    if(self->State == Servo_Sleeping)
        self->State = Servo_Running;
}

void Servo_Break(struct servo_ctrl *self)
{
    self->State = Servo_Force_Stopping;
}


servo_state_t Servo_GetState(struct servo_ctrl *self)
{
    return self->State;
}

void Servo_SetAngleLimit(struct servo_ctrl *self,float MaxAngle,float MinAngle)
{
    self->MaxAngle = MaxAngle;
    self->MinAngle = MinAngle;
}

float Servo_GetMaxAngle(struct servo_ctrl *self)
{
    return self->MaxAngle;
}

float Servo_GetMinAngle(struct servo_ctrl *self)
{
    return self->MinAngle;
}

uint16_t Servo_GetPwmValue(struct servo_ctrl *self)
{
    return self->PwmValue;
}

uint16_t Servo_GetPwmCentValue(struct servo_ctrl *self)
{
    return self->PwmCentValue;
}

void Servo_SetPwmCentValue(struct servo_ctrl *self,uint16_t value)
{
    self->PwmCentValue = value;
}

void Servo_DefaultProtect(struct servo_ctrl *self,float angle,void *argv,uint16_t argc)
{
//    if(...)
//        self->State = Servo_Stalling;
}

uint16_t Servo_DefaultCtrlStrategy(struct servo_ctrl *self,float target_angle,float actual_angle,void *argv,uint16_t argc)
{
    return (uint16_t)(-target_angle + self->PwmCentValue);
}

void Servo_SetPwmValue(struct servo_ctrl *self,uint16_t value)
{
    self->Driver(self,value);
}

void Servo_SetState(struct servo_ctrl *self,servo_state_t state)
{
    self->State = state;
}
void Servo_Driver(struct servo_ctrl *self,uint16_t value)
{
    self->PwmValue = value;
    PWMx.Write(self->Pwmn,self->PwmValue);
}

void Servo_Connect(struct servo_ctrl *self,servo_ctrlcallback ctrlstrategy,void *argv,uint16_t argc)
{
    self->CtrlStrategy = ctrlstrategy;
    self->Argv = argv;
    self->Argc = argc;
}

uint8_t Servo_Init(struct servo_ctrl *self)
{
    PWMx.Init(self->Pwmn);

    self->State = Servo_Stopped;
    self->AngleCache = 0.0;
    self->MaxAngle = 90;
    self->MinAngle = -90;
    self->MPUn = NULL;
    self->PwmCentValue = 0;
    self->PwmValue = 0;

    self->SetAngle = Servo_SetAngle;
    self->GetAngle = Servo_GetAngle;
    self->GetAngleFromCache = Servo_GetAngleFromCache;

    self->CtrlStrategy= Servo_DefaultCtrlStrategy;
    self->Argc = 0;
    self->Argv = NULL;
    self->Connect = Servo_Connect;

    self->Protect = Servo_DefaultProtect;

    self->Start = Servo_Start;
    self->WakeUp = Servo_WakeUp;
    self->Sleep = Servo_Sleep;
    self->Stop = Servo_Stop;
    self->Break = Servo_Break;

    self->SetAngleLimit = Servo_SetAngleLimit;
    self->GetMaxAngle = Servo_GetMaxAngle;
    self->GetMinAngle = Servo_GetMinAngle;
    self->GetPwmValue = Servo_GetPwmValue;
    self->GetPwmCentValue = Servo_GetPwmCentValue;
    self->SetPwmCentValue = Servo_SetPwmCentValue;

    self->SetPwmValue = Servo_SetPwmValue;
    self->SetState = Servo_SetState;
    self->Driver = Servo_Driver;

    self->Update = Servo_Update;
    self->TargetAngle = 0;

    self->GetState = Servo_GetState;

    self->Connect(self,Servo_DefaultCtrlStrategy,NULL,0);

    return 0;
}

