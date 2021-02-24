#ifndef __LQPWM_H__
#define __LQPWM_H__
#include "LQ_GPIO_Cfg.h"
#include "fsl_pwm.h"

/* 最大占空比 可自行修改 */
#define DUTY_MAX  10000  

/**********************************  PWM(引脚复用) *******************************************************/
//     PWM输出通道                  端口       可选范围                   建议
#define PWM1_MODULE0_PWMA             G2      //  G2  J4                   G2接外部SDRAM
#define PWM1_MODULE0_PWMB             D3      //  D3  J3                   D3接外部SDRAM
#define PWM1_MODULE1_PWMA             D2      //  D2  J1                   D2接外部SDRAM
#define PWM1_MODULE1_PWMB             B3      //  B3  K1                   B3接外部SDRAM
#define PWM1_MODULE2_PWMA             H2      //  A2  H2                   A2接外部SDRAM
#define PWM1_MODULE2_PWMB             J2      //  D1  J2                   D1接外部SDRAM
#define PWM1_MODULE3_PWMA             L5      //  A11 D6  G13  H1  L5      H1 D6接外部SDRAM
#define PWM1_MODULE3_PWMB             M5      //  A6  B7  B11  G10 M5      A6 B7接外部SDRAM

#define PWM2_MODULE0_PWMA             A8      //  A8  H5                   H5 接外部SDRAM
#define PWM2_MODULE0_PWMB             A9      //  A9  H4                   H4 接外部SDRAM
#define PWM2_MODULE1_PWMA             B9      //  B9  H3                   H3 接外部SDRAM
#define PWM2_MODULE1_PWMB             C9      //  C2  C9                   C2 接外部SDRAM
#define PWM2_MODULE2_PWMA             G1      //  D9  G1                   G1 接外部SDRAM
#define PWM2_MODULE2_PWMB             G3      //  A10 G3                   G3 接外部SDRAM
#define PWM2_MODULE3_PWMA             M3      //  B4  C11  F14 M3  M14     B4 接外部SDRAM  F14 JTAG TDI接口
#define PWM2_MODULE3_PWMB             M4      //  A3  D11  H10 M4          A3 接外部SDRAM

#define PWM3_MODULE0_PWMA             E1      //  E1                       E1 接外部SDRAM
#define PWM3_MODULE0_PWMB             C6      //  C6                       C6 接外部SDRAM
#define PWM3_MODULE1_PWMA             C5      //  C5                       C5 接外部SDRAM
#define PWM3_MODULE1_PWMB             D5      //  D5                       D5 接外部SDRAM
#define PWM3_MODULE2_PWMA             C4      //  C4                       C4 接外部SDRAM
#define PWM3_MODULE2_PWMB             D4      //  D4                       D4 接外部SDRAM
#define PWM3_MODULE3_PWMA             C1      //  C1                       C1 接外部SDRAM
#define PWM3_MODULE3_PWMB             F1      //  F1                       F1 接外部SDRAM

#define PWM4_MODULE0_PWMA             H13     //  E3  H13                  E3 接外部SDRAM
#define PWM4_MODULE0_PWMB             F3      //  F3                       F3 接外部SDRAM
#define PWM4_MODULE1_PWMA             M13     //  F4  M13                  F4 接外部SDRAM
#define PWM4_MODULE1_PWMB             G4      //  G4                       G4 接外部SDRAM
#define PWM4_MODULE2_PWMA             C14     //  F2  C14                  F2 接外部SDRAM
#define PWM4_MODULE2_PWMB             G5      //  G5                       G5 接外部SDRAM
#define PWM4_MODULE3_PWMA             A4      //  A4  B14                  A4 接外部SDRAM
#define PWM4_MODULE3_PWMB             B2      //  B2                       B2 接外部SDRAM




