/*
 * systime.h
 *
 *  Created on: 2020Äê12ÔÂ26ÈÕ
 *      Author: 936305695
 */

#ifndef DRIVER_SYSDRIVER_SYSTIME_H_
#define DRIVER_SYSDRIVER_SYSTIME_H_

#include "common.h"
#include "tim.h"
#include "platform.h"

typedef struct
{
     void (*Init)(void);
     uint32_t (*Get_Timeus)(void);
     uint32_t (*Get_Timems)(void);
     uint8_t (*Delayms)(uint32_t ms);
     uint8_t (*Delayus)(uint32_t us);
     STM_t STM;
     STM_Channel_t Channel;
     void (*InterruptCallBack)(void);
}systimex_t;

extern systimex_t Systime;

#endif /* DRIVER_SYSDRIVER_SYSTIME_H_ */
