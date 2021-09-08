/*
 * timer.c
 *
 *  Created on: 2020Äê12ÔÂ12ÈÕ
 *      Author: 936305695
 */
#include <tim.h>

uint8_t TIMx_Init(timx_t *timer)
{
    if(timer->timer_source == PITx)
    {
        //PIT_Type *Pitx = (PIT_Type *)timer->Timer;
        pit_chnl_t Channel = (pit_chnl_t) timer->Channel;
        
        PIT_InitConfig(Channel,timer->interrupt_interval);

//        if(timer->Enable_Interrupt)
//            STM_EnableInterrupt(Stm, Channel);
    }
    else if(timer->timer_source == GPTx)
    {
        GPT_Type *GPT = (GPT_Type *)timer->Timer;
        GPT_TimeInitConfig(GPT);
//        if(timer->Enable_Interrupt)
//            CCU6_EnableInterrupt(CCU, Channel);
    }
    else
        return 1;
    return 0;
}
uint8_t TIMx_Start(timx_t *timer)
{
    timer->Is_Run =true;
    if(timer->timer_source == PITx)
    {

    }
    else if(timer->timer_source == GPTx)
    {
        GPT_Type *GPT = (GPT_Type *)timer->Timer;
        GPT_StopTimer(GPT);
        GPT_StartTimer(GPT); 
    }
    return 0;
}
uint8_t TIMx_Stop(timx_t *timer)
{
    timer->Is_Run =false;
   
    if(timer->timer_source == PITx)
    {

    }
    else if(timer->timer_source == GPTx)
    {
        GPT_Type *GPT = (GPT_Type *)timer->Timer;
        GPT_StopTimer(GPT);
    }
    
    return 0;
}
uint8_t TIMx_Delay(timx_t *timer,uint32_t us)
{
    if(timer->timer_source == PITx)
    {

    }
    else if(timer->timer_source == GPTx)
    {

    }

    return 0;
}
uint32_t TIMx_GetTime(timx_t *timer)
{
    if(timer->timer_source == PITx)
    {

    }
    else if(timer->timer_source == GPTx)
    {

    }

    return 0;
}
uint8_t TIMx_DeInit(timx_t *timer)
{
    if(timer->timer_source == PITx)
    {

    }
    else if(timer->timer_source == GPTx)
    {

    }
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



