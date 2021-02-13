/*
 * henv.c
 *
 *  Created on: 2021��1��31��
 *      Author: 936305695
 */
#include "henv.h"

//App_Cpu0 g_AppCpu0; /**< \brief CPU 0 global data */
//IfxCpu_mutexLock mutexCore0InitIsOk;
//IfxCpu_mutexLock mutexCore1InitIsOk;
//IfxCpu_mutexLock mutexCore2InitIsOk;

/*!
  * @brief    �������Դ
  *
  * @param    
  *
  * @return   
  *
  * @note     IMXRTϵ�е�Ƭ���ϵ�ʱ�ܽŲ���Ϊ�ߵ�ƽ���������ģ�����ϵ磬
  * @note     ��Ƭ�������������Ĺܽž��п��ܱ�����ģ�����ߵ�������ʧ��
  * @note     ���ʹ��һ���ܽ���Ϊ����ģ���Դ���أ�����Ƭ�������󣬽�C10��Ϊ�ߵ�ƽ���������Դ
  *
  * @see      
  *
  * @date     2019/9/12 ������
  */
void POWER_ENABLE(void)
{ 
    PIN_InitConfig(C10, PIN_MODE_OUTPUT, 1);
}

void Core0_HardWareEnvInit()
{
    BOARD_ConfigMPU();            /* ��ʼ���ڴ汣����Ԫ */
    
    BOARD_BootClockRUN();         /* ��ʼ��������ʱ��   */ 
    
    POWER_ENABLE();               /* ����ĸ����Χ��·��Դ ��ĸ������й� */

    BOARD_InitPins();             /* ���ڹܽų�ʼ��     */
    BOARD_InitDebugConsole();     /* ��ʼ������         */

	/*�����ж����ȼ���  0: 0����ռ���ȼ�16λ�������ȼ� 
     *1: 2����ռ���ȼ� 8�������ȼ� 2: 4����ռ���ȼ� 4�������ȼ� 
     *3: 8����ռ���ȼ� 2�������ȼ� 4: 16����ռ���ȼ� 0�������ȼ�
     */
    /* �������ȼ��� 2: 4����ռ���ȼ� 4�������ȼ� */
    NVIC_SetPriorityGrouping(NVIC_Group2);
    
    
//    //�ر�CPU���ж�
//    IfxCpu_disableInterrupts();
//
//    //�رտ��Ź�����������ÿ��Ź�ι����Ҫ�ر�
//    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
//    IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());
//
//    //��ȡ����Ƶ��
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
    //֪ͨCPU1��CPU0��ʼ�����
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
