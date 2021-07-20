/*
 * os.c
 *
 *  Created on: 2020Äê12ÔÂ12ÈÕ
 *      Author: 936305695
 */
#include "os.h"

void OS_Init()
{
    os.time.init();
    os.softtimer.init();
    os.file.init();
//    printb(var.b);
//    printc(var.c);
//    printi(var.i);
//    printd(var.f);
//    prints(var.s);
//    printu(var.ui);
//
//    print(var.ui,'u');
}

os_t os =
{
        .init = OS_Init,
        .time = {
                .init = OSTime_Init,
                .now = OSTime_Now,
                .get_timeus = OSTime_Get_Timeus,
                .get_timems = OSTime_Get_Timems,
                .delayms = OSTime_Delayms,
                .delayus = OSTime_Delayus,
        },
        .softtimer = {
                .init = SoftTimerInit,

                .start = SoftTimerStart,
                .update = SoftTimerUpdate,
                .stop = SoftTimerStop,
                .getStates = SoftTimerGetStates,
                .findFreeTimer = SoftTimerFindFreeTimer,
        },
        .task = {
                .KeyScan = Task_KeyScan,
                .SoftTimerUpdate = Task_SoftTimerUpdate,
                .UiUpdate = Task_UiUpdate,
                .DebugConsole = Task_DebugConsole,
        },
        .path =
        {
                .exist = FileSys_PathIsExists,
        },
        .file =
        {
                .init = FileSys_Init,
        },
};



