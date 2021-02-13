/*
 * ui.h
 *
 *  Created on: 2020Äê12ÔÂ7ÈÕ
 *      Author: 936305695
 */

#ifndef OS_UI_UI_H_
#define OS_UI_UI_H_

#include "device.h"

typedef struct
{
        void (*Update)(void);
}ui_t;


extern ui_t UI;
extern __attribute__((weak)) void Task_UiUpdate();

#endif /* OS_UI_UI_H_ */
