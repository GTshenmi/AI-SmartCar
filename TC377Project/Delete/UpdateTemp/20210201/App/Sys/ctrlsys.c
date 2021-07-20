/*
 * ctrlsys.c
 *
 *  Created on: 2020Äê12ÔÂ28ÈÕ
 *      Author: 936305695
 */
#include <ctrlsys.h>

void Unit_Init(struct unit *self);
void Unit_WakeUpCallBack(void *argv,uint16_t argc);


void CtrlSysInit(struct ctrlsys *self)
{
    Unit_Init(&self->DecisionUnit);
    Unit_Init(&self->ExecutionUnit);
    Unit_Init(&self->SensorUnit);
    self->UsrData = NULL;
}

void Unit_DefaultRun(struct unit *self,void *data)
{
/*
 * @Example In MotorUnit:
 */
    /*
      mctrlsysdata_t* mdata = (mctrlsysdata_t*)data;
      if(self->state == CtrlSys_Running)
      {
          do something...
      }
      .......
    */
/*
 * @Example In ServoUnit:
 */
    /*
      sctrlsysdata_t* sdata = (sctrlsysdata_t*)data;
      if(self->state == CtrlSys_Running)
      {
          do something...
      }
    */

}
void Unit_Start(struct unit *self)
{
    self->State = CtrlSys_Running;
}
void Unit_Sleep(struct unit *self,uint32_t time)
{
    if(self->State == CtrlSys_Running)
    {
        self->State = CtrlSys_Sleeping;
        if(time != 0)
        {
            uint16_t id = os.softtimer.findFreeTimer();
            os.softtimer.start(id,SoftTimer_Mode_OneShot,time,0,Unit_WakeUpCallBack,self,0);
        }
    }
}
void Unit_WakeUp(struct unit *self)
{
    if(self->State == CtrlSys_Sleeping)
        self->State = CtrlSys_Running;
}

void Unit_WakeUpCallBack(void *argv,uint16_t argc)
{
    struct unit *self = (struct unit *)argv;
    self->WakeUp(self);
}


void Unit_Stop(struct unit *self)
{
    self->State = CtrlSys_Stopped;
}

void Unit_Init(struct unit *self)
{
    self->Init = Unit_Init;
    self->Start = Unit_Start;
    self->Run = Unit_DefaultRun;
    self->Self = self;
    self->Sleep = Unit_Sleep;
    self->State = CtrlSys_Stopped;
    self->Stop = Unit_Stop;
    self->WakeUp = Unit_WakeUp;
}

ctrlsys_t ServoSys =
{
        .Init = CtrlSysInit,
        .Self = &ServoSys,
        .UsrData = NULL,
};

ctrlsys_t MotorSys =
{
        .Init = CtrlSysInit,
        .Self = &MotorSys,
        .UsrData = NULL,
};


