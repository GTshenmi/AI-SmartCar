/*
 * bluetooth.h
 *
 *  Created on: 2021Äê1ÔÂ19ÈÕ
 *      Author: 936305695
 */

#ifndef OS_DEVICE_BLE_BLUETOOTH_H_
#define OS_DEVICE_BLE_BLUETOOTH_H_

#include "platform.h"

typedef struct bluetooth
{
    private

     void (*__Init__)(void);

    public

     void (*Init)(struct bluetooth *self);

     struct bluetooth *Self;


}bluetooth_t;

void Bluetooth_Init(struct bluetooth *self);

#endif /* OS_DEVICE_BLE_BLUETOOTH_H_ */
