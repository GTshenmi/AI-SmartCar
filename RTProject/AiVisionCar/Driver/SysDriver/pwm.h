/*
 * pwm.h
 *
 *  Created on: 2020Äê12ÔÂ3ÈÕ
 *      Author: GT_shenmi
 */

#ifndef DRIVER_SYSDRIVER_PWM_H_
#define DRIVER_SYSDRIVER_PWM_H_

#include "LQ_PWM.h" 
#include "chipdatatype.h"

typedef struct
{
        PWM_Type *Base; 
        pwm_submodule_t SubModule; 
        pwm_channels_t  Channels;
        uint16_t Freq;
        uint32_t Duty;
}pwmx_t;

typedef struct
{
        uint8_t (*Init)(pwmx_t *);
        uint8_t (*Write)(pwmx_t *,uint32_t duty);
        uint32_t MaxPwm;
        uint32_t MinPwm;
}spwm_m;

extern spwm_m PWMx;

#endif /* DRIVER_SYSDRIVER_PWM_H_ */
