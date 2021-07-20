/*
 * info_page.h
 *
 *  Created on: 2021年3月15日
 *      Author:
 */

#ifndef OS_UI_INFO_PAGE_H_
#define OS_UI_INFO_PAGE_H_

#include "ui.h"
#include "template_page.h"
#include "ui_utilities.h"

void displayAIInfo(UIPageStruct *Self, uint8_t dataLineTmp, uint16_t beginLineTmp);
void displayCarInfo(UIPageStruct *Self, uint8_t dataLineTmp, uint16_t beginLineTmp);
void displaySysInfo(UIPageStruct *Self, uint8_t dataLineTmp, uint16_t beginLineTmp); // 电机舵机状态

void changeSysStatus(UIPageStruct *Self);
void changeCarStatus(UIPageStruct *Self);

#endif
