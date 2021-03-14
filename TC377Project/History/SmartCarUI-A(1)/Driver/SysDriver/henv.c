/*
 * henv.c
 *
 *  Created on: 2021��1��31��
 *      Author: 936305695
 */
#include "henv.h"
App_Cpu0 g_AppCpu0; /**< \brief CPU 0 global data */
IfxCpu_mutexLock mutexCore0InitIsOk = 1;
IfxCpu_mutexLock mutexCore1InitIsOk = 1;
IfxCpu_mutexLock mutexCore2InitIsOk = 1;

void Core0_HardWareEnvInit()
{
    // �ر�CPU���ж�
    IfxCpu_disableInterrupts();

    // �رտ��Ź�����������ÿ��Ź�ι����Ҫ�ر�
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
    IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());

    // ��ȡ����Ƶ��
    g_AppCpu0.info.pllFreq = IfxScuCcu_getPllFrequency();
    g_AppCpu0.info.cpuFreq = IfxScuCcu_getCpuFrequency(IfxCpu_getCoreIndex());
    g_AppCpu0.info.sysFreq = IfxScuCcu_getSpbFrequency();
    g_AppCpu0.info.stmFreq = IfxStm_getFrequency(&MODULE_STM0);

}
void Core1_HardWareEnvInit()
{
    // ����CPU���ж�
    IfxCpu_disableInterrupts();

    // �رտ��Ź�
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
}
void Core2_HardWareEnvInit()
{
    IfxCpu_disableInterrupts();

    // �رտ��Ź�
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
}
void Core0_WaitForSync(void *data)
{
    IfxCpu_enableInterrupts();
    //֪ͨCPU1��CPU0��ʼ�����
    IfxCpu_releaseMutex(&mutexCore0InitIsOk);
    //while(!IfxCpu_acquireMutex(&mutexCore1InitIsOk));
    //while(!IfxCpu_acquireMutex(&mutexCore2InitIsOk));
}
void Core1_WaitForSync(void *data)
{
    IfxCpu_enableInterrupts();
    IfxCpu_releaseMutex(&mutexCore1InitIsOk);
    while(!IfxCpu_acquireMutex(&mutexCore0InitIsOk));
    //while(!IfxCpu_acquireMutex(&mutexCore2InitIsOk));
}
void Core2_WaitForSync(void *data)
{
    IfxCpu_enableInterrupts();
    IfxCpu_releaseMutex(&mutexCore2InitIsOk);
    while(!IfxCpu_acquireMutex(&mutexCore0InitIsOk));
    //while(!IfxCpu_acquireMutex(&mutexCore1InitIsOk));
}
