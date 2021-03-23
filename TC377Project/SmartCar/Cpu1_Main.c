#include "include.h"


int core1_main (void)
{
    // ����CPU���ж�
    IfxCpu_enableInterrupts();

    // �رտ��Ź�
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());

    Core1_HardWareInit();
    Core1_SoftWareInit();

    // �ȴ�CPU0 ��ʼ�����
    while(!IfxCpu_acquireMutex(&mutexCpu0InitIsOk));

    Core1_Main();

    while(1)//��ѭ��
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
