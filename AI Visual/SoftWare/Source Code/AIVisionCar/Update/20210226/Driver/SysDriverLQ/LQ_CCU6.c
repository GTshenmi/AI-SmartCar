/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】北京龙邱智能科技TC3xxxA核心板
【编    写】chiusir
【E-mail】chiusir@163.com
【软件版本】V1.1 版权所有，单位使用请先联系授权
【最后更新】2020年10月28日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://longqiu.taobao.com
------------------------------------------------
【dev.env.】AURIX Development Studio1.2.2及以上版本
【Target 】 TC3xxxA
【Crystal】 20.000Mhz
【SYS PLL】 300MHz
________________________________________________________________
基于iLLD_1_0_1_11_0底层程序,
使用例程的时候，建议采用没有空格的英文路径，
=================================================================
程序配套视频地址：https://space.bilibili.com/95313236
 ________________________________________________________________
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ
 *  TC397 有两个CCU6模块  每个模块有两个独立定时器  触发定时器中断
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include "LQ_CCU6.h"

#include <IfxCpu_Irq.h>

//#include "LQ_GPT12_ENC.h"

//volatile sint16 ECPULSE1 = 0;          // 速度全局变量
//volatile sint16 ECPULSE2 = 0;          // 速度全局变量
//volatile sint32 RAllPulse = 0;          // 速度全局变量

IFX_INTERRUPT(CCU60_CH0_IRQHandler, CCU60_VECTABNUM, CCU60_CH0_PRIORITY);
IFX_INTERRUPT(CCU60_CH1_IRQHandler, CCU60_VECTABNUM, CCU60_CH1_PRIORITY);
IFX_INTERRUPT(CCU61_CH0_IRQHandler, CCU61_VECTABNUM, CCU61_CH0_PRIORITY);
IFX_INTERRUPT(CCU61_CH1_IRQHandler, CCU61_VECTABNUM, CCU61_CH1_PRIORITY);

/** CCU6中断CPU标号 */
const uint8 Ccu6IrqVectabNum[2] = {CCU60_VECTABNUM, CCU61_VECTABNUM};

/** CCU6中断优先级 */
const uint8 Ccu6IrqPriority[4] = {CCU60_CH0_PRIORITY, CCU60_CH1_PRIORITY, CCU61_CH0_PRIORITY, CCU61_CH1_PRIORITY};

/** CCU6中断服务函数地址 */
const void *Ccu6IrqFuncPointer[4] = {&CCU60_CH0_IRQHandler, &CCU60_CH1_IRQHandler, &CCU61_CH0_IRQHandler,&CCU61_CH1_IRQHandler};

/***********************************************************************************************/
/********************************CCU6外部中断  服务函数******************************************/
/***********************************************************************************************/

/*************************************************************************
 *  函数名称：void CCU60_CH0_IRQHandler(void)
 *  功能说明：
 *  参数说明：无
 *  函数返回：无
 *  修改时间：2020年3月30日
 *  备    注：CCU60_CH0使用的中断服务函数
 *************************************************************************/
void CCU60_CH0_IRQHandler (void)
{
    /* 开启CPU中断  否则中断不可嵌套 */
    IfxCpu_enableInterrupts();

    //清除中断标志
    IfxCcu6_clearInterruptStatusFlag(&MODULE_CCU60, IfxCcu6_InterruptSource_t12PeriodMatch);

    /* 用户代码 */
  //  LED_Ctrl(LED3,RVS);

}

/*************************************************************************
 *  函数名称：void CCU60_CH1_IRQHandler(void)
 *  功能说明：
 *  参数说明：无
 *  函数返回：无
 *  修改时间：2020年3月30日
 *  备    注：CCU60_CH1使用的中断服务函数
 *************************************************************************/
void CCU60_CH1_IRQHandler (void)
{
    /* 开启CPU中断  否则中断不可嵌套 */
    IfxCpu_enableInterrupts();

    //清除中断标志
    IfxCcu6_clearInterruptStatusFlag(&MODULE_CCU60, IfxCcu6_InterruptSource_t13PeriodMatch);

    /* 用户代码 */
  //  LED_Ctrl(LED2,RVS);

}

/*************************************************************************
 *  函数名称：void CCU61_CH0_IRQHandler(void)
 *  功能说明：
 *  参数说明：无
 *  函数返回：无
 *  修改时间：2020年3月30日
 *  备    注：CCU61_CH0使用的中断服务函数
 *************************************************************************/
void CCU61_CH0_IRQHandler(void)
{
    /* 开启CPU中断  否则中断不可嵌套 */
    IfxCpu_enableInterrupts();

    //清除中断标志
    IfxCcu6_clearInterruptStatusFlag(&MODULE_CCU61, IfxCcu6_InterruptSource_t12PeriodMatch);

//    /* 用户代码 */
//    /* 获取编码器值 */
//    ECPULSE1 = ENC_GetCounter(ENC2_InPut_P33_7); // 左电机 母板上编码器1，小车前进为负值
//    ECPULSE2 = ENC_GetCounter(ENC4_InPut_P02_8); // 右电机 母板上编码器2，小车前进为正值
//    RAllPulse += ECPULSE2;
}

/*************************************************************************
 *  函数名称：void CCU61_CH1_IRQHandler(void)
 *  功能说明：
 *  参数说明：无
 *  函数返回：无
 *  修改时间：2020年3月30日
 *  备    注：CCU61_CH1使用的中断服务函数
 *************************************************************************/
