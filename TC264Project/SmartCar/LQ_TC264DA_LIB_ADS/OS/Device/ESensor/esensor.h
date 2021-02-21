/*
 * esensor.h
 *
 *  Created on: 2020Äê12ÔÂ6ÈÕ
 *      Author: 936305695
 */

#ifndef OS_DEVICE_ESENSOR_ESENSOR_H_
#define OS_DEVICE_ESENSOR_ESENSOR_H_

#include "driver.h"

#define ESENSOR_BITS_FILTER_ENABLE_MASK 0x00000001
#define ESENSOR_BITS_GAIN_ENABLE_MASK   0x00000002

#define ESENSOR_FAST_READ_LEVEL_0 0
#define ESENSOR_FAST_READ_LEVEL_1 1
#define ESENSOR_FAST_READ_LEVEL_2 2

#define ESENSOR_FAST_READ_LEVEL ESENSOR_FAST_READ_LEVEL_0

#if ESENSOR_FAST_READ_LEVEL == ESENSOR_FAST_READ_LEVEL_0

#elif ESENSOR_FAST_READ_LEVEL == ESENSOR_FAST_READ_LEVEL_1
#define ESensor_ReadADC(self) ADCx.Read(self->ADCDevice)
#elif ESENSOR_FAST_READ_LEVEL == ESENSOR_FAST_READ_LEVEL_2
#define ESensor_ReadADC(self) ADC_Read(self->ADCDevice->Channel)
#else
#error Please check the macro definition : ESENSOR_FAST_READ_LEVEL.
#endif

typedef struct esensor
{
    private

        adcx_t *ADCn;
        uint16_t FilterBuf[10];
        uint16_t FilterBufLen;

        float Gain;
        uint16_t Cache;
        /*
         * bit 0 : Enable Filter
         * bit 1 : Enable Gain
         */
        uint32_t ConfigReg;

        void *Argv;
        uint16_t Argc;

#if ESENSOR_FAST_READ_LEVEL == ESENSOR_FAST_READ_LEVEL_0
        uint16_t (*ReadADC)(struct esensor *self);
#endif

    public

        uint16_t (*Init)(struct esensor *self);
        uint16_t (*Read)(struct esensor *self);
        uint16_t (*ReadFromCache)(struct esensor *self);
        uint16_t (*Filter)(struct esensor *self,void *argv,uint16_t argc);/*Sensor Filter Function Can Re-Definition.*/

        void (*EnableFilter)(struct esensor *self,bool enable);
        void (*EnableGain)(struct esensor *self,bool enable);
        void (*SetGain)(struct esensor *self,float gain);

        void (*BindUsrData)(struct esensor *self,void *argv,uint16_t argc);

        void (*Test)(struct esensor *self);
        struct esensor *Self;
}esensor_t;

uint16_t ESensorInit(struct esensor *self);

#endif /* OS_DEVICE_ESENSOR_ESENSOR_H_ */
