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
        void (*Update)(void *argv,uint16_t argc);
}ui_t;

typedef struct
{
        sint16_t Speed;
        uint16_t LADC_Value[5];
}ui_data_pkg_t;

extern ui_t UI;
extern __attribute__((weak)) void Task_UiUpdate(void *argv,uint16_t argc);

#endif /* OS_UI_UI_H_ */
