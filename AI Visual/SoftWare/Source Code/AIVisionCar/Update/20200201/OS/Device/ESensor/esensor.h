/*
 * esensor_driver.h
 *
 *  Created on: 2020Äê12ÔÂ6ÈÕ
 *      Author: 936305695
 */

#ifndef OS_DEVICE_ESENSOR_ESENSOR_H_
#define OS_DEVICE_ESENSOR_ESENSOR_H_

#include "driver.h"

#define ESENSOR_BITS_FILTER_ENABLE_MASK 0x00000001
#define ESENSOR_BITS_GAIN_ENABLE_MASK   0x00000002

typedef struct esensor
{
    private

        adcx_t *ADCDevice;
        uint16_t FilterBuf[10];
        uint16_t FilterBufLen;

        float Gain;
        uint16_t Cache;
        /*
         * bit 0 : Enable Filter
         * bit 1 : Enable Gain
         */
        uint32_t ConfigReg;

        void *UsrData;

    public

        uint16_t (*Init)(struct esensor *self);
        uint16_t (*Read)(struct esensor *self);
        uint16_t (*ReadFromCache)(struct esensor *self);
        uint16_t (*Filter)(struct esensor *self,void *usrdata);/*Sensor Filter Function Can Re-Definition.*/

        void (*EnableFilter)(struct esensor *self,bool enable);
        void (*EnableGain)(struct esensor *self,bool enable);
        void (*SetGain)(struct esensor *self,float gain);

        void (*BindUsrData)(struct esensor *self,void *usrdata);

        void (*Test)(struct esensor *self);
        struct esensor *Self;
}esensor_t;

uint16_t ESensorInit(struct esensor *self);

#endif /* OS_DEVICE_ESENSOR_ESENSOR_H_ */
