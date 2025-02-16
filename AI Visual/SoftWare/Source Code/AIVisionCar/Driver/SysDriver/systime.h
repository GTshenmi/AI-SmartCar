/*
 * systime.h
 *
 *  Created on: 2020��12��26��
 *      Author: 936305695
 */

#ifndef DRIVER_SYSDRIVER_SYSTIME_H_
#define DRIVER_SYSDRIVER_SYSTIME_H_

#include "LQ_SYSTICK.h"
#include "stdlib.h"
#include "platform.h"

typedef struct
{
     uint8_t (*Init)(void);
     uint32_t (*Get_Timeus)(void);
     uint32_t (*Get_Timems)(void);
     uint8_t (*Delayms)(uint32_t ms);
     uint8_t (*Delayus)(uint32_t us);

     void (*InterruptCallBack)(void);
}systimex_t;

extern systimex_t Systime;

#endif /* DRIVER_SYSDRIVER_SYSTIME_H_ */
