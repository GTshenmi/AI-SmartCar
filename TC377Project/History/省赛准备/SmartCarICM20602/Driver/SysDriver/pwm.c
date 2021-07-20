/*
 * pwm.c
 *
 *  Created on: 2020年12月3日
 *      Author: GT_shenmi
 */
#include <pwm.h>

uint8_t PWMx_Init(pwmx_t *pwm)
{
    pwm_source_t pwm_source = pwm->pwm_source;
    if(pwm_source == TOM)
    {
        IfxGtm_Tom_ToutMap *pin =(IfxGtm_Tom_ToutMap *) pwm->Pin;

        static uint8 count = 0;

        if(count == 0)
        {
            IfxGtm_enable(&MODULE_GTM); /* 使能 GTM */

            IfxGtm_Cmu_enableClocks(&MODULE_GTM, IFXGTM_CMU_CLKEN_FXCLK);       /* Enable the FXU clocks               */

            count ++;
        }


        IfxGtm_Tom_Pwm_initConfig(&tomConfig, &MODULE_GTM);                 /* Initialize default parameters            */

        tomConfig.tom = pin->tom;                                 //选择PWM输出管脚
        tomConfig.tomChannel = pin->channel;                      //选择PWM输出管脚对应的通道
        tomConfig.pin.outputPin = pin;                           //设置输出管脚
        tomConfig.period = TOM_PWM_CLK / pwm->Freq;              //设置输出周期
        tomConfig.dutyCycle = (unsigned long)(pwm->Duty * ((float)tomConfig.period / TOM_PWM_MAX));//设置占空比
        tomConfig.synchronousUpdateEnabled = TRUE;                        //使能PWM同步更新
        tomConfig.clock = IfxGtm_Tom_Ch_ClkSrc_cmuFxclk1;                   /* Select the FXU clock 1              */

        uint8 tempNum = (uint8)pin->tom * 16 + (uint8)pin->channel;
        IfxGtm_Tom_Pwm_init(&tomDriverPWM[tempNum], &tomConfig);         /* Initialize the module                    */
        IfxGtm_Tom_Pwm_start(&tomDriverPWM[tempNum], TRUE);              /* Start the generation of the PWM signal   */
    }
    else if(pwm_source == ATOM)
    {
        IfxGtm_Atom_ToutMap *pin =(IfxGtm_Atom_ToutMap *) pwm->Pin;

        static uint8 count = 0;

        if(count == 0)
        {
            IfxGtm_enable(&MODULE_GTM); /* 使能 GTM */

            IfxGtm_Cmu_setClkFrequency(&MODULE_GTM, IfxGtm_Cmu_Clk_0, ATOM_PWM_CLK); //设置 CMU clock 100M 频率
            IfxGtm_Cmu_enableClocks(&MODULE_GTM, IFXGTM_CMU_CLKEN_CLK0);             //使能 CMU clock 0

            count ++;
        }


        IfxGtm_Atom_Pwm_initConfig(&g_atomConfig, &MODULE_GTM);

        g_atomConfig.atom = pin->atom;                                //选择PWM输出管脚
        g_atomConfig.atomChannel = pin->channel;                      //选择PWM输出管脚对应的通道
        g_atomConfig.pin.outputPin = pin;                           //设置输出管脚
        g_atomConfig.period = ATOM_PWM_CLK / pwm->Freq;             //设置输出周期
        g_atomConfig.dutyCycle = (unsigned long)(pwm->Duty * ((float)g_atomConfig.period / ATOM_PWM_MAX));//设置占空比
        g_atomConfig.synchronousUpdateEnabled = TRUE;                        //使能PWM同步更新

        uint8 tempNum = (uint8)pin->atom * 8 + (uint8)pin->channel;

        IfxGtm_Atom_Pwm_init(&g_AtomDriverPWM[tempNum], &g_atomConfig);      //ATOM_PWM初始化
        IfxGtm_Atom_Pwm_start(&g_AtomDriverPWM[tempNum], TRUE);              //开始输出PWM
    }
    else
        return 0;

    if(pwm->MaxUseDuty < pwm->MinUseDuty)
        return 1;

    if(pwm->MaxUseDuty > PWMx.MaxPwmDuty)
        pwm->MaxUseDuty = PWMx.MaxPwmDuty;

    if(pwm->MinUseDuty <= PWMx.MinPwmDuty)
        pwm->MinUseDuty = PWMx.MinPwmDuty;


    return 0;
}
uint8_t PWMx_Write(pwmx_t *pwm,unsigned long duty)
{
//    if(duty > PWMx.MaxPwmDuty)
//        duty = PWMx.MaxPwmDuty;
//    else if(duty < PWMx.MinPwmDuty)
//        duty = PWMx.MinPwmDuty;

    if(duty > pwm->MaxUseDuty)
        duty = pwm->MaxUseDuty;
    else if(duty <= PWMx.MinPwmDuty)
        duty = PWMx.MinPwmDuty;

    pwm->Duty = duty;

    pwm_source_t pwm_source = pwm->pwm_source;
    if(pwm_source == TOM)
    {
        IfxGtm_Tom_ToutMap *pin =(IfxGtm_Tom_ToutMap *) pwm->Pin;

        unsigned long period = TOM_PWM_CLK / pwm->Freq;                         //设置输出周期
        unsigned long dutyCycle = (unsigned long)(duty * ((float)period / TOM_PWM_MAX));//设置占空比

        Ifx_GTM_TOM     *atomSFR = &MODULE_GTM.TOM[pin->tom];

        //设置频率
//        IfxGtm_Tom_Ch_setCompareZeroShadow(atomSFR, pin.channel, period);

        //设置占空比
        IfxGtm_Tom_Ch_setCompareOneShadow(atomSFR, pin->channel, dutyCycle);
    }
    else if(pwm_source == ATOM)
    {
        IfxGtm_Atom_ToutMap *pin =(IfxGtm_Atom_ToutMap *) pwm->Pin;

        unsigned long period = ATOM_PWM_CLK / pwm->Freq;                         //设置输出周期
        unsigned long dutyCycle = (unsigned long)(duty * ((float)period / ATOM_PWM_MAX));//设置占空比

        Ifx_GTM_ATOM     *atomSFR = &MODULE_GTM.ATOM[pin->atom];

        //设置频率
//        IfxGtm_Atom_Ch_setCompareZeroShadow(atomSFR, pin.channel, period);

        //设置占空比
        IfxGtm_Atom_Ch_setCompareOneShadow(atomSFR, pin->channel, dutyCycle);

    }
    else
        return 0;
    return 0;
}
spwm_m PWMx =
{
        .Init = PWMx_Init,
        .Write = PWMx_Write,
        .MaxPwmDuty = 10000,
        .MinPwmDuty = 0,
};



