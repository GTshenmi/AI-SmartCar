/*
 * time.c
 *
 *  Created on: 2020Äê12ÔÂ12ÈÕ
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
    float time_us = 0.0;
    float time_ms = 0.0;
    float time_s = 0.0;

    switch(unit)
    {
        case us:
            time_us = OSTime_Get_Timeus() * 1.0;
            time = time_us;
            break;
        case ms:
            time_us = OSTime_Get_Timeus() * 1.0;
            time_ms = OSTime_Get_Timems() * 1.0;
            time = time_ms + (time_us/1000.0);
            break;
        case s:
            time_us = OSTime_Get_Timeus() * 1.0;
            time_ms = OSTime_Get_Timems() * 1.0;
            time_s = OSTime_Get_Times() * 1.0;
            time = time_s + (time_ms/1000.0) + (time_us/1000000.0);
            break;
        default:
            break;

    }
    return time;
}

float OSTime_Delay(float time,uint16_t unit)
{
    uint32_t time_us = 0;
    uint32_t time_ms = 0;
    uint32_t time_s =  0;

    float error = 0.0;

    float t1 = 0.0;
    float t2 = 0.0;
    float dt = 0.0;

    t1 = OSTime_Get_Time(unit);

    switch(unit)
    {
        case us:

            time_us = (uint32_t)time;

            OSTime_Delayus(time_us);

            //error = time_us * 1.0 - time;

            break;

        case ms:

            time_ms = (uint32_t)(time);
            time_us = (uint32_t)((time - time_ms * 1.0) * 1000.0);

            OSTime_Delayms(time_ms);
            OSTime_Delayus(time_us);

            //error = (time_ms * 1.0) + (time_us * 1.0/1000.0) - time;

            break;

        case s:

            time_s = (uint32_t)(time);
            time_ms = (uint32_t)((time - time_s * 1.0) * 1000.0);
            time_us = (uint32_t)((time - time_s * 1.0 - (time_ms * 1.0/1000.0)) * 1000000.0);

            OSTime_Delays(time_s);
            OSTime_Delayms(time_ms);
            OSTime_Delayus(time_us);

            //error = (time_s * 1.0) + (time_ms * 1.0/1000.0) + (time_us/1000000.0) - time;

            break;

        default:

            break;

    }

    t2 = OSTime_Get_Time(us);

    dt = t2 - t1;

    error = dt - time;

    return error;
}





