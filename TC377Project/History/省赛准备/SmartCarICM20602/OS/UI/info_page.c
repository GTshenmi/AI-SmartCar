/*
 * info_page.c
 *
 *  Created on: 2021年3月15日
 *      Author:
 */

#include "info_page.h"
#include "template_page.h"

char* getStatus (char *String, uint *targetData)
{
    switch (*targetData)
    {
        case 0 :
            return "Stop";
        case 1 :
            return "Run";
        case 2 :
            return "Sleep";
    }
    return "ERR";
}

void displayAIInfo (UIPageStruct *Self, uint8_t dataLineTmp, uint16_t beginLineTmp)
{

    char stringBuffer[30];
    sprintf(stringBuffer, "AI:");
    switch (*UIData.AI_State)
    {
        case 0 :
            strcpy(&stringBuffer[strlen(stringBuffer)], "Free ");
            break;
        case 1 :
            strcpy(&stringBuffer[strlen(stringBuffer)], "Start");
            break;
        case 2 :
            strcpy(&stringBuffer[strlen(stringBuffer)], "Busy ");
            break;
    }
    Screen.ShowString(Screen.Self, 0, beginLineTmp * Screen.Font.Hight * 2, (uint8_t*) stringBuffer,
            cursorSelected(Self));

}

void displayCarInfo (UIPageStruct *Self, uint8_t dataLineTmp, uint16_t beginLineTmp)
{

    char stringBuffer[30];
    sprintf(stringBuffer, "CarState:%-4s ", (*UIData.CarState ? "Run" : "Stop"));
    Screen.ShowString(Screen.Self, 0, beginLineTmp * Screen.Font.Hight * 2, (uint8_t*) stringBuffer,
            cursorSelected(Self));

}

void displaySysInfo (UIPageStruct *Self, uint8_t dataLineTmp, uint16_t beginLineTmp) // 电机舵机状态
{

    char stringBuffer[30];
    sprintf(stringBuffer, "%s:%-5s ", Self->description, getStatus(stringBuffer, (uint*) Self->primaryTargetData));
    Screen.ShowString(Screen.Self, 0, beginLineTmp * Screen.Font.Hight * 2, (uint8_t*) stringBuffer,
            cursorSelected(Self));

}

void changeSysStatus (UIPageStruct *Self)
{
    if (*(uint*) Self->primaryTargetData == 2)
    {
        (*(uint*) Self->primaryTargetData) = 0;
    }
    else
    {
        (*(uint*) Self->primaryTargetData)++;
    }
}

void changeCarStatus (UIPageStruct *Self)
{
    *UIData.CarState = !*UIData.CarState;
}
