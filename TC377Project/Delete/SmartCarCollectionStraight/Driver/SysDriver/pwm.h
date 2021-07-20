/*
 * pwm.h
 *
 *  Created on: 2020Äê12ÔÂ3ÈÕ
 *      Author: GT_shenmi
 */

#ifndef DRIVER_SYSDRIVER_PWM_H_
#define DRIVER_SYSDRIVER_PWM_H_

#include "platform.h"
#include "common.h"

typedef enum
{
        TOM,ATOM,PwmDefaultConfig,
}pwm_source_t;

typedef struct
{
        IfxGtm_Tom_ToutMap Pin;
        uint32_t Freq;
        uint32_t Duty;
}tom_pwm_t;

typedef struct
{
        IfxGtm_Atom_ToutMap Pin;
        uint32_t Freq;
        uint32_t Duty;
}atom_pwm_t;

typedef struct
{
        void *Pin; /*IfxGtm_Atom_ToutMap,IfxGtm_Tom_ToutMap*/
        uint32_t Freq;
        uint32_t Duty;
        pwm_source_t pwm_source;

        uint32_t MinUseDuty;
        uint32_t MaxUseDuty;
}pwmx_t;

typedef struct
{
        uint8_t (*Init)(pwmx_t *);
        uint8_t (*Write)(pwmx_t *,uint32_t duty);
        uint32_t MaxPwmDuty;
        uint32_t MinPwmDuty;
}spwm_m;

extern spwm_m PWMx;

#endif /* DRIVER_SYSDRIVER_PWM_H_ */
