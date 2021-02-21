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
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ
*  备    注：TC377 有两个CCU6模块  每个模块有两个独立定时器  触发定时器中断
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#ifndef SRC_APPSW_TRICORE_DRIVER_LQ_CCU6_H_
#define SRC_APPSW_TRICORE_DRIVER_LQ_CCU6_H_

#include "IfxPort.h"
#include "IfxCcu6_Timer.h"
#include "Cpu/Std/Platform_Types.h"
#include "stdint.h"
#include "Bsp.h"
#include "IfxCcu6.h"
#include "IfxCcu6_cfg.h"
#include "IfxCcu6_reg.h"
#include "IfxCcu6_regdef.h"
#include "IfxCcu6_Timer.h"
#include "IfxCpu.h"
#include "IfxCpu_Irq.h"
#include "IfxScuCcu.h"
#include "interrupt.h"

extern volatile sint16 ECPULSE1;          //速度全局变量
extern volatile sint16 ECPULSE2;          //速度全局变量
extern volatile sint32 RAllPulse;         //速度全局变量

/**
 * 	CCU6模块枚举
 */
typedef enum
{
	CCU60,
	CCU61
}CCU6_t;

/**
 * 	CCU6通道枚举
 */
typedef enum
{
	CCU6_Channel0,
	CCU6_Channel1,
}CCU6_Channel_t;

/*************************************************************************
*  函数名称：CCU6_InitConfig CCU6
*  功能说明：定时器周期中断初始化
*  参数说明：ccu6    ： ccu6模块            CCU60 、 CCU61
*  参数说明：channel ： ccu6模块通道  CCU6_Channel0 、 CCU6_Channel1
*  参数说明：us      ： ccu6模块  中断周期时间  单位us
*  函数返回：无
*  修改时间：2020年3月30日
*  备    注：
*************************************************************************/
void CCU6_InitConfig(CCU6_t ccu6, CCU6_Channel_t channel, unsigned long us);

void CCU6_DisableInterrupt(CCU6_t ccu6, CCU6_Channel_t channel);

void CCU6_EnableInterrupt(CCU6_t ccu6, CCU6_Channel_t channel);

#endif /* SRC_APPSW_TRICORE_DRIVER_LQ_CCU6_H_ */
