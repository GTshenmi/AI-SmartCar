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
    Ifx_STM * STM = IfxStm_getAddress((IfxStm_Index)Systime.STM);
    sint32 freq = (sint32)IfxStm_getFrequency(STM)/1000000;
    Ifx_TickTime stmNow;
    char      interruptState;

    interruptState = disableInterrupts();
    stmNow         = (Ifx_TickTime)IfxStm_get(STM) & TIME_INFINITE;
    restoreInterrupts(interruptState);

    return (unsigned long)(stmNow/freq);
}

uint32_t SysTime_Get_Timems()
{
    return SysTime_Get_Timeus()/1000;
}
uint8_t SysTime_Delayus(uint32_t us)
{
    Ifx_STM * STM = IfxStm_getAddress((IfxStm_Index)Systime.STM);
    uint32 tick = IfxStm_getTicksFromMicroseconds(STM, us);

    IfxStm_waitTicks(STM, tick);

    return 0;
}
uint8_t SysTime_Delayms(uint32_t ms)
{
    SysTime_Delayus(ms * 1000);

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

