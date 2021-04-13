/*
 * pwm.c
 *
 *  Created on: 2020��12��3��
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
            IfxGtm_enable(&MODULE_GTM); /* ʹ�� GTM */

            IfxGtm_Cmu_enableClocks(&MODULE_GTM, IFXGTM_CMU_CLKEN_FXCLK);       /* Enable the FXU clocks               */

            count ++;
        }


        IfxGtm_Tom_Pwm_initConfig(&tomConfig, &MODULE_GTM);                 /* Initialize default parameters            */

        tomConfig.tom = pin->tom;                                 //ѡ��PWM����ܽ�
        tomConfig.tomChannel = pin->channel;                      //ѡ��PWM����ܽŶ�Ӧ��ͨ��
        tomConfig.pin.outputPin = pin;                           //��������ܽ�
        tomConfig.period = TOM_PWM_CLK / pwm->Freq;              //�����������
        tomConfig.dutyCycle = (unsigned long)(pwm->Duty * ((float)tomConfig.period / TOM_PWM_MAX));//����ռ�ձ�
        tomConfig.synchronousUpdateEnabled = TRUE;                        //ʹ��PWMͬ������
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
            IfxGtm_enable(&MODULE_GTM); /* ʹ�� GTM */

            IfxGtm_Cmu_setClkFrequency(&MODULE_GTM, IfxGtm_Cmu_Clk_0, ATOM_PWM_CLK); //���� CMU clock 100M Ƶ��
            IfxGtm_Cmu_enableClocks(&MODULE_GTM, IFXGTM_CMU_CLKEN_CLK0);             //ʹ�� CMU clock 0

            count ++;
        }


        IfxGtm_Atom_Pwm_initConfig(&g_atomConfig, &MODULE_GTM);

        g_atomConfig.atom = pin->atom;                                //ѡ��PWM����ܽ�
        g_atomConfig.atomChannel = pin->channel;                      //ѡ��PWM����ܽŶ�Ӧ��ͨ��
        g_atomConfig.pin.outputPin = pin;                           //��������ܽ�
        g_atomConfig.period = ATOM_PWM_CLK / pwm->Freq;             //�����������
        g_atomConfig.dutyCycle = (unsigned long)(pwm->Duty * ((float)g_atomConfig.period / ATOM_PWM_MAX));//����ռ�ձ�
        g_atomConfig.synchronousUpdateEnabled = TRUE;                        //ʹ��PWMͬ������

        uint8 tempNum = (uint8)pin->atom * 8 + (uint8)pin->channel;

        IfxGtm_Atom_Pwm_init(&g_AtomDriverPWM[tempNum], &g_atomConfig);      //ATOM_PWM��ʼ��
        IfxGtm_Atom_Pwm_start(&g_AtomDriverPWM[tempNum], TRUE);              //��ʼ���PWM
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

        unsigned long period = TOM_PWM_CLK / pwm->Freq;                         //�����������
        unsigned long dutyCycle = (unsigned long)(duty * ((float)period / TOM_PWM_MAX));//����ռ�ձ�

        Ifx_GTM_TOM     *atomSFR = &MODULE_GTM.TOM[pin->tom];

        //����Ƶ��
//        IfxGtm_Tom_Ch_setCompareZeroShadow(atomSFR, pin.channel, period);

        //����ռ�ձ�
        IfxGtm_Tom_Ch_setCompareOneShadow(atomSFR, pin->channel, dutyCycle);
    }
    else if(pwm_source == ATOM)
    {
        IfxGtm_Atom_ToutMap *pin =(IfxGtm_Atom_ToutMap *) pwm->Pin;

        unsigned long period = ATOM_PWM_CLK / pwm->Freq;                         //�����������
        unsigned long dutyCycle = (unsigned long)(duty * ((float)period / ATOM_PWM_MAX));//����ռ�ձ�

        Ifx_GTM_ATOM     *atomSFR = &MODULE_GTM.ATOM[pin->atom];

        //����Ƶ��
//        IfxGtm_Atom_Ch_setCompareZeroShadow(atomSFR, pin.channel, period);

        //����ռ�ձ�
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



