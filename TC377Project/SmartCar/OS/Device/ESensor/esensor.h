/*
 * esensor_driver.h
 *
 *  Created on: 2020Äê12ÔÂ6ÈÕ
 *      Author: 936305695
 */

#ifndef OS_DEVICE_ESENSOR_ESENSOR_H_
#define OS_DEVICE_ESENSOR_ESENSOR_H_

#include "driver.h"

typedef struct esensor
{
        adcx_t *ADCDevice;
        uint16_t FilterBuf[10];
        uint16_t FilterBufLen;

        uint16_t (*Init)(struct esensor *self);
        uint16_t (*Read)(struct esensor *self);
        uint16_t (*Filter)(struct esensor *self);/*Sensor Filter Function Can Re-Definition.*/
        uint16_t Cache;

        bool Use_Filter;

        void (*Test)(struct esensor *self);
        struct esensor *Self;
}esensor_t;

uint16_t ESensorInit(struct esensor *self);

#endif /* OS_DEVICE_ESENSOR_ESENSOR_H_ */
