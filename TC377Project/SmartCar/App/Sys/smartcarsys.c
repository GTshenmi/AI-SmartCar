/*
 * smartcarsys.c
 *
 *  Created on: 2021Äê1ÔÂ9ÈÕ
 *      Author: 936305695
 */
#include <smartcarsys.h>

#include "include.h"

smartcarsys_t SmartCarSys =
{
        .Core0.Init = Core0_Init,

        .Core1.Init = Core1_Init,

        .CtrlInterruptRun = Ctrl_InterruptRun,

        .SensorInterruptRun = Sensor_InterruptRun,

        .Core0.Run = Core0_Main,
        .Core1.Run = Core1_Main,

#if defined(Chip) && (Chip == TC264 || Chip == TC377)
        .Core0.mutexCoreInitIsOk = 1,
        .Core1.mutexCoreInitIsOk = 1,
#endif

};



