/*
 * beep.h
 *
 *  Created on: 2020年12月14日
 *      Author: 936305695
 */

#ifndef OS_DEVICE_BEEP_BEEP_H_
#define OS_DEVICE_BEEP_BEEP_H_

#include "platform.h"

typedef struct beep
{
    private

    void *GPIOn;
    uint8_t ONLevel;

    public

    void (*Init)(struct beep *self);
    void (*ON)(struct beep *self);
    void (*OFF)(struct beep *self);
    void (*Toggle)(struct beep *self); /*状态翻转*/
    struct beep *Self;
}beep_t;

void BEEP_Init(struct beep *self);

#endif /* OS_DEVICE_BEEP_BEEP_H_ */
