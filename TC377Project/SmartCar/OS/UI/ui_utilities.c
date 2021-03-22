/*
 * template_page.c
 *
 *  Created on: 2021Äê3ÔÂ8ÈÕ
 *      Author:
 */

#include "ui_utilities.h"
#if defined(Chip) && (Chip == TC264 || Chip == TC377)
IfxCpu_mutexLock UIMutexLock;
#endif

void ClearScreen(void){

#if defined(Chip) && Chip == TC377 || Chip == TC264
    while(!UIAcquireMutex(&UIMutexLock));

#endif

    Screen.Fill(Screen.Self, 0, 0, Screen.Width, Screen.Font.Hight * 17 - 1,
            WHITE);

#if defined(Chip) && Chip == TC377 || Chip == TC264
    UIReleaseMutex(&UIMutexLock);
#endif

}
