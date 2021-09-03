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
        .CtrlInterruptRun = Ctrl_InterruptRun,

        .SensorInterruptRun = Sensor_InterruptRun,
};



