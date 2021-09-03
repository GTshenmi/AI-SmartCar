/*
 * dsensor.c
 *
 *  Created on: 2021Äê6ÔÂ24ÈÕ
 *      Author: 936305695
 */
#include "dsensor.h"
#include "driver.h"


uint16_t DSensor_ReadADC(struct dsensor *self)
{
    return ADCx.Read(self->ADCn);
}

uint16_t DSensor_Read(struct dsensor *self)
{
    return self->ReadADC(self);
}

float DSensor_GetDistance(struct dsensor *self)
{
    float distance = 0.0;

    for(int i = 0 ; i < 10 ; i ++)
    {
        distance += self->Read(self);
    }

    distance /= 10.0;

    distance = 26.481*pow(distance,(-1.05));

    return distance;
}


uint16_t DSensor_Init(struct dsensor *self)
{
    ADCx.Init(self->ADCn);
    self->GetDistance = DSensor_GetDistance;
    self->Read = DSensor_Read;
    self->ReadADC = DSensor_ReadADC;

    return 0;
}
