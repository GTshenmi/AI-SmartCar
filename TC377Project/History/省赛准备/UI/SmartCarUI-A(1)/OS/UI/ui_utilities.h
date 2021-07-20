/*
 * ui_utilities.h
 *
 *  Created on: 2021Äê3ÔÂ8ÈÕ
 *      Author:
 */

#ifndef OS_UI_UI_UTILITIES_H_
#define OS_UI_UI_UTILITIES_H_

#include "device.h"
#include "IfxCpu.h"

#if defined(Chip) && (Chip == TC264 || Chip == TC377)
#define UIAcquireMutex(lock) IfxCpu_acquireMutex(lock)
#define UIReleaseMutex(lock) IfxCpu_releaseMutex(lock)
extern IfxCpu_mutexLock UIMutexLock;
#endif

void ClearScreen(void);

#endif
