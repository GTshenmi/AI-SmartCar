#include "include.h"


int core1_main (void)
{
    // 开启CPU总中断
    IfxCpu_enableInterrupts();

    // 关闭看门狗
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());

    Core1_HardWareInit();
    Core1_SoftWareInit();

    // 等待CPU0 初始化完成
    while(!IfxCpu_acquireMutex(&mutexCpu0InitIsOk));

    Core1_Main();

    while(1)//主循环
    {

    }

//    os.core[1].HardWareInit = Core1_HardWareInit;
//    os.core[1].Run = Core1_Main;
//    os.core[1].SoftWareInit = Core1_SoftWareInit;
//
//    os.init(1);
//
//    os.core[1].Run();
//
//    while(1)
//    {
//       ;
//    }
}
