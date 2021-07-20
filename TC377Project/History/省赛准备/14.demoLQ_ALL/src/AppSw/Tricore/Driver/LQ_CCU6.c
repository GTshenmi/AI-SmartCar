/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
��ƽ    ̨�������������ܿƼ�TC3xxxA���İ�
����    д��chiusir
��E-mail��chiusir@163.com
������汾��V1.1 ��Ȩ���У���λʹ��������ϵ��Ȩ
�������¡�2020��10��28��
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://longqiu.taobao.com
------------------------------------------------
��dev.env.��AURIX Development Studio1.2.2�����ϰ汾
��Target �� TC3xxxA
��Crystal�� 20.000Mhz
��SYS PLL�� 300MHz
________________________________________________________________
����iLLD_1_0_1_11_0�ײ����,
ʹ�����̵�ʱ�򣬽������û�пո��Ӣ��·����
=================================================================
����������Ƶ��ַ��https://space.bilibili.com/95313236
 ________________________________________________________________
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ
 *  TC397 ������CCU6ģ��  ÿ��ģ��������������ʱ��  ������ʱ���ж�
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include "LQ_CCU6_Timer.h"
#include "LQ_CCU6.h"
#include <IfxCpu_Irq.h>
#include <LQ_GPIO_LED.h>
#include <Main.h>
#include <LQ_GPT12_ENC.h>

volatile sint16 ECPULSE1 = 0;          // �ٶ�ȫ�ֱ���
volatile sint16 ECPULSE2 = 0;          // �ٶ�ȫ�ֱ���
volatile sint32 RAllPulse = 0;          // �ٶ�ȫ�ֱ���

IFX_INTERRUPT(CCU60_CH0_IRQHandler, CCU60_VECTABNUM, CCU60_CH0_PRIORITY);
IFX_INTERRUPT(CCU60_CH1_IRQHandler, CCU60_VECTABNUM, CCU60_CH1_PRIORITY);
IFX_INTERRUPT(CCU61_CH0_IRQHandler, CCU61_VECTABNUM, CCU61_CH0_PRIORITY);
IFX_INTERRUPT(CCU61_CH1_IRQHandler, CCU61_VECTABNUM, CCU61_CH1_PRIORITY);

/** CCU6�ж�CPU��� */
const uint8 Ccu6IrqVectabNum[2] = {CCU60_VECTABNUM, CCU61_VECTABNUM};

/** CCU6�ж����ȼ� */
const uint8 Ccu6IrqPriority[4] = {CCU60_CH0_PRIORITY, CCU60_CH1_PRIORITY, CCU61_CH0_PRIORITY, CCU61_CH1_PRIORITY};

/** CCU6�жϷ�������ַ */
const void *Ccu6IrqFuncPointer[4] = {&CCU60_CH0_IRQHandler, &CCU60_CH1_IRQHandler, &CCU61_CH0_IRQHandler,&CCU61_CH1_IRQHandler};

/***********************************************************************************************/
/********************************CCU6�ⲿ�ж�  ������******************************************/
/***********************************************************************************************/

/*************************************************************************
 *  �������ƣ�void CCU60_CH0_IRQHandler(void)
 *  ����˵����
 *  ����˵������
 *  �������أ���
 *  �޸�ʱ�䣺2020��3��30��
 *  ��    ע��CCU60_CH0ʹ�õ��жϷ�����
 *************************************************************************/
void CCU60_CH0_IRQHandler (void)
{
    /* ����CPU�ж�  �����жϲ���Ƕ�� */
    IfxCpu_enableInterrupts();

    //����жϱ�־
    IfxCcu6_clearInterruptStatusFlag(&MODULE_CCU60, IfxCcu6_InterruptSource_t12PeriodMatch);

    /* �û����� */
    LED_Ctrl(LED3,RVS);

}

/*************************************************************************
 *  �������ƣ�void CCU60_CH1_IRQHandler(void)
 *  ����˵����
 *  ����˵������
 *  �������أ���
 *  �޸�ʱ�䣺2020��3��30��
 *  ��    ע��CCU60_CH1ʹ�õ��жϷ�����
 *************************************************************************/
