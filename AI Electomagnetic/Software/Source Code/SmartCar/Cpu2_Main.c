#include "include.h"

void core2_main(void)
{

    // 开启CPU总中断
    disableInterrupts();

    // 关闭看门狗
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());

    Core2_HardWareInit();
    Core2_SoftWareInit();

    IfxCpu_emitEvent(&g_cpuSyncEvent);
    IfxCpu_waitEvent(&g_cpuSyncEvent, 0xFFFFFFFF);
    enableInterrupts();

    Core2_Main();

    while(1)
    {

    }
}
