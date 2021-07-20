/*
 * motor.c
 *
 *  Created on: 2020Äê12ÔÂ6ÈÕ
 *      Author: 936305695
 */
#include <motor.h>

uint16_t SetMotorSpeed(struct motor_ctrl *self,sint16_t speed)
{
    if(speed > self->MaxSpeed)
        speed = self->MaxSpeed;
    else if(speed < self->MinSpeed)
        speed = self->MinSpeed;

    sint16_t actual_speed = self->SpeedCache;

    self->Protect(self,speed,self->UsrData);

    switch(self->State)
    {
        case Motor_Running:
            self->PwmValue = self->CtrlStrategy(self,speed,actual_speed,self->UsrData);
            break;
        case Motor_Sleeping:
            break;

        case Motor_Normal_Stopping:
            self->PwmValue = self->CtrlStrategy(self,0,actual_speed,self->UsrData);
            if(self->SpeedCache == 0)
                self->State = Motor_Stopped;
            break;
        case Motor_Force_Stopping:
            self->PwmValue = 0;
            self->State = Motor_Stopped;
            break;
        case Motor_Stopped:
            self->PwmValue = 0;
            break;

        case Motor_Stalling:
            self->Break(self);
            break;
    }

    if(speed > 0)
    {
        PWMx.Write(self->PwmDevice[0],self->PwmValue);
        PWMx.Write(self->PwmDevice[1],0);
    }
    else
    {
        PWMx.Write(self->PwmDevice[0],0);
        PWMx.Write(self->PwmDevice[1],self->PwmValue);
    }

    return self->PwmValue;
}

sint16_t GetMotorSpeed(struct motor_ctrl *self)
{
    uint16_t speed = ENCx.Read(self->EncDevice);

    if(speed > self->MaxSpeed)
      speed = self->MaxSpeed;
    else if(speed < self->MinSpeed)
      speed = self->MinSpeed;

    return self->SpeedCache = speed;
}

motor_state_t GetMotorState(struct motor_ctrl *self)
{
    return self->State;
}
void MotorSleep(struct motor_ctrl *self)
{
    if(self->State == Motor_Running)
        self->State = Motor_Sleeping;
}
void MotorStop(struct motor_ctrl *self)
{
    if(self->State == Motor_Running || self->State == Motor_Sleeping)
        self->State = Motor_Normal_Stopping;
}
void MotorStart(struct motor_ctrl *self)
{
    if(self->State == Motor_Stopped)
        self->State = Motor_Running;
}
void MotorWakeUp(struct motor_ctrl *self)
{
    if(self->State == Motor_Sleeping)
        self->State = Motor_Running;
}
void MotorBreak(struct motor_ctrl *self)
{
    self->State = Motor_Force_Stopping;
}
void MotorDefaultProtect(struct motor_ctrl *self,sint16_t speed,void *usr)
{
//    static int times = 0;
//
//    if(self->SpeedCache == 0 && speed != 0)
//        times ++;
//    if(times >= 500)
//    {
//        self->State = Motor_Stalling;
//    }
}

uint16_t MotorDefaultCtrlStrategy(struct motor_ctrl *self,sint16_t target_speed,sint16_t actual_speed,void *usr)
{
    return (uint16_t)((target_speed > 0) ? target_speed : -target_speed);
}

uint8_t MotorInit(struct motor_ctrl *self)
{
    PWMx.Init(self->PwmDevice[0]);
    PWMx.Init(self->PwmDevice[1]);
    ENCx.Init(self->EncDevice);
    self->SetSpeed = SetMotorSpeed;
    self->GetSpeed = GetMotorSpeed;
    self->CtrlStrategy = MotorDefaultCtrlStrategy;
    self->UsrData = NULL;

    self->Break = MotorBreak;
    self->Protect = MotorDefaultProtect;
    self->GetState = GetMotorState;
    self->WakeUp = MotorWakeUp;
    self->Sleep = MotorSleep;
    self->Start = MotorStart;
    self->Stop = MotorStop;
    self->State = Motor_Stopped;
    self->MaxSpeed = 10000;
    self->MinSpeed = -10000;

    return 0;
}


