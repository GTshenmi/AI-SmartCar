/*
 * task.h
 *
 *  Created on: 2020Äê12ÔÂ7ÈÕ
 *      Author: 936305695
 */

#ifndef OS_TASK_TASK_H_
#define OS_TASK_TASK_H_

#include "device.h"

typedef struct
{
        void (*KeyScan)(void);
        void (*UiUpdate)(void);
        void (*SoftTimerUpdate)(void);
        void (*DebugConsole)(void);
}task_t;

void Task_KeyScan(void);

#endif /* OS_TASK_TASK_H_ */
