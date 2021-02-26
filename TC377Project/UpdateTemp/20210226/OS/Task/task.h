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
        void (*KeyScan)(void *argv,uint16_t argc);/*ok*/
        void (*UiUpdate)(void *argv,uint16_t argc);
        void (*SoftTimerUpdate)(void *argv,uint16_t argc);/*ok*/
        void (*DebugConsole)(void *argv,uint16_t argc);
}task_t;

void Task_KeyScan(void *argv,uint16_t argc);
void Task_UiUpdate(void *argv,uint16_t argc);
void Task_DebugConsole(void *argv,uint16_t argc);
void Task_SoftTimerUpdate(void *argv,uint16_t argc);

#endif /* OS_TASK_TASK_H_ */
