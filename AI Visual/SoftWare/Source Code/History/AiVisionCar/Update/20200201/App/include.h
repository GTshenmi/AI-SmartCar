/*
 * include.h
 *
 *  Created on: 2020年12月6日
 *      Author: 936305695
 */

#ifndef APP_INCLUDE_H_
#define APP_INCLUDE_H_

#include "os.h"
#include "sys.h"
#include "usr.h"

#include "foo.h"
#include "init.h"
#include "app.h"

#if defined(Chip) && Chip == TC264
extern App_Cpu0 g_AppCpu0; /**< \brief CPU 0 global data */
extern IfxCpu_mutexLock mutexCpu0InitIsOk;   /** CPU0 初始化完成标志位  */
#endif

#if defined(Chip) && Chip == RT1064
void POWER_ENABLE(void);
/* 中断优先级组 */
#define NVIC_Group0   0x07
#define NVIC_Group1   0x06
#define NVIC_Group2   0x05
#define NVIC_Group3   0x04
#define NVIC_Group4   0x03
#endif

#endif /* APP_INCLUDE_H_ */