void CCU61_CH1_IRQHandler (void)
{
    /* 开启CPU中断  否则中断不可嵌套 */
    IfxCpu_enableInterrupts();

    //清除中断标志
    IfxCcu6_clearInterruptStatusFlag(&MODULE_CCU61, IfxCcu6_InterruptSource_t13PeriodMatch);

    /* 用户代码 */
   // LED_Ctrl(LED0,RVS);
}

/*************************************************************************
 *  函数名称：CCU6_InitConfig CCU6
 *  功能说明：定时器周期中断初始化
 *  参数说明：ccu6    ： ccu6模块            CCU60 、 CCU61
 *  参数说明：channel ： ccu6模块通道  CCU6_Channel0 、 CCU6_Channel1
 *  参数说明：us      ： ccu6模块  中断周期时间  单位us
 *  函数返回：无
 *  修改时间：2020年3月30日
 *  备    注：    CCU6_InitConfig(CCU60, CCU6_Channel0, 100);  //100us进入一次中断
 *************************************************************************/
void CCU6_InitConfig(CCU6_t ccu6, CCU6_Channel_t channel, uint32 us)
{
    IfxCcu6_Timer_Config timerConfig;
    IfxCcu6_Timer Ccu6Timer;

    Ifx_CCU6 * module = IfxCcu6_getAddress((IfxCcu6_Index)ccu6);
    uint8 Index = ccu6 * 2 + channel;
    uint32 period = 0;
    uint64 clk = 0;

    /* 关闭中断 */
    boolean interrupt_state = disableInterrupts();

    IfxCcu6_Timer_initModuleConfig(&timerConfig, module);
    clk = IfxScuCcu_getSpbFrequency();

    /* 设置时钟频率  */
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
            timerConfig.base.t12Period            = period; //设置定时中断
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
    IfxCpu_Irq_installInterruptHandler((void*)Ccu6IrqFuncPointer[Index], Ccu6IrqPriority[Index]);//配置中断函数和中断号
    restoreInterrupts(interrupt_state);
    IfxCcu6_Timer_start(&Ccu6Timer);
}

/*************************************************************************
 *  函数名称：CCU6_DisableInterrupt(CCU6_t ccu6, CCU6_Channel_t channel)
 *  功能说明：停止CCU6通道中断
 *  参数说明：ccu6    ： ccu6模块            CCU60 、 CCU61
 *  参数说明：channel ： ccu6模块通道  CCU6_Channel0 、 CCU6_Channel1
 *  函数返回：无
 *  修改时间：2020年5月6日
 *  备    注：
 *************************************************************************/
void CCU6_DisableInterrupt (CCU6_t ccu6, CCU6_Channel_t channel)
{
    Ifx_CCU6 * module = IfxCcu6_getAddress((IfxCcu6_Index) ccu6);
    IfxCcu6_clearInterruptStatusFlag(module, (IfxCcu6_InterruptSource) (7 + channel * 2));
    IfxCcu6_disableInterrupt(module, (IfxCcu6_InterruptSource) (7 + channel * 2));
}

/*************************************************************************
 *  函数名称：CCU6_DisableInterrupt(CCU6_t ccu6, CCU6_Channel_t channel)
 *  功能说明：使能CCU6通道中断
 *  参数说明：ccu6    ： ccu6模块            CCU60 、 CCU61
 *  参数说明：channel ： ccu6模块通道  CCU6_Channel0 、 CCU6_Channel1
 *  函数返回：无
 *  修改时间：2020年5月6日
 *  备    注：
 *************************************************************************/
void CCU6_EnableInterrupt (CCU6_t ccu6, CCU6_Channel_t channel)
{
    Ifx_CCU6 * module = IfxCcu6_getAddress((IfxCcu6_Index) ccu6);
    IfxCcu6_clearInterruptStatusFlag(module, (IfxCcu6_InterruptSource) (7 + channel * 2));
    IfxCcu6_enableInterrupt(module, (IfxCcu6_InterruptSource) (7 + channel * 2));
}





















































/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////测试专用调试函数////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

void initCCU60CH0 (uint64 frq, uint32 prd);
void initCCU60CH1 (uint64 frq, uint32 prd);
void initCCU61CH0 (uint64 frq, uint32 prd);
void initCCU61CH1 (uint64 frq, uint32 prd);
/*************************************************************************
 *  函数名称：CCU6_InitConfig CCU6
 *  功能说明：定时器周期中断初始化
 *  参数说明：ccu6    ： ccu6模块            CCU60 、 CCU61
 *  参数说明：channel ： ccu6模块通道  CCU6_Channel0 、 CCU6_Channel1
 *  参数说明：us      ： ccu6模块  中断周期时间  单位us
 *  函数返回：无
 *  修改时间：2020年3月30日
 *  备    注：    CCU6_InitConfig(CCU60, CCU6_Channel0, 100);  //100us进入一次中断
 *************************************************************************/
void CCU6_InitConfig2(CCU6_t ccu6, CCU6_Channel_t channel, uint32 us)
{
    IfxCcu6_Timer_Config timerConfig;
    Ifx_CCU6 * module = IfxCcu6_getAddress((IfxCcu6_Index) ccu6);
    uint32 period = 0;
    uint64 clk = 0;

    IfxCcu6_Timer_initModuleConfig(&timerConfig, module);
    clk = IfxScuCcu_getSpbFrequency();

    /* 设置时钟频率  */
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

