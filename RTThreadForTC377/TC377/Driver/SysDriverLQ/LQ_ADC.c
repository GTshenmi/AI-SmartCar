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
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ

理论有8个转换器，可同步转换；每个转换8个通道，精度为8/10/12位。默认为最高精度12位。
ADC端口AN可以作为输入口，不可作为输出口，与ARM单片机不同！
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include "LQ_ADC.h"

/**
 * ADC寄存器
 */
IfxEvadc_Adc vadc;

/**
 * ADC转换结构体
 */
Ifx_EVADC_G_RES conversionResult;

/**
 * ADC转换组
 */
IfxEvadc_Adc_Group g_AdcGroup[ADC_GROUPS_NUM];
char adc_init_flag = 0;
char adc_group_init_flag[ADC_GROUPS_NUM];
/**
 * ADC转换通道配置结构体
 */
IfxEvadc_Adc_Channel g_AdcChannel[ADC_CHANNELS_NUM];

/*************************************************************************
*  函数名称：void ADC_init(void)
*  功能说明：ADC初始化函数
*  参数说明：channel  ADC通道
*  参数说明：Freq_Hz  ADC通道采样频率     注意 ADC有三个模块 通道0-13  16-25  35-49分别属于这三个通道  这里使用扫描采样方式，因此如果一个ADC模块同时初始化了N个通道，则ADC实际采用率 = Freq_Hz / N
*  函数返回：无
*  修改时间：2020年5月10日
*  备    注：     注意 adc采样率最高1000000
*************************************************************************/
void ADC_InitConfig(ADC_Channel_t channel, unsigned long Freq_Hz)
{
    //新建配置
    IfxEvadc_Adc_Config adcConfig;
    //新建通道配置
    IfxEvadc_Adc_ChannelConfig adcChannelConfig;
    //新建ADC组配置
    IfxEvadc_Adc_GroupConfig adcGroupConfig;

    IfxEvadc_Adc_initModuleConfig(&adcConfig, &MODULE_EVADC);

    adcConfig.supplyVoltage = IfxEvadc_SupplyVoltageLevelControl_lowerVoltage;

    //初始化模块
    IfxEvadc_Adc_initModule(&vadc, &adcConfig);
    if(0==adc_init_flag)
     {
         adc_init_flag = 1;// 防止覆盖前面金的初始化
         IfxEvadc_Adc_initModule(&vadc, &adcConfig);
     }
     else
     {
         vadc.evadc = adcConfig.evadc;
     }

    IfxEvadc_Adc_initGroupConfig(&adcGroupConfig, &vadc);

    if(Freq_Hz > 1000000)
    {
        Freq_Hz = 1000000;
    }

    /* 设置采样时间 */
    adcGroupConfig.inputClass[0].sampleTime = 1.0f/Freq_Hz;
    adcGroupConfig.inputClass[1].sampleTime = 1.0f/Freq_Hz;

#pragma warning 547         // 屏蔽警告
    //修改组
    adcGroupConfig.groupId = ADC_GetGroup(channel);
    //重要说明:通常用同一个组作为master!
    adcGroupConfig.master = ADC_GetGroup(channel);
#pragma warning default     // 打开警告
    //启动全部仲裁请求源，
    adcGroupConfig.arbiter.requestSlotQueue0Enabled = TRUE;

    //使能所有的门为 "always" 模式 (无边沿检测)
    adcGroupConfig.queueRequest[IfxEvadc_RequestSource_queue0].triggerConfig.gatingMode = IfxEvadc_GatingMode_always;

    //初始化组
    if(0==adc_group_init_flag[ADC_GetGroup(channel)])
    {
        adc_group_init_flag[ADC_GetGroup(channel)] = 1;
        IfxEvadc_Adc_initGroup(&g_AdcGroup[ADC_GetGroup(channel)], &adcGroupConfig);
    }
    else
    {
        g_AdcGroup[ADC_GetGroup(channel)].groupId = adcGroupConfig.groupId;
        g_AdcGroup[ADC_GetGroup(channel)].module = *adcGroupConfig.module;
        g_AdcGroup[ADC_GetGroup(channel)].group = &adcGroupConfig.module->evadc->G[g_AdcGroup[ADC_GetGroup(channel)].groupId];
    }

    IfxEvadc_Adc_initChannelConfig(&adcChannelConfig, &g_AdcGroup[ADC_GetGroup(channel)]);//初始化
    adcChannelConfig.channelId = (IfxEvadc_ChannelId)(ADC_GetChannel(channel));           //通道ID选择
    adcChannelConfig.resultRegister = (IfxEvadc_ChannelResult)(ADC_GetChannel(channel));  //对应通道转换结果结存器选择

    //使用专用结果寄存器初始化通道
    IfxEvadc_Adc_initChannel(&g_AdcChannel[ADC_GetIndex(channel)], &adcChannelConfig);
    /* Start the queue */
    IfxEvadc_Adc_startQueue(&g_AdcGroup[ADC_GetGroup(channel)], IfxEvadc_RequestSource_queue0);
    IfxEvadc_Adc_addToQueue(&g_AdcChannel[ADC_GetIndex(channel)], IfxEvadc_RequestSource_queue0, IFXEVADC_QUEUE_REFILL);
}

/*************************************************************************
*  函数名称：unsigned short ADC_Read(ADC_Channel_t  chn)
*  功能说明：ADC采集
*  参数说明：chn    : ADC通道
*  函数返回：ADC数值
*  修改时间：2020年3月10日
*  备    注：
*************************************************************************/
unsigned short ADC_Read(ADC_Channel_t  chn)
{
    //等待转换完成
    do{
        conversionResult = IfxEvadc_Adc_getResult(&g_AdcChannel[ADC_GetIndex(chn)]);
    } while( !conversionResult.B.VF );

    return  conversionResult.B.RESULT;//返回转换结果
}

/*************************************************************************
*  函数名称：unsigned short ADC_Read(ADC_Channel_t  chn)
*  功能说明：ADC多次采集取平均值
*  参数说明：chn    : ADC通道
*  参数说明：count  : 采集次数
*  函数返回：ADC数值
*  修改时间：2020年3月10日
*  备    注：
*************************************************************************/
unsigned short ADC_ReadAverage(ADC_Channel_t  chn, unsigned char count)
{
    unsigned short i = 0;
    unsigned long sum = 0;
    for(i = 0; i < count; i++)
    {
        sum += ADC_Read(chn);
    }
    return  (unsigned short)(sum/count);//返回转换结果
}
/////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////完/////////////////////////////////////////////////

