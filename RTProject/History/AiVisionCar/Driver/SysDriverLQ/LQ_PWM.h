#ifndef __LQPWM_H__
#define __LQPWM_H__
#include "LQ_GPIO_Cfg.h"
#include "fsl_pwm.h"

/* ���ռ�ձ� �������޸� */
#define DUTY_MAX  10000  

/**********************************  PWM(���Ÿ���) *******************************************************/
//     PWM���ͨ��                  �˿�       ��ѡ��Χ                   ����
#define PWM1_MODULE0_PWMA             G2      //  G2  J4                   G2���ⲿSDRAM
#define PWM1_MODULE0_PWMB             D3      //  D3  J3                   D3���ⲿSDRAM
#define PWM1_MODULE1_PWMA             D2      //  D2  J1                   D2���ⲿSDRAM
#define PWM1_MODULE1_PWMB             B3      //  B3  K1                   B3���ⲿSDRAM
#define PWM1_MODULE2_PWMA             H2      //  A2  H2                   A2���ⲿSDRAM
#define PWM1_MODULE2_PWMB             J2      //  D1  J2                   D1���ⲿSDRAM
#define PWM1_MODULE3_PWMA             L5      //  A11 D6  G13  H1  L5      H1 D6���ⲿSDRAM
#define PWM1_MODULE3_PWMB             M5      //  A6  B7  B11  G10 M5      A6 B7���ⲿSDRAM

#define PWM2_MODULE0_PWMA             A8      //  A8  H5                   H5 ���ⲿSDRAM
#define PWM2_MODULE0_PWMB             A9      //  A9  H4                   H4 ���ⲿSDRAM
#define PWM2_MODULE1_PWMA             B9      //  B9  H3                   H3 ���ⲿSDRAM
#define PWM2_MODULE1_PWMB             C9      //  C2  C9                   C2 ���ⲿSDRAM
#define PWM2_MODULE2_PWMA             G1      //  D9  G1                   G1 ���ⲿSDRAM
#define PWM2_MODULE2_PWMB             G3      //  A10 G3                   G3 ���ⲿSDRAM
#define PWM2_MODULE3_PWMA             M3      //  B4  C11  F14 M3  M14     B4 ���ⲿSDRAM  F14 JTAG TDI�ӿ�
#define PWM2_MODULE3_PWMB             M4      //  A3  D11  H10 M4          A3 ���ⲿSDRAM

#define PWM3_MODULE0_PWMA             E1      //  E1                       E1 ���ⲿSDRAM
#define PWM3_MODULE0_PWMB             C6      //  C6                       C6 ���ⲿSDRAM
#define PWM3_MODULE1_PWMA             C5      //  C5                       C5 ���ⲿSDRAM
#define PWM3_MODULE1_PWMB             D5      //  D5                       D5 ���ⲿSDRAM
#define PWM3_MODULE2_PWMA             C4      //  C4                       C4 ���ⲿSDRAM
#define PWM3_MODULE2_PWMB             D4      //  D4                       D4 ���ⲿSDRAM
#define PWM3_MODULE3_PWMA             C1      //  C1                       C1 ���ⲿSDRAM
#define PWM3_MODULE3_PWMB             F1      //  F1                       F1 ���ⲿSDRAM

#define PWM4_MODULE0_PWMA             H13     //  E3  H13                  E3 ���ⲿSDRAM
#define PWM4_MODULE0_PWMB             F3      //  F3                       F3 ���ⲿSDRAM
#define PWM4_MODULE1_PWMA             M13     //  F4  M13                  F4 ���ⲿSDRAM
#define PWM4_MODULE1_PWMB             G4      //  G4                       G4 ���ⲿSDRAM
#define PWM4_MODULE2_PWMA             C14     //  F2  C14                  F2 ���ⲿSDRAM
#define PWM4_MODULE2_PWMB             G5      //  G5                       G5 ���ⲿSDRAM
#define PWM4_MODULE3_PWMA             A4      //  A4  B14                  A4 ���ⲿSDRAM
#define PWM4_MODULE3_PWMB             B2      //  B2                       B2 ���ⲿSDRAM




