/*
 * ui_utilities.h
 *
 *  Created on: 2021��3��8��
 *      Author:
 */

#ifndef OS_UI_UI_UTILITIES_H_
#define OS_UI_UI_UTILITIES_H_

#include "device.h"
#include "IfxCpu.h"
#include "template_page.h"

#if defined(Chip) && (Chip == TC264 || Chip == TC377)
//#define UIAcquireMutex(lock) IfxCpu_acquireMutex(lock)
//#define UIReleaseMutex(lock) IfxCpu_releaseMutex(lock)
extern Cpu_MutexLock UIMutexLock;
#endif

void ClearScreen(void);
uint8_t cursorSelected(UIPageStruct *Self);
void changeCursorLocation (void);
void UIPageDisplay (UIPageStruct *Self);

#endif
