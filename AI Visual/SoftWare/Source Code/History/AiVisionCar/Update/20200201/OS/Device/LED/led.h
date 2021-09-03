/*
 * led.h
 *
 *  Created on: 2020��12��8��
 *      Author: 936305695
 */

#ifndef OS_DEVICE_LED_LED_H_
#define OS_DEVICE_LED_LED_H_

#include "driver.h"

typedef struct led
{
    private

        gpiox_t *GPIODevice;

    public

        void (*Init)(struct led *self);
        void (*ON)(struct led *self);
        void (*OFF)(struct led *self);
        void (*Toggle)(struct led *self); /*״̬��ת*/

        void (*Test)(struct led *self);
        struct led *Self;
}led_t;

void LED_Init(struct led *self);

#endif /* OS_DEVICE_LED_LED_H_ */
