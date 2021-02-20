/*
 * dipswitch.h
 *
 *  Created on: 2021Äê1ÔÂ8ÈÕ
 *      Author: 936305695
 */

#ifndef OS_DEVICE_SWITCH_DIPSWITCH_H_
#define OS_DEVICE_SWITCH_DIPSWITCH_H_

#include "driver.h"
#include "switch.h"

typedef struct dip_switch
{
    /*1:Switch ON 0:Switch OFF,NOT EQUAL TO IO LEVEL ! for example : 0b1001 [3:0] Switch_ON,Switch_OFF,Switch_OFF,Switch_ON*/
    void (*Init)(struct dip_switch *);
    uint32_t (*Read)(struct dip_switch *);
    switch_t *Switch;
    uint8_t SwitchNum;
    uint32_t Bits;
    struct dip_switch *Self;
}dip_switch_t;

void DIPSwitch_Init(struct dip_switch *self);

#endif /* OS_DEVICE_SWITCH_DIPSWITCH_H_ */
