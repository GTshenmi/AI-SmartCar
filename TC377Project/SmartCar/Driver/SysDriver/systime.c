/*
 * systime.c
 *
 *  Created on: 2020年12月26日
 *      Author: 936305695
 */
#include "systime.h"

void STM0_CH0_IRQHandler()
{
    /* 开启CPU中断  否则中断不可嵌套 */
    IfxCpu_enableInterrupts();

    //清除中断标志
    IfxStm_clearCompareFlag(&MODULE_STM0, g_StmCompareConfig[0].comparator);

    //开启新的中断配置，开始下次中断
    IfxStm_increaseCompare(&MODULE_STM0, g_StmCompareConfig[0].comparator, g_StmCompareConfig[0].ticks);

    if(Systime.InterruptCallBack != NULL)
    {
         Systime.InterruptCallBack();
    }
}

void SysTime_Init()
{
    char      interruptState;
    interruptState = disableInterrupts();

    Ifx_STM * STM = IfxStm_getAddress((IfxStm_Index)Systime.STM);
    unsigned char Index = (unsigned char)IfxStm_getIndex(STM) * 2 + (unsigned char)Systime.Channel;

    IfxStm_CompareConfig *stmCompareConfig = &g_StmCompareConfig[Index];

    //调用结构体实现配置
    IfxStm_initCompareConfig(stmCompareConfig);

     //挂起
    IfxStm_enableOcdsSuspend(STM);

    //获取STM模块的微秒计时 tick 数值
    sint32 ticks = IfxStm_getTicksFromMicroseconds((Ifx_STM *)STM, 1000);

    //修改ticks数值，使能触发输出
    stmCompareConfig->ticks = ticks;
    stmCompareConfig->comparator           = (IfxStm_Comparator)Systime.Channel;
    stmCompareConfig->comparatorInterrupt  = (IfxStm_ComparatorInterrupt)Systime.Channel; //中断选择
    stmCompareConfig->triggerPriority      = StmIrqPriority[Index];               //中断优先级设置
    stmCompareConfig->typeOfService        = StmIrqVectabNum[Systime.STM];                //服务内核

    //比较功能初始化
    IfxStm_initCompare(STM, stmCompareConfig);
    IfxCpu_Irq_installInterruptHandler((void*)StmIrqFuncPointer[Index], StmIrqPriority[Index]);//配置中断函数和中断号

    restoreInterrupts(interruptState);

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

