/*
 * timer.c
 *
 *  Created on: 2020年12月12日
 *      Author: 936305695
 */
#include <tim.h>

uint8_t TIMx_Init(timx_t *timer)
{
    if(timer == NULL)
        return 1;

    if(timer->timer_source == STM)
    {
        STM_t Stm = (STM_t)timer->Timer;
        STM_Channel_t Channel = (STM_Channel_t ) timer->Channel;

        char      interruptState;
        interruptState = disableInterrupts();

        Ifx_STM * STM = IfxStm_getAddress((IfxStm_Index)Stm);
        unsigned char Index = (unsigned char)IfxStm_getIndex(STM) * 2 + (unsigned char)Channel;

        IfxStm_CompareConfig *stmCompareConfig = &g_StmCompareConfig[Index];

        //调用结构体实现配置
        IfxStm_initCompareConfig(stmCompareConfig);

         //挂起
        IfxStm_enableOcdsSuspend(STM);

        //获取STM模块的微秒计时 tick 数值
        sint32 ticks = IfxStm_getTicksFromMicroseconds((Ifx_STM *)STM, timer->interrupt_interval);

        //修改ticks数值，使能触发输出
        stmCompareConfig->ticks = ticks;
        stmCompareConfig->comparator           = (IfxStm_Comparator)Channel;
        stmCompareConfig->comparatorInterrupt  = (IfxStm_ComparatorInterrupt)Channel; //中断选择
        stmCompareConfig->triggerPriority      = StmIrqPriority[Index];               //中断优先级设置
        stmCompareConfig->typeOfService        = StmIrqVectabNum[Stm];                //服务内核

        //比较功能初始化
        IfxStm_initCompare(STM, stmCompareConfig);
        IfxCpu_Irq_installInterruptHandler((void*)StmIrqFuncPointer[Index], StmIrqPriority[Index]);//配置中断函数和中断号

        restoreInterrupts(interruptState);

        if(timer->Enable_Interrupt)
        {
            IfxStm_clearCompareFlag(STM, (IfxStm_Comparator)Channel);
            IfxStm_enableComparatorInterrupt(STM, (IfxStm_Comparator)Channel);
        }
    }
    else if(timer->timer_source == CCU6)
    {
        CCU6_t CCU = (CCU6_t )timer->Timer;
        CCU6_Channel_t Channel = (CCU6_Channel_t) timer->Channel;

        IfxCcu6_Timer_Config timerConfig;
        IfxCcu6_Timer Ccu6Timer;

        Ifx_CCU6 * module = IfxCcu6_getAddress((IfxCcu6_Index)CCU);
        uint8 Index = CCU * 2 + Channel;
        uint32 period = 0;
        uint64 clk = 0;

        /* 关闭中断 */
        boolean interrupt_state = disableInterrupts();

        IfxCcu6_Timer_initModuleConfig(&timerConfig, module);
        clk = IfxScuCcu_getSpbFrequency();

        /* 设置时钟频率  */
        uint8 i = 0;
        while(i++ <16)
        {
            period = (uint32)(clk * timer->interrupt_interval / 1000000);
            if(period < 0xffff)
            {
                break;
            }
            else
            {
                clk = clk/2;
            }
        }

        switch(Channel)
        {
            case CCU6_Channel0:
                timerConfig.base.t12Frequency         = (float)clk;
                timerConfig.base.t12Period            = period; //设置定时中断
                timerConfig.timer = IfxCcu6_TimerId_t12;
                timerConfig.interrupt1.source         = IfxCcu6_InterruptSource_t12PeriodMatch;
                timerConfig.interrupt1.priority       = Ccu6IrqPriority[Index];
                timerConfig.interrupt1.typeOfService  = Ccu6IrqVectabNum[CCU];
                timerConfig.interrupt1.serviceRequest = IfxCcu6_ServiceRequest_1;
                timerConfig.clock.t12countingInputMode = IfxCcu6_CountingInputMode_internal;
                timerConfig.timer12.counterValue      = 0;
                break;

            case CCU6_Channel1:
                timerConfig.base.t13Frequency         = (float)clk;
                timerConfig.base.t13Period            = period;
                timerConfig.timer = IfxCcu6_TimerId_t13;
                timerConfig.interrupt2.source         = IfxCcu6_InterruptSource_t13PeriodMatch;
                timerConfig.interrupt2.priority       = Ccu6IrqPriority[Index];
                timerConfig.interrupt2.typeOfService  = Ccu6IrqVectabNum[CCU];
                timerConfig.interrupt2.serviceRequest = IfxCcu6_ServiceRequest_2;
                timerConfig.clock.t13countingInputMode = IfxCcu6_CountingInputMode_internal;
                timerConfig.timer13.counterValue      = 0;
                break;
        }

        timerConfig.trigger.t13InSyncWithT12 = FALSE;
        IfxCcu6_Timer_initModule(&Ccu6Timer, &timerConfig);
        IfxCpu_Irq_installInterruptHandler((void*)Ccu6IrqFuncPointer[Index], Ccu6IrqPriority[Index]);//配置中断函数和中断号
        restoreInterrupts(interrupt_state);
        IfxCcu6_Timer_start(&Ccu6Timer);

        if(timer->Enable_Interrupt)
        {
            IfxCcu6_clearInterruptStatusFlag(module, (IfxCcu6_InterruptSource) (7 + Channel * 2));
            IfxCcu6_enableInterrupt(module, (IfxCcu6_InterruptSource) (7 + Channel * 2));
        }
    }
    else
        return 1;
    return 0;
}
uint8_t TIMx_Start(timx_t *timer)
{
    if(timer == NULL)
        return 1;
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
    if(timer == NULL)
        return 1;

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
    if(timer == NULL)
        return 1;

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
    if(timer == NULL)
        return 1;

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
    if(timer == NULL)
        return 1;

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



