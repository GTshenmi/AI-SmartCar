/*
 * switch.h
 *
 *  Created on: 2021年1月8日
 *      Author: 936305695
 */

#ifndef OS_DEVICE_SWITCH_SWITCH_H_
#define OS_DEVICE_SWITCH_SWITCH_H_

#include "platform.h"

typedef enum
{
    Switch_OFF,Switch_ON,Switch_NULL
}switch_state_t;

typedef struct switchx
{
    private

        switch_state_t State;                    //按键状态
        void *GPIOn;
        uint8_t Level;                           //虚拟当前IO电平，按下1，抬起0
        uint8_t DownLevel;                       //按下时IO实际的电平
        uint8_t (*ReadIOLevel)(struct switchx *self);

    public

        void (*Init)(struct switchx *self);
        switch_state_t (*Read)(struct switchx *self);
        void (*SetDownLevel)(struct switchx *self,uint8_t DownLevel);

        struct switchx *Self;

}switch_t;

void Switch_Init(struct switchx *self);

#endif /* OS_DEVICE_SWITCH_SWITCH_H_ */
