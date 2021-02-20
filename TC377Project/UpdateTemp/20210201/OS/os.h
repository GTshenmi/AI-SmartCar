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
#include "ui.h"
#include "file.h"
#include "softtimer.h"
#include "ostime.h"
#include "print.h"
#include "console.h"

typedef struct
{
     void (*init)(void);
     ostime_t time;
     ssofttimer_m softtimer;
     task_t    task;
     path_t    path;
     file_t    file;

}os_t;

extern os_t os;

#endif /* OS_OS_H_ */
