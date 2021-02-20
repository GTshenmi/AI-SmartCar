/*
 * esensor_driver.c
 *
 *  Created on: 2020��12��6��
 *      Author: 936305695
 */
#include <esensor.h>

static unsigned ESensorMidFilter(struct esensor *self);

uint16_t ESensorRead(struct esensor *self)
{
    if(self->Use_Filter)
        self->Cache = self->Filter(self);
    else
        self->Cache = ADCx.Read(self->ADCDevice);

    return self->Cache;
}

static uint16_t ESensorSlidingMedianFilter(struct esensor *self,uint16_t *FilterBuf,uint16_t len,uint16_t new_data)
{
    for(sint16_t i = 0;i < len-1 ;i++)
        FilterBuf[i] = FilterBuf[i+1];
    FilterBuf[len-1] = new_data;

    uint16_t result = 0;
    for(sint16_t i = 0;i < len ;i++)
    {
        result += FilterBuf[i];
    }
    result /= len;

    return result;
}

static unsigned ESensorMidFilter(struct esensor *self)
{
    uint16_t i,j,k,tmp;
    //1.ȡ3��A/Dת�����
    i = ADCx.Read(self->ADCDevice);
    j = ADCx.Read(self->ADCDevice);
    k = ADCx.Read(self->ADCDevice);
    //2.ȡ��ֵ
    if (i > j)
    {
        tmp = i; i = j; j = tmp;
    }
    if (k > j)
        tmp = j;
    else if(k > i)
        tmp = k;
    else
        tmp = i;

    return tmp;
}

uint16_t ESensorFilter(struct esensor *self)
{
    return ESensorSlidingMedianFilter(self,self->FilterBuf,self->FilterBufLen,ESensorMidFilter(self));
}

uint16_t ESensorInit(struct esensor *self)
{
    ADCx.Init(self->ADCDevice);
    self->Read = ESensorRead;
    self->Filter = ESensorFilter;
    self->Cache = 0;
    self->Use_Filter = true;
    return 0;
}



