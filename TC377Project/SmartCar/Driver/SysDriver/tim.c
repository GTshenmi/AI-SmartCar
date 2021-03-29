/*
 * timer.c
 *
 *  Created on: 2020Äê12ÔÂ12ÈÕ
 *      Author: 936305695
 */
#include <tim.h>

uint8_t TIMx_Init(timx_t *timer)
{
    if(timer->timer_source == STM)
    {
        STM_t Stm = (STM_t)timer->Timer;
        STM_Channel_t Channel = (STM_Channel_t ) timer->Channel;
        STM_InitConfig(Stm,Channel,timer->interrupt_interval);
        if(timer->Enable_Interrupt)
            STM_EnableInterrupt(Stm, Channel);
    }
    else if(timer->timer_source == CCU6)
    {
        CCU6_t CCU = (CCU6_t )timer->Timer;
        CCU6_Channel_t Channel = (CCU6_Channel_t) timer->Channel;
        CCU6_InitConfig(CCU,Channel,timer->interrupt_interval);
        if(timer->Enable_Interrupt)
            CCU6_EnableInterrupt(CCU, Channel);
    }
    else
        return 1;
    return 0;
}
uint8_t TIMx_Start(timx_t *timer)
{
    timer->Is_Run =true;
    if(timer->timer_source == STM)
    {

    }
    else if(timer->timer_source == CCU6)
    {

    }
    else
        return 1;
    return 0;
}
uint8_t TIMx_Stop(timx_t *timer)
{
    timer->Is_Run =false;
    if(timer->timer_source == STM)
    {

    }
    else if(timer->timer_source == CCU6)
    {

    }
    else
        return 1;
    return 0;
}
uint8_t TIMx_Delay(timx_t *timer,uint32_t us)
{
    if(timer->timer_source == STM)
    {
        STM_t Stm = (STM_t )timer->Timer;

        Ifx_STM * STM = IfxStm_getAddress((IfxStm_Index)Stm);
        uint32 tick = IfxStm_getTicksFromMicroseconds(STM, us);

        IfxStm_waitTicks(STM, tick);
    }
    else
        return 1;

    return 0;
}
uint32_t TIMx_GetTime(timx_t *timer)
{
    if(timer->timer_source == STM)
    {
        STM_t Stm = (STM_t )timer->Timer;

        Ifx_STM * STM = IfxStm_getAddress((IfxStm_Index)Stm);
        sint32 freq = (sint32)IfxStm_getFrequency(STM)/1000000;
        Ifx_TickTime stmNow;
        char      interruptState;

        interruptState = disableInterrupts();
        stmNow         = (Ifx_TickTime)IfxStm_get(STM) & TIME_INFINITE;
        restoreInterrupts(interruptState);

        return (uint32_t)(stmNow/freq);
    }

    return 0;
}
uint8_t TIMx_DeInit(timx_t *timer)
{
    if(timer->timer_source == STM)
    {
        STM_t Stm = (STM_t )timer->Timer;
        STM_Channel_t Channel = (STM_Channel_t ) timer->Channel;

        if(timer->Enable_Interrupt)
        {
            Ifx_STM * STM = IfxStm_getAddress((IfxStm_Index)Stm);
            IfxStm_clearCompareFlag(STM, (IfxStm_Comparator)Channel);
            IfxStm_disableComparatorInterrupt(STM, (IfxStm_Comparator)Channel);
        }
    }
    else if(timer->timer_source == CCU6)
    {
        CCU6_t CCU = (CCU6_t )timer->Timer;
        CCU6_Channel_t Channel = (CCU6_Channel_t ) timer->Channel;
        if(timer->Enable_Interrupt)
        {
            Ifx_CCU6 * module = IfxCcu6_getAddress((IfxCcu6_Index) CCU);
            IfxCcu6_clearInterruptStatusFlag(module, (IfxCcu6_InterruptSource) (7 + Channel * 2));
            IfxCcu6_disableInterrupt(module, (IfxCcu6_InterruptSource) (7 + Channel * 2));
        }
    }
    else
        return 1;
    return 0;
}

stim_m TIMx =
{
        .Init = TIMx_Init,
        .Start = TIMx_Start,
        .Stop = TIMx_Stop,
        .Delay = TIMx_Delay,
        .GetTime = TIMx_GetTime,
        .DeInit = TIMx_DeInit,

};



