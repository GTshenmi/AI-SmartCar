#include "include.h"

void core2_main(void)
{

    // ����CPU���ж�
    IfxCpu_enableInterrupts();

    // �رտ��Ź�
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());

    Core2_HardWareInit();
    Core2_SoftWareInit();

    // �ȴ�CPU0 ��ʼ�����
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
