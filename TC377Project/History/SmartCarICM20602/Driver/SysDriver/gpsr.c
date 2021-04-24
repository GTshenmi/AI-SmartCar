/*
 * gpsr.c
 *
 *  Created on: 2021年4月11日
 *      Author: 936305695
 */
#include "gpsr.h"

/*************************************************************************
 *  函数名称：GPSR_InitConfig(IfxSrc_Tos cpu, uint8 softIrq)
 *  功能说明：软件中的初始化
 *  参数说明：cpu     ： IfxSrc_Tos_cpu0   IfxSrc_Tos_cpu1
 *  参数说明：softIrq ： 每个CPU有4个软件中断, irq序号
 *  函数返回：无
 *  修改时间：2020年5月6日
 *  备    注：
 *************************************************************************/
void GPSR_InitConfig (IfxSrc_Tos cpu, SOFT_IRQ index)
{
    /* 关闭中断 */
    boolean interruptState = IfxCpu_disableInterrupts();

    uint8 temp = cpu * 4 + index;
    SRC_GPSR00.B.SRR = 0;
    SRC_GPSR01.B.SRR = 0;
    SRC_GPSR02.B.SRR = 0;
    SRC_GPSR03.B.SRR = 0;

    SRC_GPSR04.B.SRR = 0;
    SRC_GPSR05.B.SRR = 0;
    SRC_GPSR06.B.SRR = 0;
    SRC_GPSR07.B.SRR = 0;

    SRC_GPSR10.B.SRR = 0;
    SRC_GPSR11.B.SRR = 0;
    SRC_GPSR12.B.SRR = 0;
    SRC_GPSR13.B.SRR = 0;

    /* 开启中断 */
    volatile Ifx_SRC_SRCR *src = &MODULE_SRC.GPSR.GPSR[(int) cpu].SR[0];

    IfxSrc_init(src, (int) cpu, (uint32) CpuSoftIrqPriority[temp]);
    IfxSrc_enable(src);

    src = &MODULE_SRC.GPSR.GPSR[(int) cpu].SR[1];
    IfxSrc_init(src, (int) cpu, (uint32) CpuSoftIrqPriority[temp]);
    IfxSrc_enable(src);

    src = &MODULE_SRC.GPSR.GPSR[(int) cpu].SR[2];
    IfxSrc_init(src, (int) cpu, (uint32) CpuSoftIrqPriority[temp]);
    IfxSrc_enable(src);

    src = &MODULE_SRC.GPSR.GPSR[(int) cpu].SR[3];
    IfxSrc_init(src, (int) cpu, (uint32) CpuSoftIrqPriority[temp]);
    IfxSrc_enable(src);

    /* 安装中断服务函数 */
    IfxCpu_Irq_installInterruptHandler((void*) CpuSoftIrqFuncPointer[temp], (uint32) CpuSoftIrqPriority[temp]);

    /* 开启中断 */
    IfxCpu_restoreInterrupts(interruptState);
}

/*************************************************************************
 *  函数名称：CPSR_Trig(IfxSrc_Tos cpu, uint8 softIrq)
 *  功能说明：触发软件中断
 *  参数说明：cpu     ： IfxSrc_Tos_cpu0   IfxSrc_Tos_cpu1
 *  参数说明：softIrq ： 每个CPU有4个软件中断, irq序号
 *  函数返回：无
 *  修改时间：2020年5月6日
 *  备    注：
 *************************************************************************/
void CPSR_Trig (IfxSrc_Tos cpu, SOFT_IRQ index)
{
    if (cpu == IfxSrc_Tos_cpu0)
    {
        INT_SRB0.U = 1 << (uint8) index;
    }
    else
    {
        INT_SRB1.U = 1 << (uint8) index;
    }
}

