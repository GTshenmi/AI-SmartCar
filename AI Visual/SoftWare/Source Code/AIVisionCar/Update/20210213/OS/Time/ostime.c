/*
 * time.c
 *
 *  Created on: 2020Äê12ÔÂ12ÈÕ
 *      Author: 936305695
 */
#include "ostime.h"

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
uint8_t OSTime_Delayus(uint32_t us)
{
    return Systime.Delayus(us);
}

uint8_t OSTime_Delayms(uint32_t ms)
{
    return Systime.Delayus(ms * 1000);
}





