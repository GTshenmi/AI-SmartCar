/*
 * servo.c
 *
 *  Created on: 2020Äê12ÔÂ6ÈÕ
 *      Author: 936305695
 */
#include <servo.h>
#include "driver.h"

uint16_t ServoSetAngle(struct servo_ctrl *self,sint16_t angle)
{
    if(angle > self->MaxAngle)
        angle = self->MaxAngle;
    else if(angle < self->MinAngle)
        angle = self->MinAngle;

    float actual_angle = self->AngleCache;

    self->Protect(self,angle,self->Argv,self->Argc);

    uint16_t PwmValue = 0;

    switch(self->State)
    {
        case Servo_Running:
            PwmValue = self->CtrlStrategy(self,angle,actual_angle,self->Argv,self->Argc);
            break;
        case Servo_Sleeping:
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

float ServoGetAngle(struct servo_ctrl *self)
{
    return self->AngleCache = self->PwmCentValue - self->PwmValue;
}

float ServoGetAngleFromCache(struct servo_ctrl *self)
{
    return self->AngleCache;
}

void ServoSleep(struct servo_ctrl *self)
{
    if(self->State == Servo_Running)
        self->State = Servo_Sleeping;
}

void ServoStop(struct servo_ctrl *self)
{
    if(self->State == Servo_Running || self->State == Servo_Sleeping)
        self->State = Servo_Normal_Stopping;
}

void ServoStart(struct servo_ctrl *self)
{
    if(self->State == Servo_Stopped)
        self->State = Servo_Running;
}

void ServoWakeUp(struct servo_ctrl *self)
{
    if(self->State == Servo_Sleeping)
        self->State = Servo_Running;
}

void ServoBreak(struct servo_ctrl *self)
{
    self->State = Servo_Force_Stopping;
}


servo_state_t GetServoState(struct servo_ctrl *self)
{
    return self->State;
}

void ServoBindUsrData(struct servo_ctrl *self,void *argv,uint16_t argc)
{
    self->Argv = argv;
    self->Argc = argc;
}

void ServoSetSpeedLimit(struct servo_ctrl *self,sint16_t MaxAngle,sint16_t MinAngle)
{
    self->MaxAngle = MaxAngle;
    self->MinAngle = MinAngle;
}

sint16_t ServoGetMaxSpeed(struct servo_ctrl *self)
{
    return self->MaxAngle;
}

sint16_t ServoGetMinSpeed(struct servo_ctrl *self)
{
    return self->MinAngle;
}

uint16_t ServoGetPwmValue(struct servo_ctrl *self)
{
    return self->PwmValue;
}

uint16_t ServoGetPwmCentValue(struct servo_ctrl *self)
{
    return self->PwmCentValue;
}

void ServoSetPwmCentValue(struct servo_ctrl *self,uint16_t value)
{
    self->PwmCentValue = value;
}

void ServoDefaultProtect(struct servo_ctrl *self,sint16_t angle,void *argv,uint16_t argc)
{
//    if(...)
//        self->State = Servo_Stalling;
}

uint16_t ServoDefaultCtrlStrategy(struct servo_ctrl *self,sint16_t target_angle,float actual_angle,void *argv,uint16_t argc)
{
    return target_angle + self->PwmCentValue;
}

void ServoSetPwmValue(struct servo_ctrl *self,uint16_t value)
{
    self->Driver(self,value);
}

void ServoSetState(struct servo_ctrl *self,servo_state_t state)
{
    self->State = state;
}
void ServoDriver(struct servo_ctrl *self,uint16_t value)
{
    self->PwmValue = value;
    PWMx.Write(self->Pwmn,self->PwmValue);
}

uint8_t ServoInit(struct servo_ctrl *self)
{
    PWMx.Init(self->Pwmn);

    self->Argv = NULL;
    self->Argc = 0;
    self->State = Servo_Stopped;
    self->AngleCache = 0.0;
    self->MaxAngle = 90;
    self->MinAngle = -90;
    self->MPUn = NULL;
    self->PwmCentValue = 0;
    self->PwmValue = 0;

    self->SetAngle = ServoSetAngle;
    self->GetAngle = ServoGetAngle;
    self->GetAngleFromCache = ServoGetAngleFromCache;
    self->CtrlStrategy = ServoDefaultCtrlStrategy;
    self->Protect = ServoDefaultProtect;

    self->Start = ServoStart;
    self->WakeUp = ServoWakeUp;
    self->Sleep = ServoSleep;
    self->Stop = ServoStop;
    self->Break = ServoBreak;

    self->BindUsrData = ServoBindUsrData;
    self->SetAngleLimit = ServoSetSpeedLimit;
    self->GetMaxAngle = ServoGetMaxSpeed;
    self->GetMinAngle = ServoGetMinSpeed;
    self->GetPwmValue = ServoGetPwmValue;
    self->GetPwmCentValue = ServoGetPwmCentValue;
    self->SetPwmCentValue = ServoSetPwmCentValue;

    self->SetPwmValue = ServoSetPwmValue;
    self->SetState = ServoSetState;
    self->Driver = ServoDriver;

    self->GetState = GetServoState;


    return 0;
}

