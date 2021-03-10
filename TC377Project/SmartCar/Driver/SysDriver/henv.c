/*
 * henv.c
 *
 *  Created on: 2021年1月31日
 *      Author: 936305695
 */
#include "henv.h"
App_Cpu0 g_AppCpu0; /**< \brief CPU 0 global data */
IfxCpu_mutexLock mutexCore0InitIsOk = 1;
IfxCpu_mutexLock mutexCore1InitIsOk = 1;
IfxCpu_mutexLock mutexCore2InitIsOk = 1;

void Core0_HardWareEnvInit()
{
    // 关闭CPU总中断
    IfxCpu_disableInterrupts();

    // 关闭看门狗，如果不设置看门狗喂狗需要关闭
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
    IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());

    // 读取总线频率
    g_AppCpu0.info.pllFreq = IfxScuCcu_getPllFrequency();
    g_AppCpu0.info.cpuFreq = IfxScuCcu_getCpuFrequency(IfxCpu_getCoreIndex());
    g_AppCpu0.info.sysFreq = IfxScuCcu_getSpbFrequency();
    g_AppCpu0.info.stmFreq = IfxStm_getFrequency(&MODULE_STM0);

}
void Core1_HardWareEnvInit()
{
    // 开启CPU总中断
    IfxCpu_disableInterrupts();

    // 关闭看门狗
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
}
void Core2_HardWareEnvInit()
{
    IfxCpu_disableInterrupts();

    // 关闭看门狗
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
}
void Core0_WaitForSync(void *data)
{
    IfxCpu_enableInterrupts();
    //通知CPU1，CPU0初始化完成
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
