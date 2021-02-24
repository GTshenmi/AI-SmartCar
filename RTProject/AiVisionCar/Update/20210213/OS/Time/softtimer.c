/*
 * ostimer.c
 *
 *  Created on: 2020Äê12ÔÂ13ÈÕ
 *      Author: 936305695
 */
#include <softtimer.h>

static uint32_t softTimerBase;
const static uint32_t softTimerNum = 11;
static softtimer_t softTimer[11];

void Task_SoftTimerUpdate(void *argv,uint16_t argc)
{
    for(uint16_t i = 0;i< softTimerNum;i++)
    {
        if(softTimer[i].State != SoftTimer_Stopped)
        {
            SoftTimerUpdate((uint16_t)i);
        }
    }
}

void OSTimeCallBack()
{
    softTimerBase++;
}

void SoftTimerInit()
{
    for(uint32_t i = 0;i< softTimerNum;i++)
    {
         softTimer[i].CallBack = NULL;
         softTimer[i].Mode = SoftTimer_Mode_OneShot;
         softTimer[i].State = SoftTimer_Stopped;
         softTimer[i].argc = 0;
         softTimer[i].argv = NULL;
         softTimer[i].Match = 0;
         softTimer[i].Period = 0;
         softTimer[i].Duration = 0;
    }
    Systime.InterruptCallBack = OSTimeCallBack;
}
void SoftTimerStart(uint16_t id,stmrmode_t mode,uint32_t delay,uint32_t duration,void (*callback)(void *argv,uint16_t argc),void *argv,uint16_t argc)
{
     softTimer[id].CallBack = callback;
     softTimer[id].Mode = mode;
     softTimer[id].State = SoftTimer_Running;
     softTimer[id].argc = argc;
     softTimer[id].argv = argv;
     softTimer[id].Match = softTimerBase + delay;
     softTimer[id].Period = delay;
     softTimer[id].Duration = duration;
}
void SoftTimerStop(uint16_t id)
{

     softTimer[id].State = SoftTimer_Stopped;
}
uint8_t SoftTimerGetStates(uint16_t id)
{

    return  softTimer[id].State;
}
uint16_t SoftTimerFindFreeTimer()
{
    for(uint16_t id = 0;id <softTimerNum ; id++)
    {
        if(softTimer[id].State == SoftTimer_Stopped)
            return id;
    }
    return 0;
}

uint8_t SoftTimerUpdate(uint16_t id)
{
    switch( softTimer[id].State)
    {
        case SoftTimer_Stopped:
            break;
        case SoftTimer_Running:
            if( softTimer[id].Match <= softTimerBase)
            {
                 softTimer[id].State = SoftTimer_TimeOut;
                 softTimer[id].CallBack( softTimer[id].argv, softTimer[id].argc);
            }
            break;
        case SoftTimer_TimeOut:
            if( softTimer[id].Mode == SoftTimer_Mode_OneShot)
            {
                 softTimer[id].State = SoftTimer_Stopped;
            }
            else if( softTimer[id].Mode == SoftTimer_Mode_XTimes)
            {
                if( softTimer[id].Duration <  softTimer[id].Period)
                     softTimer[id].State = SoftTimer_Stopped;
                else
                     softTimer[id].Duration -=  softTimer[id].Period;

            }
            else
            {
                 softTimer[id].Match = softTimerBase +  softTimer[id].Period;
                 softTimer[id].State = SoftTimer_Running;
            }
            break;
        default:
            return 1;
    }
    return 0;
}

