#include "include.h"

void core2_main(void)
{

    // ����CPU���ж�
    disableInterrupts();

    // �رտ��Ź�
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());

    Core2_HardWareInit();
    Core2_SoftWareInit();

    IfxCpu_emitEvent(&g_cpuSyncEvent);
    IfxCpu_waitEvent(&g_cpuSyncEvent, 0xFFFF);
    enableInterrupts();

#if defined(Debug)
    Console.WriteLine("Core2 Run To Main.");
#endif

    Core2_Main();

    while(1)
    {

    }
}
