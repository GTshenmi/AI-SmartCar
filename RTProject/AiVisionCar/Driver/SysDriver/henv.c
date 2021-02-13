/*
 * henv.c
 *
 *  Created on: 2021年1月31日
 *      Author: 936305695
 */
#include "henv.h"

//App_Cpu0 g_AppCpu0; /**< \brief CPU 0 global data */
//IfxCpu_mutexLock mutexCore0InitIsOk;
//IfxCpu_mutexLock mutexCore1InitIsOk;
//IfxCpu_mutexLock mutexCore2InitIsOk;

/*!
  * @brief    打开外设电源
  *
  * @param    
  *
  * @return   
  *
  * @note     IMXRT系列单片机上电时管脚不能为高电平，如果外设模块先上电，
  * @note     单片机与外设相连的管脚就有可能被外设模块拉高导致启动失败
  * @note     因此使用一个管脚作为外设模块电源开关，当单片机启动后，将C10置为高电平，打开外设电源
  *
  * @see      
  *
  * @date     2019/9/12 星期四
  */
void POWER_ENABLE(void)
{ 
    PIN_InitConfig(C10, PIN_MODE_OUTPUT, 1);
}

void Core0_HardWareEnvInit()
{
    BOARD_ConfigMPU();            /* 初始化内存保护单元 */
    
    BOARD_BootClockRUN();         /* 初始化开发板时钟   */ 
    
    POWER_ENABLE();               /* 开启母板外围电路电源 与母版设计有关 */

    BOARD_InitPins();             /* 串口管脚初始化     */
    BOARD_InitDebugConsole();     /* 初始化串口         */

	/*设置中断优先级组  0: 0个抢占优先级16位个子优先级 
     *1: 2个抢占优先级 8个子优先级 2: 4个抢占优先级 4个子优先级 
     *3: 8个抢占优先级 2个子优先级 4: 16个抢占优先级 0个子优先级
     */
    /* 配置优先级组 2: 4个抢占优先级 4个子优先级 */
    NVIC_SetPriorityGrouping(NVIC_Group2);
    
    
//    //关闭CPU总中断
//    IfxCpu_disableInterrupts();
//
//    //关闭看门狗，如果不设置看门狗喂狗需要关闭
//    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
//    IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());
//
//    //读取总线频率
//    g_AppCpu0.info.pllFreq = IfxScuCcu_getPllFrequency();
//    g_AppCpu0.info.cpuFreq = IfxScuCcu_getCpuFrequency(IfxCpu_getCoreIndex());
//    g_AppCpu0.info.sysFreq = IfxScuCcu_getSpbFrequency();
//    g_AppCpu0.info.stmFreq = IfxStm_getFrequency(&MODULE_STM0);
}
void Core1_HardWareEnvInit()
{
    //IfxCpu_disableInterrupts();
   // IfxScuWdt_disableCpuWatchdog (IfxScuWdt_getCpuWatchdogPassword ());
}
void Core2_HardWareEnvInit()
{
    //IfxCpu_disableInterrupts();
   // IfxScuWdt_disableCpuWatchdog (IfxScuWdt_getCpuWatchdogPassword ());
}
void Core0_WaitForSync(void *data)
{
    /*do nothing*/
  //  IfxCpu_enableInterrupts();
    //通知CPU1，CPU0初始化完成
   // IfxCpu_releaseMutex(&mutexCore0InitIsOk);
  //  while(!IfxCpu_acquireMutex(&mutexCore1InitIsOk));
}
void Core1_WaitForSync(void *data)
{
   // IfxCpu_enableInterrupts();
   // IfxCpu_releaseMutex(&mutexCore1InitIsOk);
   // while(!IfxCpu_acquireMutex(&mutexCore0InitIsOk));
}
void Core2_WaitForSync(void *data)
{

}
