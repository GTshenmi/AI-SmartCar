/*
 * os.h
 *
 *  Created on: 2020Äê12ÔÂ7ÈÕ
 *      Author: 936305695
 */

#ifndef OS_OS_H_
#define OS_OS_H_

#include "driver.h"
#include "device.h"
#include "malloc.h"
#include "task.h"
#include "file.h"
#include "softtimer.h"
#include "ostime.h"

#include "debug.h"
#include "ui.h"

#define Version "V2.1.0"
#define Author  "GT_shenmi"
#define Date    "2021-08-06"

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
