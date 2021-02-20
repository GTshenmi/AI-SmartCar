/*
 * beep.h
 *
 *  Created on: 2020Äê12ÔÂ14ÈÕ
 *      Author: 936305695
 */

#ifndef OS_DEVICE_BEEP_BEEP_H_
#define OS_DEVICE_BEEP_BEEP_H_

#include "driver.h"

typedef struct beep
{
    void (*Init)(struct beep *self);
    void (*ON)(struct beep *self);
    void (*OFF)(struct beep *self);
    void (*Toggle)(struct beep *self);

    gpiox_t *GPIODevice;

    struct beep *Self;
}beep_t;

void BEEP_Init(struct beep *self);

#endif /* OS_DEVICE_BEEP_BEEP_H_ */