void CCU60_CH1_IRQHandler (void)
{
    /* ����CPU�ж�  �����жϲ���Ƕ�� */
    IfxCpu_enableInterrupts();

    //����жϱ�־
    IfxCcu6_clearInterruptStatusFlag(&MODULE_CCU60, IfxCcu6_InterruptSource_t13PeriodMatch);

    /* �û����� */
    LED_Ctrl(LED2,RVS);

}

/*************************************************************************
 *  �������ƣ�void CCU61_CH0_IRQHandler(void)
 *  ����˵����
 *  ����˵������
 *  �������أ���
 *  �޸�ʱ�䣺2020��3��30��
 *  ��    ע��CCU61_CH0ʹ�õ��жϷ�����
 *************************************************************************/
void CCU61_CH0_IRQHandler(void)
{
    /* ����CPU�ж�  �����жϲ���Ƕ�� */
    IfxCpu_enableInterrupts();

    //����жϱ�־
    IfxCcu6_clearInterruptStatusFlag(&MODULE_CCU61, IfxCcu6_InterruptSource_t12PeriodMatch);

    /* �û����� */
    /* ��ȡ������ֵ */
    ECPULSE1 = ENC_GetCounter(ENC2_InPut_P33_7); // ���� ĸ���ϱ�����1��С��ǰ��Ϊ��ֵ
    ECPULSE2 = ENC_GetCounter(ENC4_InPut_P02_8); // �ҵ�� ĸ���ϱ�����2��С��ǰ��Ϊ��ֵ
    RAllPulse += ECPULSE2;
}

/*************************************************************************
 *  �������ƣ�void CCU61_CH1_IRQHandler(void)
 *  ����˵����
 *  ����˵������
 *  �������أ���
 *  �޸�ʱ�䣺2020��3��30��
 *  ��    ע��CCU61_CH1ʹ�õ��жϷ�����
 *************************************************************************/
void CCU61_CH1_IRQHandler (void)
{
    /* ����CPU�ж�  �����жϲ���Ƕ�� */
    IfxCpu_enableInterrupts();

    //����жϱ�־
    IfxCcu6_clearInterruptStatusFlag(&MODULE_CCU61, IfxCcu6_InterruptSource_t13PeriodMatch);

    /* �û����� */
    LED_Ctrl(LED0,RVS);
}

/*************************************************************************
 *  �������ƣ�CCU6_InitConfig CCU6
 *  ����˵������ʱ�������жϳ�ʼ��
 *  ����˵����ccu6    �� ccu6ģ��            CCU60 �� CCU61
 *  ����˵����channel �� ccu6ģ��ͨ��  CCU6_Channel0 �� CCU6_Channel1
 *  ����˵����us      �� ccu6ģ��  �ж�����ʱ��  ��λus
 *  �������أ���
 *  �޸�ʱ�䣺2020��3��30��
 *  ��    ע��    CCU6_InitConfig(CCU60, CCU6_Channel0, 100);  //100us����һ���ж�
 *************************************************************************/
