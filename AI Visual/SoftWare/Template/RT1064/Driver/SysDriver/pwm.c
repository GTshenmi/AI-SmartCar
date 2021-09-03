/*
 * pwm.c
 *
 *  Created on: 2020Äê12ÔÂ3ÈÕ
 *      Author: GT_shenmi
 */
#include <pwm.h>

uint8_t PWMx_Init(pwmx_t *pwm)
{
    PWM_InitConfig(pwm->Base,pwm->SubModule,pwm->Channels,pwm->Freq);
    return 0;
}
uint8_t PWMx_Write(pwmx_t *pwm,uint32_t duty)
{
    if(duty > PWMx.MaxPwmDuty)
        duty = PWMx.MaxPwmDuty;
    else if(duty < PWMx.MinPwmDuty)
        duty = PWMx.MinPwmDuty;

    pwm->Duty = duty;
    
    PWM_SetDuty(pwm->Base,pwm->SubModule,pwm->Channels,pwm->Duty);

    return 0;
}
spwm_m PWMx =
{
        .Init = PWMx_Init,
        .Write = PWMx_Write,
        .MaxPwmDuty = 10000,
        .MinPwmDuty = 0,
};



