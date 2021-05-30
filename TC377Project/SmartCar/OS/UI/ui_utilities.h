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
#include "template_page.h"

void ClearScreen (void);
void ExecuteClearScreen (void);
uint8_t cursorSelected (UIPageStruct *Self);
void changeCursorLocation (void);
void UIPageDisplay (UIPageStruct *Self);

#endif
