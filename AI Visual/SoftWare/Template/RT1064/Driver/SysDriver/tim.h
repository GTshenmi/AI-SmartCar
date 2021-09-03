/*
 * timer.h
 *
 *  Created on: 2020Äê12ÔÂ12ÈÕ
 *      Author: 936305695
 */

#ifndef DRIVER_SYSDRIVER_TIM_H_
#define DRIVER_SYSDRIVER_TIM_H_

#include "stdbool.h"
#include "LQ_GPT.h"
#include "LQ_PIT.h"
#include "LQ_QTMR.h"
#include "platform.h"

typedef enum
{
     GPTx,PITx
}timer_source_t;

typedef struct
{
        void *Timer;
        
        uint32_t Channel;
        
        uint32_t interrupt_interval;
        
        bool Enable_Interrupt;
        bool Is_Run;
        timer_source_t timer_source;
}timx_t;

typedef struct
{
        uint8_t (*Init)(timx_t *);
        uint8_t (*Start)(timx_t *);
        uint8_t (*Delay)(timx_t *,uint32_t us);
        uint32_t (*GetTime)(timx_t *);
        uint8_t (*DeInit)(timx_t *);
        uint8_t (*Stop)(timx_t *);
}stim_m;

extern stim_m TIMx;



#endif /* DRIVER_SYSDRIVER_TIM_H_ */
