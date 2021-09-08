/*
 * henv.h
 *
 *  Created on: 2021Äê1ÔÂ31ÈÕ
 *      Author: 936305695
 */

#ifndef DRIVER_SYSDRIVER_HENV_H_
#define DRIVER_SYSDRIVER_HENV_H_

#include "sys_driverlq.h"

extern App_Cpu0 g_AppCpu0;
void Core0_HardWareEnvInit(void);
void Core1_HardWareEnvInit(void);
void Core2_HardWareEnvInit(void);
void Core0_WaitForSync(void *data);
void Core1_WaitForSync(void *data);
void Core2_WaitForSync(void *data);

#endif /* DRIVER_SYSDRIVER_HENV_H_ */
