/*
 * time.h
 *
 *  Created on: 2020��12��12��
 *      Author: 936305695
 */

#ifndef OS_TIME_OSTIME_H_
#define OS_TIME_OSTIME_H_

#include "device.h"

enum
{
    ms,us,s
};

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
     nowtime_t     (*now)(void);

     uint32_t (*getTimeus)(void);
     uint32_t (*getTimems)(void);
     uint32_t (*getTimes)(void);/*��ȡʱ��(����Ϊ��λ)*/

     uint8_t (*delayms)(uint32_t ms);
     uint8_t (*delayus)(uint32_t us);
     uint8_t (*delays)(uint32_t s);/*��ʱ(����Ϊ��λ)*/


     float (*getTime)(uint16_t unit);
     float (*delay)(float time,uint16_t unit);/*��ʱʱ�䲻Ҫ������Сʱ*/
}ostime_t;

void OSTime_Init(void);

nowtime_t  OSTime_Now(void);

uint32_t OSTime_Get_Timeus(void);
uint32_t OSTime_Get_Timems(void);
uint32_t OSTime_Get_Times(void);

uint8_t OSTime_Delayus(uint32_t us);
uint8_t OSTime_Delayms(uint32_t ms);
uint8_t OSTime_Delays(uint32_t s);

float OSTime_Get_Time(uint16_t unit);
float OSTime_Delay(float time,uint16_t unit);

#endif /* OS_TIME_OSTIME_H_ */
