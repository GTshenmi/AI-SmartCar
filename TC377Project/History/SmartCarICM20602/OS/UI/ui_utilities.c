/*
 * template_page.c
 *
 *  Created on: 2021��3��8��
 *      Author:
 */

#include "ui_utilities.h"

#if defined(Chip) && (Chip == TC264 || Chip == TC377)
Cpu_MutexLock UIMutexLock;
#endif

void ClearScreen(void){

#if defined(Chip) && Chip == TC377 || Chip == TC264
    while(!Cpu_AcquireMutex(&UIMutexLock));
#endif

    Screen.Fill(Screen.Self, 0, 0, Screen.Width, Screen.Font.Hight * 17 - 1,
            WHITE);

#if defined(Chip) && Chip == TC377 || Chip == TC264
    Cpu_ReleaseMutex(&UIMutexLock);
#endif

}

uint8_t cursorSelected (UIPageStruct *Self)
{
    if (Self->cursorSelected)
    {
        return BLUE;
    }
    else
    {
        return BLACK;
    }
}

void UIPageDisplay (UIPageStruct *Self)
{

    uint8_t dataLineTmp = 0;
    int16_t beginLineTmp = Self->beginLine;

    while (1)
    {
        if (beginLineTmp < SCREEN_MIN_LINE)
        {
            beginLineTmp++;
            dataLineTmp++;
            continue;
        }
        if (beginLineTmp >= SCREEN_MAX_LINE || dataLineTmp >= Self->lineLength)
        {
            break;
        }

        Self->displayData(Self,dataLineTmp,beginLineTmp);

        beginLineTmp++;
        dataLineTmp++;
    }
}

void changeCursorLocation(void){
    uint8_t i = 0;
    while(!UIPages[i].cursorSelected){
        i++;
    }
    if(UIPages[i].beginLine <= 0 || UIPages[i].beginLine >= SCREEN_MAX_LINE){
        UIPages[i].cursorSelected = 0;
        i = 0;
        while(UIPages[i].beginLine < 0){
            i++;
        }
        UIPages[i].cursorSelected = 1;
    }
}

