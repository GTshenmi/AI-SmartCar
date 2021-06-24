/*
 * dsensor.h
 *
 *  Created on: 2021Äê6ÔÂ24ÈÕ
 *      Author: 936305695
 */

#ifndef OS_DEVICE_DSENSOR_DSENSOR_H_
#define OS_DEVICE_DSENSOR_DSENSOR_H_

#include "platform.h"

typedef struct dsensor
{
    private

        void *ADCn;
        uint16_t (*ReadADC)(struct dsensor *self);

    public

        uint16_t (*Init)(struct dsensor *self);
        uint16_t (*Read)(struct dsensor *self);

        float (*GetDistance)(struct dsensor *self);


        struct dsensor *Self;
}dsensor_t;



#endif /* OS_DEVICE_DSENSOR_DSENSOR_H_ */
