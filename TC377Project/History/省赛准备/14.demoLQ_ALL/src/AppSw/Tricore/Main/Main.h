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
#ifndef SRC_APPSW_TRICORE_MAIN_MAIN_H_
#define SRC_APPSW_TRICORE_MAIN_MAIN_H_

#include <stdint.h>

extern IfxCpu_mutexLock mutexCpu0InitIsOk;   /** CPU0 初始化完成标志位  */

extern IfxCpu_mutexLock mutexTFTIsOk;        /** TFT18使用标志位  */
/**
 * 定时器 5ms和50ms标志位
 */
extern volatile unsigned char cpu1Flage5ms;
extern volatile unsigned char cpu1Flage50ms;

/* 期望速度  */
extern volatile signed short targetSpeed;
/* 避障标志位 */
extern volatile unsigned char evadibleFlage;

int core0_main (void);

#endif /* SRC_APPSW_TRICORE_MAIN_MAIN_H_ */
