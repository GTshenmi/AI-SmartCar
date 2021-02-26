/*
 * adc.h
 *
 *  Created on: 2020Äê12ÔÂ3ÈÕ
 *      Author: GT_shenmi
 */

#ifndef DRIVER_SYSDRIVER_ADC_H_
#define DRIVER_SYSDRIVER_ADC_H_

#include "LQ_ADC.h"
#include "platform.h"

typedef struct
{
        ADC_Channel_t Channel;
        uint32_t Freq;
}adcx_t;

typedef struct
{
        uint8_t (*Init)(adcx_t *);
        uint16_t (*Read)(adcx_t *);
        uint16_t MaxValue;
        uint16_t MinValue;
}sadc_m;

extern sadc_m ADCx;

#endif /* DRIVER_SYSDRIVER_ADC_H_ */