/**
  * @brief    同时设置 一个子模块的A 和 B相  用这个函数
  *
  * @param    base          ：PWM模块 PWM1~PWM4
  * @param    subModule     ：PWM模块下的子模块 kPWM_Module_0 ~ kPWM_Module_3 
  * @param    dutyA         ：占空比*DUTY_MAX  
  * @param    dutyB         ：占空比*DUTY_MAX  
  *
  * @return   
  *
  * @note     同时设置 一个子模块的A 和 B相 可以 用这个函数
  * @note     默认对齐方式kPWM_SignedCenterAligned， 可以自行修改  
  *
  * @example  PWM_SetABDuty(PWM2, kPWM_Module_2, 500, 1000); //设置 PWM1 子模块0的通道A 占空比= 500/DUTY_MAX 子模块0的通道B 占空比= 1000/DUTY_MAX
  *
  * @date     2019/5/23 星期四
  */
void PWM_SetABDuty(PWM_Type *base, pwm_submodule_t subModule, uint16_t dutyA, uint16_t dutyB);


/**
  * @brief    设置占空比
  *
  * @param    base          ：PWM模块 PWM1~PWM4
  * @param    subModule     ：PWM模块下的子模块 kPWM_Module_0 ~ kPWM_Module_3 
  * @param    pwm_channels  ：PWM模块下的子模块 的通道 ： kPWM_PwmA 、 kPWM_PwmB  、 kPWM_PwmA_B 
  * @param    duty          ：占空比*DUTY_MAX  
  *
  * @return   
  *
  * @note     默认对齐方式kPWM_SignedCenterAligned， 可以自行修改  
  *
  * @example  PWM_SetDuty(PWM1,    kPWM_Module_0, kPWM_PwmA, 500); //设置 PWM1 子模块0的通道A 占空比= 500/DUTY_MAX
  *
  * @date     2019/5/23 星期四
  */
void PWM_SetDuty(PWM_Type *base, pwm_submodule_t subModule, pwm_channels_t pwm_channels, uint16_t duty);



/**
  * @brief    PWM功能初始化
  *
  * @param    base          ：PWM模块 PWM1~PWM4
  * @param    subModule     ：PWM模块下的子模块 kPWM_Module_0 ~ kPWM_Module_3 
  * @param    pwm_channels  ：PWM模块下的子模块 的通道 ： kPWM_PwmA 、 kPWM_PwmB  、 kPWM_PwmA_B 
  * @param    Frequency     ：PWM模块下的子模块的频率
  *
  * @return   
  *
  * @note     PWM 使用 IPG时钟  150M
  *
  * @example  PWM_InitConfig(PWM1, kPWM_Module_0, kPWM_PwmA, 18000);  //初始化 PWM1 子模块0的通道A 并且子模块0的频率18K
  *
  * @date     2019/5/23 星期四
  */
void PWM_InitConfig(PWM_Type *base, pwm_submodule_t subModule, pwm_channels_t pwm_channels, uint16_t Frequency);


      

/**
  * @brief    PWM引脚初始化
  *
  * @param    base          ：PWM模块 PWM1~PWM4
  * @param    subModule     ：PWM模块下的子模块 kPWM_Module_0 ~ kPWM_Module_3 
  * @param    pwm_channels  ：PWM模块下的子模块 的通道 ： kPWM_PwmA 、 kPWM_PwmB  、 kPWM_PwmA_B 
  *
  * @return   
  *
  * @note     内部调用的，仅用来设置串口管脚复用的  
  *
  * @example  
  *
  * @date     2019/5/23 星期四
  */
void PWM_InitPins(PWM_Type *base, pwm_submodule_t subModule, pwm_channels_t pwm_channels);


/**
  * @brief    改变占空比
  *
  * @param    base       ：PWM模块 PWM1~PWM4
  * @param    subModule  ：PWM模块下的子模块 kPWM_Module_0 ~ kPWM_Module_3 
  * @param    pwmSignal  ：kPWM_PwmA/kPWM_PwmB
  * @param    currPwmMode：对齐方式
  * @param    duty       ：占空比*DUTY_MAX
  *
  * @return   无
  *
  * @note     内部调用
  *
  * @example  
  *
  * @date     2019/5/23 星期四
  */
void PWM_UpdateDuty(PWM_Type *base,
                            pwm_submodule_t subModule,
                            pwm_channels_t pwmSignal,
                            pwm_mode_t currPwmMode,
                            uint16_t duty);

#endif 



