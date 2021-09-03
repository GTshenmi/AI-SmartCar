/*
 * adc.c
 *
 *  Created on: 2020年12月3日
 *      Author: GT_shenmi
 */
#include <adc.h>

uint8_t ADCx_Init(adcx_t *adc)
{
    if(adc == NULL)
       return 1;

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

    if(adc->Freq > 1000000)
    {
        adc->Freq = 1000000;
    }

    /* 设置采样时间 */
    adcGroupConfig.inputClass[0].sampleTime = 1.0f/adc->Freq;
    adcGroupConfig.inputClass[1].sampleTime = 1.0f/adc->Freq;

#pragma warning 547         // 屏蔽警告
    //修改组
    adcGroupConfig.groupId = ADC_GetGroup(adc->Channel);
    //重要说明:通常用同一个组作为master!
    adcGroupConfig.master = ADC_GetGroup(adc->Channel);
#pragma warning default     // 打开警告
    //启动全部仲裁请求源，
    adcGroupConfig.arbiter.requestSlotQueue0Enabled = TRUE;

    //使能所有的门为 "always" 模式 (无边沿检测)
    adcGroupConfig.queueRequest[IfxEvadc_RequestSource_queue0].triggerConfig.gatingMode = IfxEvadc_GatingMode_always;

    //初始化组
    if(0==adc_group_init_flag[ADC_GetGroup(adc->Channel)])
    {
        adc_group_init_flag[ADC_GetGroup(adc->Channel)] = 1;
        IfxEvadc_Adc_initGroup(&g_AdcGroup[ADC_GetGroup(adc->Channel)], &adcGroupConfig);
    }
    else
    {
        g_AdcGroup[ADC_GetGroup(adc->Channel)].groupId = adcGroupConfig.groupId;
        g_AdcGroup[ADC_GetGroup(adc->Channel)].module = *adcGroupConfig.module;
        g_AdcGroup[ADC_GetGroup(adc->Channel)].group = &adcGroupConfig.module->evadc->G[g_AdcGroup[ADC_GetGroup(adc->Channel)].groupId];
    }

    IfxEvadc_Adc_initChannelConfig(&adcChannelConfig, &g_AdcGroup[ADC_GetGroup(adc->Channel)]);//初始化
    adcChannelConfig.channelId = (IfxEvadc_ChannelId)(ADC_GetChannel(adc->Channel));           //通道ID选择
    adcChannelConfig.resultRegister = (IfxEvadc_ChannelResult)(ADC_GetChannel(adc->Channel));  //对应通道转换结果结存器选择

    //使用专用结果寄存器初始化通道
    IfxEvadc_Adc_initChannel(&g_AdcChannel[ADC_GetIndex(adc->Channel)], &adcChannelConfig);
    /* Start the queue */
    IfxEvadc_Adc_startQueue(&g_AdcGroup[ADC_GetGroup(adc->Channel)], IfxEvadc_RequestSource_queue0);
    IfxEvadc_Adc_addToQueue(&g_AdcChannel[ADC_GetIndex(adc->Channel)], IfxEvadc_RequestSource_queue0, IFXEVADC_QUEUE_REFILL);

    return 0;
}
uint16_t ADCx_Read(adcx_t *adc)
{
    uint16_t value = 0;

    if(adc == NULL)
        return 1;

    do{
        conversionResult = IfxEvadc_Adc_getResult(&g_AdcChannel[ADC_GetIndex(adc->Channel)]);
    } while( !conversionResult.B.VF );

    value = conversionResult.B.RESULT;//返回转换结果

    if(value > ADCx.MaxValue)
        value = ADCx.MaxValue;
    else if(value <= ADCx.MinValue)
        value = ADCx.MinValue;

    return value;
}
sadc_m ADCx =
{
        .Init = ADCx_Init,
        .Read = ADCx_Read,
        .MaxValue = 4096,
        .MinValue = 0,
};






