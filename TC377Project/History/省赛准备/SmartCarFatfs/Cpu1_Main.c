#include "include.h"


int core1_main (void)
{
    disableInterrupts();

    // �رտ��Ź�
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());

    //delayms(500);

    Core1_HardWareInit();
    Core1_SoftWareInit();

    // �ȴ�CPU0 ��ʼ�����
    IfxCpu_emitEvent(&g_cpuSyncEvent);
    IfxCpu_waitEvent(&g_cpuSyncEvent, 0xFFFF);

    IfxCpu_enableInterrupts();

#if defined(Debug)
    Console.WriteLine("Core1 Run To Main.");
#endif

    Core1_Main();

    while(1)//��ѭ��
    {

    }
}
