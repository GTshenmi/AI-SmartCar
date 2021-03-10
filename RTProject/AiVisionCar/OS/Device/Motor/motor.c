/*
 * motor.c
 *
 *  Created on: 2020年12月6日
 *      Author: 936305695
 */
#include <motor.h>
#include "driver.h"

uint16_t MotorSetSpeed(struct motor_ctrl *self,sint16_t speed)
{
    /*传参检查*/
    if(speed > self->MaxSpeed)
        speed = self->MaxSpeed;
    else if(speed < self->MinSpeed)
        speed = self->MinSpeed;

    /*读取反馈值*/
    sint16_t actual_speed = self->SpeedCache;

    self->Protect(self,speed,self->Argv,self->Argc);

    /*计算PWM Duty*/
    sint16_t PwmValue = 0;

    switch(self->State)
    {
        case Motor_Running:
            PwmValue = self->CtrlStrategy(self,speed,actual_speed,self->Argv,self->Argc);
            break;
        case Motor_Sleeping:
            break;

        case Motor_Normal_Stopping:
            PwmValue = self->CtrlStrategy(self,0,actual_speed,self->Argv,self->Argc);
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

sint16_t MotorGetSpeed(struct motor_ctrl *self)
{
    sint16_t speed = ENCx.Read(self->Encn);

    if(speed > self->MaxSpeed)
      speed = self->MaxSpeed;
    else if(speed < self->MinSpeed)
      speed = self->MinSpeed;

    return self->SpeedCache = speed;
}

sint16_t MotorGetSpeedFromCache(struct motor_ctrl *self)
{
    return self->SpeedCache;
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

motor_state_t GetMotorState(struct motor_ctrl *self)
{
    return self->State;
}


void MotorBindUsrData(struct motor_ctrl *self,void *argv,uint16_t argc)
{
    self->Argv = argv;
    self->Argc = argc;
}

void MotorSetSpeedLimit(struct motor_ctrl *self,sint16_t MaxSpeed,sint16_t MinSpeed)
{
    self->MaxSpeed = MaxSpeed;
    self->MinSpeed = MinSpeed;
}

sint16_t MotorGetMaxSpeed(struct motor_ctrl *self)
{
    return self->MaxSpeed;
}

sint16_t MotorGetMinSpeed(struct motor_ctrl *self)
{
    return self->MinSpeed;
}

uint16_t MotorGetPwmValue(struct motor_ctrl *self)
{
    return self->PwmValue;
}

void MotorDefaultProtect(struct motor_ctrl *self,sint16_t speed,void *argv,uint16_t argc)
{
    if(self->SpeedCache < 5 && speed != 0)
        self->StallingTime ++;
    else
        self->StallingTime --;

    if(self->StallingTime >= 0xffffffff)
    {
        //self->State = Motor_Stalling;
    }
}

sint16_t MotorDefaultCtrlStrategy(struct motor_ctrl *self,sint16_t target_speed,sint16_t actual_speed,void *argv,uint16_t argc)
{
    return abs(target_speed);
}

void MotorSetPwmValue(struct motor_ctrl *self,sint16_t value)
{
    self->Driver(self,value);
}

void MotorSetState(struct motor_ctrl *self,motor_state_t state)
{
    self->State = state;
}

void MotorDriver(struct motor_ctrl *self,sint16_t value)
{
    self->PwmValue = (uint16_t)abs(value);

#if defined(DriverChip)
    #if DriverChip == Drv8701
    if(value > 0)
    {
        PWMx.Write(self->Pwmn[0],self->PwmValue);
        PWMx.Write(self->Pwmn[1],PWMx.MinPwmDuty);
    }
    else
    {
        PWMx.Write(self->Pwmn[0],self->PwmValue);
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

uint8_t MotorInit(struct motor_ctrl *self)
{
    PWMx.Init(self->Pwmn[0]);
    PWMx.Init(self->Pwmn[1]);
    ENCx.Init(self->Encn);

    self->Argv = NULL;
    self->Argc = 0;
    self->State = Motor_Stopped;
    self->MaxSpeed = 10000;
    self->MinSpeed = -10000;
    self->SpeedCache = 0;
    self->PwmValue = 0;

    self->SetSpeed = MotorSetSpeed;
    self->GetSpeed = MotorGetSpeed;
    self->GetSpeedFromCache = MotorGetSpeedFromCache;
    self->CtrlStrategy = MotorDefaultCtrlStrategy;
    self->Protect = MotorDefaultProtect;

    self->Start = MotorStart;
    self->Stop = MotorStop;
    self->Sleep = MotorSleep;
    self->WakeUp = MotorWakeUp;
    self->Break = MotorBreak;

    self->GetState = GetMotorState;
    self->BindUsrData = MotorBindUsrData;
    self->SetSpeedLimit = MotorSetSpeedLimit;
    self->GetMaxSpeed = MotorGetMaxSpeed;
    self->GetMinSpeed = MotorGetMinSpeed;
    self->GetPwmValue = MotorGetPwmValue;

    self->SetPwmValue = MotorSetPwmValue;
    self->SetState = MotorSetState;
    self->Driver = MotorDriver;

    return 0;
}


