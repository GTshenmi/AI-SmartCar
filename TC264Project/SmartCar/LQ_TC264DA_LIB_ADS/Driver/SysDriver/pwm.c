/*
 * pwm.c
 *
 *  Created on: 2020Äê12ÔÂ3ÈÕ
 *      Author: GT_shenmi
 */
#include <pwm.h>

uint8_t PWMx_Init(pwmx_t *pwm)
{
    pwm_source_t pwm_source = pwm->pwm_source;
    if(pwm_source == TOM)
    {
        IfxGtm_Tom_ToutMap *pin =(IfxGtm_Tom_ToutMap *) pwm->Pin;
        TOM_PWM_InitConfig(*pin,pwm->Duty,pwm->Freq);
    }
    else if(pwm_source == ATOM)
    {
        IfxGtm_Atom_ToutMap *pin =(IfxGtm_Atom_ToutMap *) pwm->Pin;
        ATOM_PWM_InitConfig(*pin,pwm->Duty,pwm->Freq);
    }
    else
        return 1;
    return 0;
}
uint8_t PWMx_Write(pwmx_t *pwm,uint32_t duty)
{
    if(duty > PWMx.MaxPwm)
        duty = PWMx.MaxPwm;
    else if(duty < PWMx.MinPwm)
        duty = PWMx.MinPwm;

    pwm->Duty = duty;

    pwm_source_t pwm_source = pwm->pwm_source;
    if(pwm_source == TOM)
    {
        IfxGtm_Tom_ToutMap *pin =(IfxGtm_Tom_ToutMap *) pwm->Pin;
        TOM_PWM_SetDuty(*pin,pwm->Duty,pwm->Freq);
    }
    else if(pwm_source == ATOM)
    {
        IfxGtm_Atom_ToutMap *pin =(IfxGtm_Atom_ToutMap *) pwm->Pin;
        ATOM_PWM_SetDuty(*pin,pwm->Duty,pwm->Freq);
    }
    else
        return 0;
    return 0;
}
spwm_m PWMx =
{
        .Init = PWMx_Init,
        .Write = PWMx_Write,
        .MaxPwm = 10000,
        .MinPwm = 0,
};



