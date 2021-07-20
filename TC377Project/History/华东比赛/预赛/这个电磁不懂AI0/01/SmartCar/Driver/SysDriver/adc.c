/*
 * adc.c
 *
 *  Created on: 2020��12��3��
 *      Author: GT_shenmi
 */
#include <adc.h>

uint8_t ADCx_Init(adcx_t *adc)
{
    if(adc == NULL)
       return 1;

    //�½�����
    IfxEvadc_Adc_Config adcConfig;
    //�½�ͨ������
    IfxEvadc_Adc_ChannelConfig adcChannelConfig;
    //�½�ADC������
    IfxEvadc_Adc_GroupConfig adcGroupConfig;

    IfxEvadc_Adc_initModuleConfig(&adcConfig, &MODULE_EVADC);

    adcConfig.supplyVoltage = IfxEvadc_SupplyVoltageLevelControl_lowerVoltage;

    //��ʼ��ģ��
    IfxEvadc_Adc_initModule(&vadc, &adcConfig);
    if(0==adc_init_flag)
     {
         adc_init_flag = 1;// ��ֹ����ǰ���ĳ�ʼ��
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

    /* ���ò���ʱ�� */
    adcGroupConfig.inputClass[0].sampleTime = 1.0f/adc->Freq;
    adcGroupConfig.inputClass[1].sampleTime = 1.0f/adc->Freq;

#pragma warning 547         // ���ξ���
    //�޸���
    adcGroupConfig.groupId = ADC_GetGroup(adc->Channel);
    //��Ҫ˵��:ͨ����ͬһ������Ϊmaster!
    adcGroupConfig.master = ADC_GetGroup(adc->Channel);
#pragma warning default     // �򿪾���
    //����ȫ���ٲ�����Դ��
    adcGroupConfig.arbiter.requestSlotQueue0Enabled = TRUE;

    //ʹ�����е���Ϊ "always" ģʽ (�ޱ��ؼ��)
    adcGroupConfig.queueRequest[IfxEvadc_RequestSource_queue0].triggerConfig.gatingMode = IfxEvadc_GatingMode_always;

    //��ʼ����
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

    IfxEvadc_Adc_initChannelConfig(&adcChannelConfig, &g_AdcGroup[ADC_GetGroup(adc->Channel)]);//��ʼ��
    adcChannelConfig.channelId = (IfxEvadc_ChannelId)(ADC_GetChannel(adc->Channel));           //ͨ��IDѡ��
    adcChannelConfig.resultRegister = (IfxEvadc_ChannelResult)(ADC_GetChannel(adc->Channel));  //��Ӧͨ��ת����������ѡ��

    //ʹ��ר�ý���Ĵ�����ʼ��ͨ��
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

    value = conversionResult.B.RESULT;//����ת�����

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






