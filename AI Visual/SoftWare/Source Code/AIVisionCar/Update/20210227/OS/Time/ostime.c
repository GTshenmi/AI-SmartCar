/*
 * time.c
 *
 *  Created on: 2020��12��12��
 *      Author: 936305695
 */
#include "ostime.h"
#include "systime.h"

void OSTime_Init()
{
    Systime.Init();
}
nowtime_t  OSTime_Now()
{
    nowtime_t nowtime;
    nowtime.Ticks = Systime.Get_Timeus();
    nowtime.us = nowtime.Ticks % 1000;
    nowtime.ms = nowtime.Ticks / 1000 % 1000;
    nowtime.s = nowtime.Ticks / 1000000 % 1000;
    nowtime.min = nowtime.Ticks / 60000000 % 1000;
    nowtime.hour = nowtime.Ticks / 3600000000 % 1000;
    return nowtime;
}
uint32_t OSTime_Get_Timeus()
{
    return Systime.Get_Timeus();
}

uint32_t OSTime_Get_Timems()
{
    return (Systime.Get_Timeus() / 1000);
}

uint32_t OSTime_Get_Times()
{
    return (Systime.Get_Timeus() / 1000000);
}

uint8_t OSTime_Delayus(uint32_t us)
{
    return Systime.Delayus(us);
}

uint8_t OSTime_Delayms(uint32_t ms)
{
    return Systime.Delayus(ms * 1000);
}

uint8_t OSTime_Delays(uint32_t s)
{
    return Systime.Delayus(s * 1000000);
}

float OSTime_Get_Time(uint16_t unit)
{
    float time = 0.0;

    switch(unit)
    {
        case us:
            time = OSTime_Get_Timeus() * 1.0;;
            break;
        case ms:
            time = OSTime_Get_Timeus() * 1.0/1000.0;
            break;
        case s:
            time = OSTime_Get_Timeus() * 1.0/1000000.0;
            break;
        default:
            break;

    }

    return time;
}

float OSTime_Delay(float time,uint16_t unit)
{
    float error = 0.0;

    float t1 = 0.0;
    float t2 = 0.0;
    float dt = 0.0;

    t1 = OSTime_Get_Time(unit);

    switch(unit)
    {
        case us:

            OSTime_Delayus((uint32_t)time);

            break;

        case ms:

            OSTime_Delayus((uint32_t)(time * 1000));

            break;

        case s:

            OSTime_Delayus((uint32_t)(time * 1000000));

            break;

        default:

            break;

    }

    t2 = OSTime_Get_Time(us);

    dt = t2 - t1;

    error = dt - time;

    return error;
}





