/*
 * include.h
 *
 *  Created on: 2020Äê12ÔÂ6ÈÕ
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
#include "rtthread.h"

extern IfxCpu_mutexLock mutexCpu0InitIsOk;
extern IFX_ALIGN(4) IfxCpu_syncEvent g_cpuSyncEvent;

#endif /* APP_INCLUDE_H_ */