void CCU6_InitConfig(CCU6_t ccu6, CCU6_Channel_t channel, uint32 us)
{
    IfxCcu6_Timer_Config timerConfig;
    IfxCcu6_Timer Ccu6Timer;

    Ifx_CCU6 * module = IfxCcu6_getAddress((IfxCcu6_Index)ccu6);
    uint8 Index = ccu6 * 2 + channel;
    uint32 period = 0;
    uint64 clk = 0;

    /* �ر��ж� */
    boolean interrupt_state = disableInterrupts();

    IfxCcu6_Timer_initModuleConfig(&timerConfig, module);
    clk = IfxScuCcu_getSpbFrequency();

    /* ����ʱ��Ƶ��  */
    uint8 i = 0;
    while(i++ <16)
    {
        period = (uint32)(clk * us / 1000000);
        if(period < 0xffff)
        {
            break;
        }
        else
        {
            clk = clk/2;
        }
    }

    switch(channel)
    {
        case CCU6_Channel0:
            timerConfig.base.t12Frequency         = (float)clk;
            timerConfig.base.t12Period            = period; //���ö�ʱ�ж�
            timerConfig.timer = IfxCcu6_TimerId_t12;
            timerConfig.interrupt1.source         = IfxCcu6_InterruptSource_t12PeriodMatch;
            timerConfig.interrupt1.priority       = Ccu6IrqPriority[Index];
            timerConfig.interrupt1.typeOfService  = Ccu6IrqVectabNum[ccu6];
            timerConfig.interrupt1.serviceRequest = IfxCcu6_ServiceRequest_1;
            timerConfig.clock.t12countingInputMode = IfxCcu6_CountingInputMode_internal;
            timerConfig.timer12.counterValue      = 0;
            break;

        case CCU6_Channel1:
            timerConfig.base.t13Frequency         = (float)clk;
            timerConfig.base.t13Period            = period;
            timerConfig.timer = IfxCcu6_TimerId_t13;
            timerConfig.interrupt2.source         = IfxCcu6_InterruptSource_t13PeriodMatch;
            timerConfig.interrupt2.priority       = Ccu6IrqPriority[Index];
            timerConfig.interrupt2.typeOfService  = Ccu6IrqVectabNum[ccu6];
            timerConfig.interrupt2.serviceRequest = IfxCcu6_ServiceRequest_2;
            timerConfig.clock.t13countingInputMode = IfxCcu6_CountingInputMode_internal;
            timerConfig.timer13.counterValue      = 0;
            break;
    }

    timerConfig.trigger.t13InSyncWithT12 = FALSE;
    IfxCcu6_Timer_initModule(&Ccu6Timer, &timerConfig);
    IfxCpu_Irq_installInterruptHandler((void*)Ccu6IrqFuncPointer[Index], Ccu6IrqPriority[Index]);//�����жϺ������жϺ�
    restoreInterrupts(interrupt_state);
    IfxCcu6_Timer_start(&Ccu6Timer);
}

/*************************************************************************
 *  �������ƣ�CCU6_DisableInterrupt(CCU6_t ccu6, CCU6_Channel_t channel)
 *  ����˵����ֹͣCCU6ͨ���ж�
 *  ����˵����ccu6    �� ccu6ģ��            CCU60 �� CCU61
 *  ����˵����channel �� ccu6ģ��ͨ��  CCU6_Channel0 �� CCU6_Channel1
 *  �������أ���
 *  �޸�ʱ�䣺2020��5��6��
 *  ��    ע��
 *************************************************************************/
void CCU6_DisableInterrupt (CCU6_t ccu6, CCU6_Channel_t channel)
{
    Ifx_CCU6 * module = IfxCcu6_getAddress((IfxCcu6_Index) ccu6);
    IfxCcu6_clearInterruptStatusFlag(module, (IfxCcu6_InterruptSource) (7 + channel * 2));
    IfxCcu6_disableInterrupt(module, (IfxCcu6_InterruptSource) (7 + channel * 2));
}

/*************************************************************************
 *  �������ƣ�CCU6_DisableInterrupt(CCU6_t ccu6, CCU6_Channel_t channel)
 *  ����˵����ʹ��CCU6ͨ���ж�
 *  ����˵����ccu6    �� ccu6ģ��            CCU60 �� CCU61
 *  ����˵����channel �� ccu6ģ��ͨ��  CCU6_Channel0 �� CCU6_Channel1
 *  �������أ���
 *  �޸�ʱ�䣺2020��5��6��
 *  ��    ע��
 *************************************************************************/
void CCU6_EnableInterrupt (CCU6_t ccu6, CCU6_Channel_t channel)
{
    Ifx_CCU6 * module = IfxCcu6_getAddress((IfxCcu6_Index) ccu6);
    IfxCcu6_clearInterruptStatusFlag(module, (IfxCcu6_InterruptSource) (7 + channel * 2));
    IfxCcu6_enableInterrupt(module, (IfxCcu6_InterruptSource) (7 + channel * 2));
}





















































