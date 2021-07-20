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
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ

������8��ת��������ͬ��ת����ÿ��ת��8��ͨ��������Ϊ8/10/12λ��Ĭ��Ϊ��߾���12λ��
ADC�˿�AN������Ϊ����ڣ�������Ϊ����ڣ���ARM��Ƭ����ͬ��
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include <LQ_ADC.h>

/**
 * ADC�Ĵ���
 */
IfxEvadc_Adc vadc;

/**
 * ADCת���ṹ��
 */
Ifx_EVADC_G_RES conversionResult;

/**
 * ADCת����
 */
IfxEvadc_Adc_Group g_AdcGroup[ADC_GROUPS_NUM];
char adc_init_flag = 0;
char adc_group_init_flag[ADC_GROUPS_NUM];
/**
 * ADCת��ͨ�����ýṹ��
 */
IfxEvadc_Adc_Channel g_AdcChannel[ADC_CHANNELS_NUM];

/*************************************************************************
*  �������ƣ�void ADC_init(void)
*  ����˵����ADC��ʼ������
*  ����˵����channel  ADCͨ��
*  ����˵����Freq_Hz  ADCͨ������Ƶ��     ע�� ADC������ģ�� ͨ��0-13  16-25  35-49�ֱ�����������ͨ��  ����ʹ��ɨ�������ʽ��������һ��ADCģ��ͬʱ��ʼ����N��ͨ������ADCʵ�ʲ����� = Freq_Hz / N
*  �������أ���
*  �޸�ʱ�䣺2020��5��10��
*  ��    ע��     ע�� adc���������1000000
*************************************************************************/
void ADC_InitConfig(ADC_Channel_t channel, unsigned long Freq_Hz)
{
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

    if(Freq_Hz > 1000000)
    {
        Freq_Hz = 1000000;
    }

    /* ���ò���ʱ�� */
    adcGroupConfig.inputClass[0].sampleTime = 1.0f/Freq_Hz;
    adcGroupConfig.inputClass[1].sampleTime = 1.0f/Freq_Hz;

#pragma warning 547         // ���ξ���
    //�޸���
    adcGroupConfig.groupId = ADC_GetGroup(channel);
    //��Ҫ˵��:ͨ����ͬһ������Ϊmaster!
    adcGroupConfig.master = ADC_GetGroup(channel);
#pragma warning default     // �򿪾���
    //����ȫ���ٲ�����Դ��
    adcGroupConfig.arbiter.requestSlotQueue0Enabled = TRUE;

    //ʹ�����е���Ϊ "always" ģʽ (�ޱ��ؼ��)
    adcGroupConfig.queueRequest[IfxEvadc_RequestSource_queue0].triggerConfig.gatingMode = IfxEvadc_GatingMode_always;

    //��ʼ����
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

    IfxEvadc_Adc_initChannelConfig(&adcChannelConfig, &g_AdcGroup[ADC_GetGroup(channel)]);//��ʼ��
    adcChannelConfig.channelId = (IfxEvadc_ChannelId)(ADC_GetChannel(channel));           //ͨ��IDѡ��
    adcChannelConfig.resultRegister = (IfxEvadc_ChannelResult)(ADC_GetChannel(channel));  //��Ӧͨ��ת����������ѡ��

    //ʹ��ר�ý���Ĵ�����ʼ��ͨ��
    IfxEvadc_Adc_initChannel(&g_AdcChannel[ADC_GetIndex(channel)], &adcChannelConfig);
    /* Start the queue */
    IfxEvadc_Adc_startQueue(&g_AdcGroup[ADC_GetGroup(channel)], IfxEvadc_RequestSource_queue0);
    IfxEvadc_Adc_addToQueue(&g_AdcChannel[ADC_GetIndex(channel)], IfxEvadc_RequestSource_queue0, IFXEVADC_QUEUE_REFILL);
}

/*************************************************************************
*  �������ƣ�unsigned short ADC_Read(ADC_Channel_t  chn)
*  ����˵����ADC�ɼ�
*  ����˵����chn    : ADCͨ��
*  �������أ�ADC��ֵ
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע��
*************************************************************************/
unsigned short ADC_Read(ADC_Channel_t  chn)
{
    //�ȴ�ת�����
    do{
        conversionResult = IfxEvadc_Adc_getResult(&g_AdcChannel[ADC_GetIndex(chn)]);
    } while( !conversionResult.B.VF );

    return  conversionResult.B.RESULT;//����ת�����
}

/*************************************************************************
*  �������ƣ�unsigned short ADC_Read(ADC_Channel_t  chn)
*  ����˵����ADC��βɼ�ȡƽ��ֵ
*  ����˵����chn    : ADCͨ��
*  ����˵����count  : �ɼ�����
*  �������أ�ADC��ֵ
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע��
*************************************************************************/
unsigned short ADC_ReadAverage(ADC_Channel_t  chn, unsigned char count)
{
    unsigned short i = 0;
    unsigned long sum = 0;
    for(i = 0; i < count; i++)
    {
        sum += ADC_Read(chn);
    }
    return  (unsigned short)(sum/count);//����ת�����
}
/////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////��/////////////////////////////////////////////////

