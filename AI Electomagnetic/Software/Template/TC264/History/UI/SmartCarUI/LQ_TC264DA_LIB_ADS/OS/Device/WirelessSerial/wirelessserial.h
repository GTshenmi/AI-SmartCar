/*
 * wirelessserial.h
 *
 *  Created on: 2021Äê1ÔÂ19ÈÕ
 *      Author: 936305695
 */

#ifndef OS_DEVICE_WIRELESSSERIAL_WIRELESSSERIAL_H_
#define OS_DEVICE_WIRELESSSERIAL_WIRELESSSERIAL_H_

#include "driver.h"

typedef struct wireless_serial
{
    private

        void (*__Init__)(void *config);
        void *__InitConfig__;

    public

        void (*Init)(struct wireless_serial *self);

        struct wireless_serial *Self;

}wireless_serial_t;

void WirelessSerial_Init(struct wireless_serial *self);

#endif /* OS_DEVICE_WIRELESSSERIAL_WIRELESSSERIAL_H_ */
