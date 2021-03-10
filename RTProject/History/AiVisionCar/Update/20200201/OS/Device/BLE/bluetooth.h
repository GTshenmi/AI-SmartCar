/*
 * bluetooth.h
 *
 *  Created on: 2021Äê1ÔÂ19ÈÕ
 *      Author: 936305695
 */

#ifndef OS_DEVICE_BLE_BLUETOOTH_H_
#define OS_DEVICE_BLE_BLUETOOTH_H_

#include "driver.h"

typedef struct bluetooth
{
     void (*Init)(struct bluetooth *self);
     void (*HardWareInit)(void);

     struct bluetooth *Self;


}bluetooth_t;

void Bluetooth_Init(struct bluetooth *self);

#endif /* OS_DEVICE_BLE_BLUETOOTH_H_ */
