/*
 * ostimer.h
 *
 *  Created on: 2020Äê12ÔÂ13ÈÕ
 *      Author: 936305695
 */

#ifndef OS_DEVICE_TIMER_SOFTTIMER_H_
#define OS_DEVICE_TIMER_SOFTTIMER_H_

#include "device.h"

#define SoftTimer_Infinity -1

typedef enum
{
    SoftTimer_Stopped,SoftTimer_Running,SoftTimer_TimeOut,
}stmrstate_t;

typedef enum
{
    SoftTimer_Mode_OneShot,SoftTimer_Mode_Periodic,
    SoftTimer_Mode_XTimes,
}stmrmode_t;

typedef struct softtimer
{
    stmrstate_t State;
    stmrmode_t Mode;
    uint32_t Match;
    uint32_t Period;
    uint32_t Duration;
    void (*CallBack)(void *argv,uint16_t argc);

    void *argv;
    uint16_t argc;
}softtimer_t;

typedef struct  //based on hardware systimer
{
    void (*init)(void);
    void (*start)(uint16_t id,stmrmode_t mode,uint32_t delay,uint32_t duration,void (*callback)(void *argv,uint16_t argc),void *argv,uint16_t argc);
    uint8_t (*update)(uint16_t id);
    void (*stop)(uint16_t id);
    uint8_t (*getStates)(uint16_t id);
    uint16_t (*findFreeTimer)(void);

}ssofttimer_m;

void SoftTimerInit();
void SoftTimerStart(uint16_t id,stmrmode_t mode,uint32_t delay,uint32_t duration,void (*callback)(void *argv,uint16_t argc),void *argv,uint16_t argc);
void SoftTimerStop(uint16_t id);
uint8_t SoftTimerGetStates(uint16_t id);
uint8_t SoftTimerUpdate(uint16_t id);
uint16_t SoftTimerFindFreeTimer();
void Task_SoftTimerUpdate(void *argv,uint16_t argc);

#endif /* OS_DEVICE_TIMER_SOFTTIMER_H_ */
