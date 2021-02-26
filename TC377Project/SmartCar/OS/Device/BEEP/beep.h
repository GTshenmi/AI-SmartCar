/*
 * beep.h
 *
 *  Created on: 2020��12��14��
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
    void (*Toggle)(struct beep *self); /*״̬��ת*/
    struct beep *Self;
}beep_t;

void BEEP_Init(struct beep *self);

#endif /* OS_DEVICE_BEEP_BEEP_H_ */
