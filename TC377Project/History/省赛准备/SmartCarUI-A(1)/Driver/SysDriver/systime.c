/*
 * systime.c
 *
 *  Created on: 2020Äê12ÔÂ26ÈÕ
 *      Author: 936305695
 */
#include "systime.h"

void STM0_CH0_IRQHandler()
{
    if(Systime.InterruptCallBack != NULL)
    {
         Systime.InterruptCallBack();
    }
}

void SysTime_Init()
{
    STM_InitConfig(Systime.STM,Systime.Channel,1000);
    Systime.InterruptCallBack = NULL;
}
uint32_t SysTime_Get_Timeus()
{
    return STM_GetNowUs(Systime.STM);
}

uint32_t SysTime_Get_Timems()
{
    return STM_GetNowUs(Systime.STM)/1000;
}
uint8_t SysTime_Delayus(uint32_t us)
{
    STM_DelayUs(Systime.STM,us);
    return 0;
}
uint8_t SysTime_Delayms(uint32_t ms)
{
    STM_DelayUs(Systime.STM,ms * 1000);
    return 0;
}

systimex_t Systime =
{
        .Init = SysTime_Init,
        .Get_Timeus = SysTime_Get_Timeus,
        .Get_Timems = SysTime_Get_Timems,
        .Delayms = SysTime_Delayms,
        .Delayus = SysTime_Delayus,

        .STM = STM0,
};