/**
  * @brief    ͬʱ���� һ����ģ���A �� B��  ���������
  *
  * @param    base          ��PWMģ�� PWM1~PWM4
  * @param    subModule     ��PWMģ���µ���ģ�� kPWM_Module_0 ~ kPWM_Module_3 
  * @param    dutyA         ��ռ�ձ�*DUTY_MAX  
  * @param    dutyB         ��ռ�ձ�*DUTY_MAX  
  *
  * @return   
  *
  * @note     ͬʱ���� һ����ģ���A �� B�� ���� ���������
  * @note     Ĭ�϶��뷽ʽkPWM_SignedCenterAligned�� ���������޸�  
  *
  * @example  PWM_SetABDuty(PWM2, kPWM_Module_2, 500, 1000); //���� PWM1 ��ģ��0��ͨ��A ռ�ձ�= 500/DUTY_MAX ��ģ��0��ͨ��B ռ�ձ�= 1000/DUTY_MAX
  *
  * @date     2019/5/23 ������
  */
void PWM_SetABDuty(PWM_Type *base, pwm_submodule_t subModule, uint16_t dutyA, uint16_t dutyB);


/**
  * @brief    ����ռ�ձ�
  *
  * @param    base          ��PWMģ�� PWM1~PWM4
  * @param    subModule     ��PWMģ���µ���ģ�� kPWM_Module_0 ~ kPWM_Module_3 
  * @param    pwm_channels  ��PWMģ���µ���ģ�� ��ͨ�� �� kPWM_PwmA �� kPWM_PwmB  �� kPWM_PwmA_B 
  * @param    duty          ��ռ�ձ�*DUTY_MAX  
  *
  * @return   
  *
  * @note     Ĭ�϶��뷽ʽkPWM_SignedCenterAligned�� ���������޸�  
  *
  * @example  PWM_SetDuty(PWM1,    kPWM_Module_0, kPWM_PwmA, 500); //���� PWM1 ��ģ��0��ͨ��A ռ�ձ�= 500/DUTY_MAX
  *
  * @date     2019/5/23 ������
  */
void PWM_SetDuty(PWM_Type *base, pwm_submodule_t subModule, pwm_channels_t pwm_channels, uint16_t duty);



/**
  * @brief    PWM���ܳ�ʼ��
  *
  * @param    base          ��PWMģ�� PWM1~PWM4
  * @param    subModule     ��PWMģ���µ���ģ�� kPWM_Module_0 ~ kPWM_Module_3 
  * @param    pwm_channels  ��PWMģ���µ���ģ�� ��ͨ�� �� kPWM_PwmA �� kPWM_PwmB  �� kPWM_PwmA_B 
  * @param    Frequency     ��PWMģ���µ���ģ���Ƶ��
  *
  * @return   
  *
  * @note     PWM ʹ�� IPGʱ��  150M
  *
  * @example  PWM_InitConfig(PWM1, kPWM_Module_0, kPWM_PwmA, 18000);  //��ʼ�� PWM1 ��ģ��0��ͨ��A ������ģ��0��Ƶ��18K
  *
  * @date     2019/5/23 ������
  */
void PWM_InitConfig(PWM_Type *base, pwm_submodule_t subModule, pwm_channels_t pwm_channels, uint16_t Frequency);


      

/**
  * @brief    PWM���ų�ʼ��
  *
  * @param    base          ��PWMģ�� PWM1~PWM4
  * @param    subModule     ��PWMģ���µ���ģ�� kPWM_Module_0 ~ kPWM_Module_3 
  * @param    pwm_channels  ��PWMģ���µ���ģ�� ��ͨ�� �� kPWM_PwmA �� kPWM_PwmB  �� kPWM_PwmA_B 
  *
  * @return   
  *
  * @note     �ڲ����õģ����������ô��ڹܽŸ��õ�  
  *
  * @example  
  *
  * @date     2019/5/23 ������
  */
void PWM_InitPins(PWM_Type *base, pwm_submodule_t subModule, pwm_channels_t pwm_channels);


/**
  * @brief    �ı�ռ�ձ�
  *
  * @param    base       ��PWMģ�� PWM1~PWM4
  * @param    subModule  ��PWMģ���µ���ģ�� kPWM_Module_0 ~ kPWM_Module_3 
  * @param    pwmSignal  ��kPWM_PwmA/kPWM_PwmB
  * @param    currPwmMode�����뷽ʽ
  * @param    duty       ��ռ�ձ�*DUTY_MAX
  *
  * @return   ��
  *
  * @note     �ڲ�����
  *
  * @example  
  *
  * @date     2019/5/23 ������
  */
void PWM_UpdateDuty(PWM_Type *base,
                            pwm_submodule_t subModule,
                            pwm_channels_t pwmSignal,
                            pwm_mode_t currPwmMode,
                            uint16_t duty);

#endif 



