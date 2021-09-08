/*
 * time.h
 *
 *  Created on: 2020Äê12ÔÂ12ÈÕ
 *      Author: 936305695
 */

#ifndef OS_TIME_OSTIME_H_
#define OS_TIME_OSTIME_H_

#include "device.h"

typedef struct
{
     uint32_t Ticks;
     uint32_t us;
     uint32_t ms;
     uint32_t s;
     uint32_t min;
     uint32_t hour;
}nowtime_t;

typedef struct
{
     void (*init)(void);
     nowtime_t     (*now)();
     uint32_t (*get_timeus)();
     uint32_t (*get_timems)();
     uint8_t (*delayms)(uint32_t ms);
     uint8_t (*delayus)(uint32_t us);
}ostime_t;

void OSTime_Init(void);
nowtime_t  OSTime_Now();
uint32_t OSTime_Get_Timeus();
uint32_t OSTime_Get_Timems();
uint8_t OSTime_Delayus(uint32_t us);
uint8_t OSTime_Delayms(uint32_t ms);

#endif /* OS_TIME_OSTIME_H_ */
