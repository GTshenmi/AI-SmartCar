/*
 * os.h
 *
 *  Created on: 2020Äê12ÔÂ7ÈÕ
 *      Author: 936305695
 */

#ifndef OS_OS_H_
#define OS_OS_H_

#include "device.h"
#include "malloc.h"
#include "task.h"
#include "file.h"
#include "softtimer.h"
#include "ostime.h"

#include "print.h"
#include "console.h"
#include "ui.h"

typedef struct
{
        void (*Init)(void);
        void (*HEnvInit)(void);
        void (*HardWareInit)(void);
        void (*SoftWareInit)(void);/**/
        void (*WaitForSync)(void *data);
        void (*Run)(void);
}core_t;

typedef struct
{
     void (*init)(uint core);
     ostime_t time;
     ssofttimer_m softtimer;
     task_t    task;
     path_t    path;
     file_t    file;
     core_t    core[3];
}os_t;

extern os_t os;

uint16_t GetRandNumberFromADC(void);

#endif /* OS_OS_H_ */
