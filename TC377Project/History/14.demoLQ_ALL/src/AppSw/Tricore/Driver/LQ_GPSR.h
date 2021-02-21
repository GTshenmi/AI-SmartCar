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
  */ 
#ifndef SRC_APPSW_TRICORE_DRIVER_LQ_GPSR_H_
#define SRC_APPSW_TRICORE_DRIVER_LQ_GPSR_H_

#include "Tricore/_Impl/IfxSrc_cfg.h"
#include "interrupt.h"
/***********************************************************************************************/
/***********************************************************************************************/
/*****************    CPU中断优先级 和 指向内核配置 用户可以自行修改*******************************/
/***********************************************************************************************/
/***********************************************************************************************/

/**
 * 软件中的序号枚举
 */
typedef enum
{
	SoftIRQ0,
	SoftIRQ1,
	SoftIRQ2,
	SoftIRQ3
}SOFT_IRQ;

void GPSR_InitConfig(IfxSrc_Tos cpu, SOFT_IRQ index);
void CPSR_Trig(IfxSrc_Tos cpu, SOFT_IRQ index);

#endif /* SRC_APPSW_TRICORE_DRIVER_LQ_GPSR_H_ */
