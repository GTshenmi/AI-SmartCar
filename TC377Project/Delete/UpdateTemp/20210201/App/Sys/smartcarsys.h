/*
 * smartcarsys.h
 *
 *  Created on: 2021Äê1ÔÂ9ÈÕ
 *      Author: 936305695
 */

#ifndef APP_SYS_SMARTCARSYS_H_
#define APP_SYS_SMARTCARSYS_H_

#include "os.h"

typedef struct
{
        void (*Init)(void);
        void (*BasicInit)(void); /**/
        void (*HardWareInit)(void);
        void (*SoftWareInit)(void);/**/
        void (*Run)(void);
#if defined(Chip) && (Chip == TC264 || Chip == TC377)
        IfxCpu_mutexLock mutexCoreInitIsOk;
#endif
}core_t;

typedef struct
{
       core_t Core0;
       core_t Core1;

#if defined(Chip) && (Chip == TC377)
       core_t Core2;
#endif

       void (*CtrlInterruptRun)(void);
       void (*SensorInterruptRun)(void);
}smartcarsys_t;

extern smartcarsys_t SmartCarSys;

#endif /* APP_SYS_SMARTCARSYS_H_ */
