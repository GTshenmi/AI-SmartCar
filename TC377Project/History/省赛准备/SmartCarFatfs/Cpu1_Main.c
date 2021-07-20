#include "include.h"


int core1_main (void)
{
    disableInterrupts();

    // 关闭看门狗
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());

    //delayms(500);

    Core1_HardWareInit();
    Core1_SoftWareInit();

    // 等待CPU0 初始化完成
    IfxCpu_emitEvent(&g_cpuSyncEvent);
    IfxCpu_waitEvent(&g_cpuSyncEvent, 0xFFFF);

    IfxCpu_enableInterrupts();

#if defined(Debug)
    Console.WriteLine("Core1 Run To Main.");
#endif

    Core1_Main();

    while(1)//主循环
    {

    }
}
