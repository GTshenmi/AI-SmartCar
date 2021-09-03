/*
 * smartcarsys.h
 *
 *  Created on: 2021Äê1ÔÂ9ÈÕ
 *      Author: 936305695
 */

#ifndef APP_SYS_SMARTCARSYS_H_
#define APP_SYS_SMARTCARSYS_H_

#include "os.h"

#define SCore0 SmartCarSys.Core0
#define SCore1 SmartCarSys.Core1
#define SCore2 SmartCarSys.Core2

typedef struct
{
       void (*CtrlInterruptRun)(void);
       void (*SensorInterruptRun)(void);
}smartcarsys_t;

extern smartcarsys_t SmartCarSys;

#endif /* APP_SYS_SMARTCARSYS_H_ */
