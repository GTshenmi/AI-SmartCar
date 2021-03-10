/*
 * os.c
 *
 *  Created on: 2020Äê12ÔÂ12ÈÕ
 *      Author: 936305695
 */
#include "os.h"



void OS_Init(uint core)
{
    os.core[core].HEnvInit();
    switch(core)
    {
        case 0:
            os.time.init();
            os.softtimer.init();
            os.file.init();
            break;
        case 1:
            break;
        case 2:
            break;
        default:
            break;
    }
    os.core[core].HardWareInit();
    os.core[core].SoftWareInit();
    os.core[core].WaitForSync(NULL);
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
        .core =
        {
                [0] = {
                          .HEnvInit = Core0_HardWareEnvInit,
                          .WaitForSync = Core0_WaitForSync,
                },
                [1] = {
                          .HEnvInit = Core1_HardWareEnvInit,
                          .WaitForSync = Core1_WaitForSync,
                },
                [2] = {
                          .HEnvInit = Core2_HardWareEnvInit,
                          .WaitForSync = Core2_WaitForSync,
                },
        },
};



