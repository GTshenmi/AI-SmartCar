/*
 * gpsr.c
 *
 *  Created on: 2021��4��11��
 *      Author: 936305695
 */
#include "gpsr.h"

/*************************************************************************
 *  �������ƣ�GPSR_InitConfig(IfxSrc_Tos cpu, uint8 softIrq)
 *  ����˵��������еĳ�ʼ��
 *  ����˵����cpu     �� IfxSrc_Tos_cpu0   IfxSrc_Tos_cpu1
 *  ����˵����softIrq �� ÿ��CPU��4������ж�, irq���
 *  �������أ���
 *  �޸�ʱ�䣺2020��5��6��
 *  ��    ע��
 *************************************************************************/
void GPSR_InitConfig (IfxSrc_Tos cpu, SOFT_IRQ index)
{
    /* �ر��ж� */
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

    /* �����ж� */
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

    /* ��װ�жϷ����� */
    IfxCpu_Irq_installInterruptHandler((void*) CpuSoftIrqFuncPointer[temp], (uint32) CpuSoftIrqPriority[temp]);

    /* �����ж� */
    IfxCpu_restoreInterrupts(interruptState);
}

/*************************************************************************
 *  �������ƣ�CPSR_Trig(IfxSrc_Tos cpu, uint8 softIrq)
 *  ����˵������������ж�
 *  ����˵����cpu     �� IfxSrc_Tos_cpu0   IfxSrc_Tos_cpu1
 *  ����˵����softIrq �� ÿ��CPU��4������ж�, irq���
 *  �������أ���
 *  �޸�ʱ�䣺2020��5��6��
 *  ��    ע��
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

