/*
 * systime.c
 *
 *  Created on: 2020��12��26��
 *      Author: 936305695
 */
#include "systime.h"

void STM0_CH0_IRQHandler()
{
    /* ����CPU�ж�  �����жϲ���Ƕ�� */
    IfxCpu_enableInterrupts();

    //����жϱ�־
    IfxStm_clearCompareFlag(&MODULE_STM0, g_StmCompareConfig[0].comparator);

    //�����µ��ж����ã���ʼ�´��ж�
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

    //���ýṹ��ʵ������
    IfxStm_initCompareConfig(stmCompareConfig);

     //����
    IfxStm_enableOcdsSuspend(STM);

    //��ȡSTMģ���΢���ʱ tick ��ֵ
    sint32 ticks = IfxStm_getTicksFromMicroseconds((Ifx_STM *)STM, 1000);

    //�޸�ticks��ֵ��ʹ�ܴ������
    stmCompareConfig->ticks = ticks;
    stmCompareConfig->comparator           = (IfxStm_Comparator)Systime.Channel;
    stmCompareConfig->comparatorInterrupt  = (IfxStm_ComparatorInterrupt)Systime.Channel; //�ж�ѡ��
    stmCompareConfig->triggerPriority      = StmIrqPriority[Index];               //�ж����ȼ�����
    stmCompareConfig->typeOfService        = StmIrqVectabNum[Systime.STM];                //�����ں�

    //�ȽϹ��ܳ�ʼ��
    IfxStm_initCompare(STM, stmCompareConfig);
    IfxCpu_Irq_installInterruptHandler((void*)StmIrqFuncPointer[Index], StmIrqPriority[Index]);//�����жϺ������жϺ�

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

