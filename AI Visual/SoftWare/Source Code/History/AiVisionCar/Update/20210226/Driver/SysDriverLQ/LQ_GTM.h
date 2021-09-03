/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】北京龙邱智能科技TC3xxxA核心板
【编    写】chiusir
【E-mail】chiusir@163.com
【软件版本】V1.1 版权所有，单位使用请先联系授权
【最后更新】2020年10月28日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://longqiu.taobao.com
------------------------------------------------
【dev.env.】AURIX Development Studio1.2.2及以上版本
【Target 】 TC3xxxA
【Crystal】 20.000Mhz
【SYS PLL】 300MHz
________________________________________________________________
基于iLLD_1_0_1_11_0底层程序,
使用例程的时候，建议采用没有空格的英文路径，
=================================================================
程序配套视频地址：https://space.bilibili.com/95313236
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#ifndef _LQ_GTMPWM_H_
#define _LQ_GTMPWM_H_

#include "IfxGtm_PinMap.h"
#include <Platform_Types.h>
#include <stdint.h>
#include <IfxGtm.h>
#include <IfxGtm_Atom_Pwm.h>
#include <IfxGtm_cfg.h>
#include <IfxGtm_Cmu.h>
#include <IfxGtm_PinMap.h>
#include <IfxGtm_reg.h>
#include <IfxGtm_Tim_In.h>
#include <IfxGtm_Tom.h>
#include <IfxGtm_Tom_Pwm.h>
#include <IfxPort.h>

// ATOM 时钟 100MHz
#define ATOM_PWM_CLK  100000000

// ATOM 最大占空比 可自行修改
#define ATOM_PWM_MAX  10000.0f

// TOM 时钟 6.25MHz/2
#define TOM_PWM_CLK  6250000

// TOM 最大占空比 可自行修改
#define TOM_PWM_MAX  10000.0f


////////////////ATOM_PWM//////////////////////
void ATOM_PWM_InitConfig(IfxGtm_Atom_ToutMap pin, unsigned long duty, unsigned long pwmFreq_Hz);
void ATOM_PWM_SetDuty(IfxGtm_Atom_ToutMap pin, unsigned long duty, unsigned long pwmFreq_Hz);



////////////////TOM_TIM//////////////////////
void TOM_PWM_InitConfig(IfxGtm_Tom_ToutMap pin, unsigned long duty, unsigned long pwmFreq_Hz);
void TOM_PWM_SetDuty(IfxGtm_Tom_ToutMap pin, unsigned long duty, unsigned long pwmFreq_Hz);


void TIM_InitConfig(IfxGtm_Tim_TinMap pin);
void TIM_GetPwm(IfxGtm_Tim_TinMap pin, float32 *Period, float32 *Duty);


#endif /* 0_APPSW_TRICORE_APP_LQ_GTM6PWM_H_ */
