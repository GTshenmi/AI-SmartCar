#include "include.h"

void core2_main(void)
{

    // 开启CPU总中断
    IfxCpu_enableInterrupts();

    // 关闭看门狗
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());

    Core2_HardWareInit();
    Core2_SoftWareInit();

    // 等待CPU0 初始化完成
    while(!IfxCpu_acquireMutex(&mutexCpu0InitIsOk));

    Core2_Main();

    while(1)
    {

    }
//    os.core[2].HardWareInit = Core2_HardWareInit;
//    os.core[2].Run = Core2_Main;
//    os.core[2].SoftWareInit = Core2_SoftWareInit;
//
//    os.init(2);
//
//    os.core[2].Run();
//
//    while(1);
}
