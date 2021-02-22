/*
 * adc.c
 *
 *  Created on: 2020Äê12ÔÂ3ÈÕ
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
    uint16_t value = ADC_Read(adc->Channel);
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






