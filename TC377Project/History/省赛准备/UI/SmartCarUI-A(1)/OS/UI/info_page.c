/*
 * info_page.c
 *
 *  Created on: 2021Äê3ÔÂ15ÈÕ
 *      Author:
 */

#include "info_page.h"
#include "template_page.h"

char* getStatus (char *String, uint *targetData)
{
    switch (*targetData)
    {
        case 0 :
            return "Stp";
        case 1 :
            return "Run";
        case 2 :
            return "Slp";
    }
    return "ERR";
}

void InfoPageDisplay (InfoPageStruct *Self)
{
    Screen.DrawHline(Screen.Self,0,Screen.Font.Hight * 17 - 1,Screen.Width,1,BLACK);
    char stringBuffer[20];
    char statusBuffer[5];

    sprintf(stringBuffer, "CarState:%-4s ", (*UIData.CarState ? "Run" : "Stop"));
    Screen.ShowString(Screen.Self,0,Screen.Font.Hight * 17,(uint8_t*)stringBuffer,(Self->SelectedSection == CARSTATE ? RED : BLACK));

    sprintf(stringBuffer,"AI:");
    switch (*UIData.AI_State)
    {
        case 0 :
            strcpy(&stringBuffer[strlen(stringBuffer)], "Free");
            break;
        case 1 :
            strcpy(&stringBuffer[strlen(stringBuffer)], "Strt");
            break;
        case 2 :
            strcpy(&stringBuffer[strlen(stringBuffer)], "Busy");
            break;
    }
    Screen.ShowString(Screen.Self,14*Screen.Font.Width,Screen.Font.Hight * 17,(uint8_t*)stringBuffer,BLACK);

    Screen.ShowString(Screen.Self,0*Screen.Font.Width,Screen.Font.Hight * 18,(uint8_t*)"Motor ",BLACK);

    sprintf(stringBuffer, "1:%s", getStatus(statusBuffer,UIData.MotorSysState[0]));
    Screen.ShowString(Screen.Self,6*Screen.Font.Width,Screen.Font.Hight * 18,(uint8_t*)stringBuffer,(Self->SelectedSection == MOTOR1 ? RED : BLACK));

    sprintf(stringBuffer, "2:%s", getStatus(statusBuffer,UIData.MotorSysState[1]));
    Screen.ShowString(Screen.Self,11*Screen.Font.Width,Screen.Font.Hight * 18,(uint8_t*)stringBuffer,(Self->SelectedSection == MOTOR2 ? RED : BLACK));

    sprintf(stringBuffer, "3:%s", getStatus(statusBuffer,UIData.MotorSysState[2]));
    Screen.ShowString(Screen.Self,16*Screen.Font.Width,Screen.Font.Hight * 18,(uint8_t*)stringBuffer,(Self->SelectedSection == MOTOR3 ? RED : BLACK));

    Screen.ShowString(Screen.Self,0*Screen.Font.Width,Screen.Font.Hight * 19,(uint8_t*)"Motor ",BLACK);

    sprintf(stringBuffer, "1:%s", getStatus(statusBuffer,UIData.ServoSysState[0]));
    Screen.ShowString(Screen.Self,6*Screen.Font.Width,Screen.Font.Hight * 19,(uint8_t*)stringBuffer,(Self->SelectedSection == SERVO1 ? RED : BLACK));

    sprintf(stringBuffer, "2:%s", getStatus(statusBuffer,UIData.ServoSysState[1]));
    Screen.ShowString(Screen.Self,11*Screen.Font.Width,Screen.Font.Hight * 19,(uint8_t*)stringBuffer,(Self->SelectedSection == SERVO2 ? RED : BLACK));

    sprintf(stringBuffer, "3:%s", getStatus(statusBuffer,UIData.ServoSysState[2]));
    Screen.ShowString(Screen.Self,16*Screen.Font.Width,Screen.Font.Hight * 19,(uint8_t*)stringBuffer,(Self->SelectedSection == SERVO3 ? RED : BLACK));

}

void InfoPageAddOrMinus (InfoPageStruct *Self)
{
    switch (Self->SelectedSection)
    {
        case CARSTATE :
            *UIData.CarState = !*UIData.CarState;
            break;
        case MOTOR1 :
            if (*UIData.MotorSysState[0] == 2)
            {
                *UIData.MotorSysState[0] = 0;
            }
            else
            {
                (*UIData.MotorSysState[0])++;
            }
            break;
        case MOTOR2 :
            if (*UIData.MotorSysState[1] == 2)
            {
                *UIData.MotorSysState[1] = 0;
            }
            else
            {
                (*UIData.MotorSysState[1])++;
            }
            break;
        case MOTOR3 :
            if (*UIData.MotorSysState[2] == 2)
            {
                *UIData.MotorSysState[2] = 0;
            }
            else
            {
                (*UIData.MotorSysState[2])++;
            }
            break;
        case SERVO1 :
            if (*UIData.ServoSysState[0] == 2)
            {
                *UIData.ServoSysState[0] = 0;
            }
            else
            {
                (*UIData.ServoSysState[0])++;
            }
            break;
        case SERVO2 :
            if (*UIData.ServoSysState[1] == 2)
            {
                *UIData.ServoSysState[1] = 0;
            }
            else
            {
                (*UIData.ServoSysState[1])++;
            }
            break;
        case SERVO3 :
            if (*UIData.ServoSysState[2] == 2)
            {
                *UIData.ServoSysState[2] = 0;
            }
            else
            {
                (*UIData.ServoSysState[2])++;
            }
            break;
        default:
            break;
    }

    Self->Display(Self);
}

void InfoPageConfirm (InfoPageStruct *Self)
{
    if (Self->Selected)
    {
        if (Self->SelectedSection == SERVO3)
        {
            Self->Selected = 0;
            Self->SelectedSection = NOSELECTED;
            Self->UIPageStructCursor->cursorSelected = 1;
        }
        else
        {
            Self->SelectedSection++;
        }
    }
    Self->Display(Self);
}

void InfoPageCancel (InfoPageStruct *Self)
{
    if (Self->Selected)
    {
        if (Self->SelectedSection == CARSTATE)
        {
            Self->Selected = 0;
            Self->SelectedSection = NOSELECTED;
            Self->UIPageStructCursor->cursorSelected = 1;
        }
        else
        {
            Self->SelectedSection--;
        }
    }
    else
    {
        uint8_t i = 0;
        while (!UIPages[i].cursorSelected)
        {
            i++;
        }
        Self->UIPageStructCursor = &UIPages[i];
        Self->UIPageStructCursor->cursorSelected = 0;
        Self->Selected = 1;
        Self->SelectedSection = CARSTATE;
    }
    Self->Display(Self);
}

uint8_t InfoPageGetStatus (InfoPageStruct *Self)
{
    return Self->Selected;
}

InfoPageStruct InfoPage = {
    .Selected = 0,
    .SelectedSection = 0,

    .Display = InfoPageDisplay,
    .GetStatus = InfoPageGetStatus,

    .AddOrMinus = InfoPageAddOrMinus,
    .Confirm = InfoPageConfirm,
    .Cancel = InfoPageCancel,

    .Self = &InfoPage,
};
