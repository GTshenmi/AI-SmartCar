/*
 * servo.c
 *
 *  Created on: 2020Äê12ÔÂ6ÈÕ
 *      Author: 936305695
 */
#include <servo.h>

uint16_t SetServoAngle(struct servo_ctrl *self,sint16_t angle)
{
    if(angle > self->MaxAngle)
        angle = self->MaxAngle;
    else if(angle < self->MinAngle)
        angle = self->MinAngle;

    float actual_angle = self->AngleCache;

    self->Protect(self,angle,self->UsrData);

    switch(self->State)
    {
        case Servo_Running:
            self->PwmValue = self->CtrlStrategy(self,angle,actual_angle,self->UsrData);
            break;
        case Servo_Sleeping:
            break;

        case Servo_Normal_Stopping:
            self->PwmValue = self->CtrlStrategy(self,0,actual_angle,self->UsrData);
            if(fabs(self->AngleCache)<1e-6)
                self->State = Servo_Stopped;
            break;
        case Servo_Force_Stopping:
            self->PwmValue = self->PwmCentValue;
            self->State = Servo_Stopped;
            break;
        case Servo_Stopped:
            self->PwmValue = self->PwmCentValue;
            break;

        case Servo_Stalling:
            self->Break(self);
            break;
    }

    PWMx.Write(self->PwmDevice,self->PwmValue);

    return self->PwmValue;
}

float GetServoAngle(struct servo_ctrl *self)
{
    return self->AngleCache = 0.0;
}

servo_state_t GetServoState(struct servo_ctrl *self)
{
    return self->State;
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
void ServoDefaultProtect(struct servo_ctrl *self,sint16_t speed,void *usr)
{
//    if(...)
//        self->State = Servo_Stalling;
}
uint16_t ServoDefaultCtrlStrategy(struct servo_ctrl *self,sint16_t target_angle,float actual_angle,void *usr)
{
    return target_angle + self->PwmCentValue;
}

uint8_t ServoInit(struct servo_ctrl *self)
{
    PWMx.Init(self->PwmDevice);
    self->SetAngle = SetServoAngle;
    self->GetAngle = GetServoAngle;
    self->CtrlStrategy = ServoDefaultCtrlStrategy;
    self->UsrData = NULL;

    self->Break = ServoBreak;
    self->Protect = ServoDefaultProtect;
    self->GetState = GetServoState;
    self->WakeUp = ServoWakeUp;
    self->Sleep = ServoSleep;
    self->Start = ServoStart;
    self->Stop = ServoStop;
    self->State = Servo_Stopped;
    return 0;
}

