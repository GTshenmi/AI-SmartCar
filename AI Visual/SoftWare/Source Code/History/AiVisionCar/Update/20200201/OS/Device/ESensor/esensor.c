/*
 * esensor_driver.c
 *
 *  Created on: 2020年12月6日
 *      Author: 936305695
 */
#include <esensor.h>

uint16_t ESensorRead(struct esensor *self)
{
    /*use filter*/
    if(self->ConfigReg & ESENSOR_BITS_FILTER_ENABLE_MASK)
        self->Cache = self->Filter(self,self->UsrData);
    else
        self->Cache = ADCx.Read(self->ADCDevice);

    /*use output gain*/
    if(self->ConfigReg & ESENSOR_BITS_GAIN_ENABLE_MASK)
        self->Cache = (uint16_t)((self->Cache) * self->Gain);

    return self->Cache;
}

static unsigned ESensorMidFilter(struct esensor *self)
{
    uint16_t i,j,k,tmp;
    //1.取3次A/D转换结果
    i = ADCx.Read(self->ADCDevice);
    j = ADCx.Read(self->ADCDevice);
    k = ADCx.Read(self->ADCDevice);
    //2.取中值
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

uint16_t ESensorFilter(struct esensor *self,void *usrdata)
{
    return ESensorSlidingMedianFilter(self,self->FilterBuf,self->FilterBufLen,ESensorMidFilter(self));
}

void ESensorEnableFilter(struct esensor *self,bool enable)
{
    if(enable)
        self->ConfigReg |= ESENSOR_BITS_FILTER_ENABLE_MASK;
    else
        self->ConfigReg &= ~ESENSOR_BITS_FILTER_ENABLE_MASK;
}

void ESensorEnableGain(struct esensor *self,bool enable)
{
    if(enable)
        self->ConfigReg |= ESENSOR_BITS_GAIN_ENABLE_MASK;
    else
        self->ConfigReg &= ~ESENSOR_BITS_GAIN_ENABLE_MASK;
}

void ESensorSetGain(struct esensor *self,float gain)
{
    self->Gain = gain;
}

uint16_t ESensorReadFromCache(struct esensor *self)
{
    return self->Cache;
}

void ESensorBindUsrData(struct esensor *self,void *usrdata)
{
    self->UsrData = usrdata;
}

uint16_t ESensorInit(struct esensor *self)
{
    ADCx.Init(self->ADCDevice);

    self->UsrData = NULL;
    self->Cache = 0;
    self->Gain = 0.0;
    self->ConfigReg = 0x00000000;

    self->Read = ESensorRead;
    self->Filter = ESensorFilter;
    self->EnableFilter = ESensorEnableFilter;
    self->EnableGain = ESensorEnableGain;
    self->SetGain = ESensorSetGain;
    self->ReadFromCache = ESensorReadFromCache;
    self->BindUsrData = ESensorBindUsrData;

    self->SetGain(self,1.0);
    self->EnableFilter(self,true);
    self->EnableGain(self,false);

    return 0;
}



