/*
 * adc.c
 *
 *  Created on: 2020年12月3日
 *      Author: GT_shenmi
 */
#include <adc.h>

uint8_t ADCx_Init(adcx_t *adc)
{
    ADC_InitConfig(adc->Channel,adc->Freq);
    return 0;
}
uint16_t ADCx_Read(adcx_t *adc)
{
    uint16_t value = 0;

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