/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////����ר�õ��Ժ���////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

void initCCU60CH0 (uint64 frq, uint32 prd);
void initCCU60CH1 (uint64 frq, uint32 prd);
void initCCU61CH0 (uint64 frq, uint32 prd);
void initCCU61CH1 (uint64 frq, uint32 prd);
/*************************************************************************
 *  �������ƣ�CCU6_InitConfig CCU6
 *  ����˵������ʱ�������жϳ�ʼ��
 *  ����˵����ccu6    �� ccu6ģ��            CCU60 �� CCU61
 *  ����˵����channel �� ccu6ģ��ͨ��  CCU6_Channel0 �� CCU6_Channel1
 *  ����˵����us      �� ccu6ģ��  �ж�����ʱ��  ��λus
 *  �������أ���
 *  �޸�ʱ�䣺2020��3��30��
 *  ��    ע��    CCU6_InitConfig(CCU60, CCU6_Channel0, 100);  //100us����һ���ж�
 *************************************************************************/
void CCU6_InitConfig2(CCU6_t ccu6, CCU6_Channel_t channel, uint32 us)
{
    IfxCcu6_Timer_Config timerConfig;
    Ifx_CCU6 * module = IfxCcu6_getAddress((IfxCcu6_Index) ccu6);
    uint32 period = 0;
    uint64 clk = 0;

    IfxCcu6_Timer_initModuleConfig(&timerConfig, module);
    clk = IfxScuCcu_getSpbFrequency();

    /* ����ʱ��Ƶ��  */
    uint8 i = 0;
    while (i++ < 16)
    {
        period = (uint32) (clk * us / 1000000);
        if (period < 0xffff)
        {
            break;
        }
        else
        {
            clk = clk / 2;
        }
    }

    if (ccu6 == CCU60)
    {
        if (channel == CCU6_Channel0)
        {
            initCCU60CH0(clk, period);
        }
        else if (channel == CCU6_Channel1)
        {
            initCCU60CH1(clk, period);
        }
    }
    else if (ccu6 == CCU61)
    {
        if (channel == CCU6_Channel0)
        {
            initCCU61CH0(clk, period);
        }
        else if (channel == CCU6_Channel1)
        {
            initCCU61CH1(clk, period);
        }
    }
}
void initCCU60CH0 (uint64 frq, uint32 prd)
{
    IfxCcu6_Timer g_timer0;
    IfxCcu6_Timer_Config timerConfig; /* Structure for timer configuration                        */
    IfxCcu6_Timer_initModuleConfig(&timerConfig, &MODULE_CCU60); /* Initialize the timer module structure with default values*/
    timerConfig.base.t12Frequency = (float) frq; /* Configure the frequency of the timer module */
    timerConfig.base.t12Period = prd; /* Configure the period of the timer (16-bit)  */
    timerConfig.timer = IfxCcu6_TimerId_t12; /* Select the timer to be started              */
    timerConfig.interrupt1.source = IfxCcu6_InterruptSource_t12PeriodMatch; /* Set interrupt source                */
    timerConfig.interrupt1.priority = CCU60_CH0_PRIORITY; /* Set the priority of the ISR                 */
    timerConfig.interrupt1.typeOfService = IfxSrc_Tos_cpu0; /* Set the type of service for the interrupt   */
    timerConfig.trigger.t13InSyncWithT12 = FALSE; /* Configure timers synchronization            */

    IfxCcu6_Timer_initModule(&g_timer0, &timerConfig); /* Initialize the CCU6 module                  */
    CCU6_EnableInterrupt (CCU60, CCU6_Channel0);
    IfxCcu6_Timer_start(&g_timer0);
}
void initCCU60CH1 (uint64 frq, uint32 prd)
{
    IfxCcu6_Timer g_timer1;
    IfxCcu6_Timer_Config timerConfig; /* Structure for timer configuration                        */
    IfxCcu6_Timer_initModuleConfig(&timerConfig, &MODULE_CCU60); /* Initialize the timer module structure with default values*/
    timerConfig.base.t13Frequency = (float) frq; /* Configure the frequency of the timer module */
    timerConfig.base.t13Period = prd; /* Configure the period of the timer (16-bit)  */
    timerConfig.timer = IfxCcu6_TimerId_t13; /* Select the timer to be started              */
    timerConfig.interrupt2.source = IfxCcu6_InterruptSource_t13PeriodMatch; /* Set interrupt source                */
    timerConfig.interrupt2.priority = CCU60_CH1_PRIORITY; /* Set the priority of the ISR                 */
    timerConfig.interrupt2.typeOfService = IfxSrc_Tos_cpu0; /* Set the type of service for the interrupt   */
    timerConfig.trigger.t13InSyncWithT12 = FALSE; /* Configure timers synchronization            */

    IfxCcu6_Timer_initModule(&g_timer1, &timerConfig); /* Initialize the CCU6 module                  */
    CCU6_EnableInterrupt (CCU60, CCU6_Channel1);
    IfxCcu6_Timer_start(&g_timer1);
}
void initCCU61CH0 (uint64 frq, uint32 prd)
{
    IfxCcu6_Timer g_timer2;
    IfxCcu6_Timer_Config timerConfig; /* Structure for timer configuration                        */
    IfxCcu6_Timer_initModuleConfig(&timerConfig, &MODULE_CCU61); /* Initialize the timer module structure with default values*/
    timerConfig.base.t12Frequency = (float) frq; /* Configure the frequency of the timer module */
    timerConfig.base.t12Period = prd; /* Configure the period of the timer (16-bit)  */
    timerConfig.timer = IfxCcu6_TimerId_t12; /* Select the timer to be started              */
    timerConfig.interrupt1.source = IfxCcu6_InterruptSource_t12PeriodMatch; /* Set interrupt source                */
    timerConfig.interrupt1.priority = CCU61_CH0_PRIORITY; /* Set the priority of the ISR                 */
    timerConfig.interrupt1.typeOfService = IfxSrc_Tos_cpu0; /* Set the type of service for the interrupt   */
    timerConfig.trigger.t13InSyncWithT12 = FALSE; /* Configure timers synchronization            */

    IfxCcu6_Timer_initModule(&g_timer2, &timerConfig); /* Initialize the CCU6 module                  */
    CCU6_EnableInterrupt (CCU61, CCU6_Channel0);
    IfxCcu6_Timer_start(&g_timer2);
}
void initCCU61CH1 (uint64 frq, uint32 prd)
{
    IfxCcu6_Timer g_timer3;
    IfxCcu6_Timer_Config timerConfig; /* Structure for timer configuration                        */
    IfxCcu6_Timer_initModuleConfig(&timerConfig, &MODULE_CCU61); /* Initialize the timer module structure with default values*/
    timerConfig.base.t13Frequency = (float) frq; /* Configure the frequency of the timer module */
    timerConfig.base.t13Period = prd; /* Configure the period of the timer (16-bit)  */
    timerConfig.timer = IfxCcu6_TimerId_t13; /* Select the timer to be started              */
    timerConfig.interrupt2.source = IfxCcu6_InterruptSource_t13PeriodMatch; /* Set interrupt source                */
    timerConfig.interrupt2.priority = CCU61_CH1_PRIORITY; /* Set the priority of the ISR                 */
    timerConfig.interrupt2.typeOfService = IfxSrc_Tos_cpu0; /* Set the type of service for the interrupt   */
    timerConfig.trigger.t13InSyncWithT12 = FALSE; /* Configure timers synchronization            */

    IfxCcu6_Timer_initModule(&g_timer3, &timerConfig); /* Initialize the CCU6 module                  */
    CCU6_EnableInterrupt (CCU61, CCU6_Channel1);
    IfxCcu6_Timer_start(&g_timer3);
}

