/*
 * test.c
 *
 *  Created on: 2021Äê3ÔÂ9ÈÕ
 *      Author: 936305695
 */
#include "include.h"

void MotorTest(struct motor_ctrl *self)
{

    sint16_t t = 0;
    sint16_t speed = 0;

    self->SetSpeedLimit(self,10000,-10000);
    Screen.WriteXLine(Screen.Self,1,"Starting Test Function.");

    Screen.WriteXLine(Screen.Self,1,"Wait.");
    self->SetSpeed(Motor.Self,2000);
    os.time.delayms(2000);

    Screen.WriteXLine(Screen.Self,1,"Start");
    self->Start(Motor.Self);
    self->SetSpeed(Motor.Self,2000);
    os.time.delayms(2000);


    Screen.WriteXLine(Screen.Self,1,"Stop ");
    self->Stop(Motor.Self);
    self->SetSpeed(Motor.Self,2000);
    os.time.delayms(2000);

    Screen.WriteXLine(Screen.Self,1,"Start");
    self->Start(Motor.Self);
    self->SetSpeed(Motor.Self,2000);
    os.time.delayms(2000);

    Screen.WriteXLine(Screen.Self,1,"Sleep");
    self->Sleep(Motor.Self);
    self->SetSpeed(Motor.Self,2000);
    os.time.delayms(2000);

    Screen.WriteXLine(Screen.Self,1,"Break");
    self->Break(Motor.Self);
    self->SetSpeed(Motor.Self,2000);
    os.time.delayms(2000);

    Screen.WriteXLine(Screen.Self,1,"Starting Test Speed.");
    Screen.Clear(Screen.Self,WHITE);

    self->Start(self);

    while(1)
    {
        t++;

        if(t >((self->MaxSpeed - self->MinSpeed) * 2))
            t = 0;

        if(t > (self->MaxSpeed - self->MinSpeed))
            speed = 2 * (self->MaxSpeed - self->MinSpeed) + self->MinSpeed - t;
        else
            speed = self->MinSpeed + t;

        Screen.WriteXLine(Screen.Self,1,"Speed = %d",speed);
        Screen.Fill(Screen.Self,0,Screen.Font.Hight,Screen.Width,Screen.Font.Hight * 2,WHITE);


        self->SetSpeed(self,speed);
        os.time.delayms(20);
    }
}

void ServoTest(struct servo_ctrl *self)
{
    self->SetAngleLimit(self,90,-90);
    Screen.WriteXLine(Screen.Self,1,"Starting Test Function.");
    Screen.Clear(Screen.Self,WHITE);

    sint16_t t = 0;
    sint16_t angle = 0;

    Screen.WriteXLine(Screen.Self,1,"Wait.");
    self->SetAngle(self,self->MaxAngle);
    os.time.delayms(2000);

    Screen.WriteXLine(Screen.Self,1,"Start");
    self->Start(self);
    self->SetAngle(self,self->MaxAngle);
    os.time.delayms(2000);


    Screen.WriteXLine(Screen.Self,1,"Stop ");
    self->Stop(self);
    self->SetAngle(self,self->MaxAngle);
    os.time.delayms(2000);

    Screen.WriteXLine(Screen.Self,1,"Start");
    self->Start(self);
    self->SetAngle(self,self->MaxAngle);
    os.time.delayms(2000);

    Screen.WriteXLine(Screen.Self,1,"Sleep");
    self->Sleep(self);
    self->SetAngle(self,self->MaxAngle);
    os.time.delayms(2000);

    Screen.WriteXLine(Screen.Self,1,"Break");
    self->Break(self);
    self->SetAngle(self,self->MaxAngle);
    os.time.delayms(2000);

    Screen.WriteXLine(Screen.Self,1,"Starting Test Angle.");
    Screen.Clear(Screen.Self,WHITE);

    self->Start(self);

    while(1)
    {
        t++;

        if(t >((self->MaxAngle - self->MinAngle) * 2))
            t = 0;

        if(t > (self->MaxAngle - self->MinAngle))
            angle = 2 * (self->MaxAngle - self->MinAngle) + self->MinAngle - t;
        else
            angle = self->MinAngle + t;

        Screen.WriteXLine(Screen.Self,1,"Angle = %d",angle);
        Screen.Fill(Screen.Self,0,Screen.Font.Hight,Screen.Width,Screen.Font.Hight * 2,WHITE);


        self->SetAngle(self,angle);
        os.time.delayms(20);
    }
}


