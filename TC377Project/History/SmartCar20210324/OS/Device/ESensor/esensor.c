/*
 * esensor.c
 *
 *  Created on: 2020年12月6日
 *      Author: 936305695
 */
#include <esensor.h>
#include "driver.h"

uint16_t ESensorRead(struct esensor *self)
{
    /*use filter*/
    if(self->ConfigReg & ESENSOR_BITS_FILTER_ENABLE_MASK)
        self->Cache = self->Filter(self,self->Argv,self->Argc);
    else
    {
#if ESENSOR_FAST_READ_LEVEL == ESENSOR_FAST_READ_LEVEL_0
        self->Cache = self->ReadADC(self);
#else
        self->Cache = ESensor_ReadADC(self);
#endif
    }
    /*use output gain*/
    if(self->ConfigReg & ESENSOR_BITS_GAIN_ENABLE_MASK)
        self->Cache = (uint16_t)((self->Cache) * self->Gain);

#if 0
    if(self->Cache * 1.0 > ADCx.MaxValue  * self->Gain)
        self->Cache = (uint16_t)ADCx.MaxValue * self->Gain;
#endif

    return self->Cache;
}

static unsigned ESensorMidFilter(struct esensor *self)
{
    uint16_t i,j,k,tmp;

    //1.取3次A/D转换结果
#if ESENSOR_FAST_READ_LEVEL == ESENSOR_FAST_READ_LEVEL_0
    i = self->ReadADC(self);
    j = self->ReadADC(self);
    k = self->ReadADC(self);
#else
    i = ESensor_ReadADC(self);
    j = ESensor_ReadADC(self);
    k = ESensor_ReadADC(self);
#endif
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

uint16_t ESensorFilter(struct esensor *self,void *argv,uint16_t argc)
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

void ESensorBindUsrData(struct esensor *self,void *argv,uint16_t argc)
{
    self->Argv = argv;
    self->Argc = argc;
}

#if ESENSOR_FAST_READ_LEVEL == ESENSOR_FAST_READ_LEVEL_0
uint16_t ESensor_ReadADC(struct esensor *self)
{
    return ADCx.Read(self->ADCn);
}
#endif

uint16_t ESensorInit(struct esensor *self)
{
    ADCx.Init(self->ADCn);

    self->Argv = NULL;
    self->Argc = 0;
    self->Cache = 0;
    self->Gain = 0.0;
    self->ConfigReg = 0x00000000;
    self->FilterBufLen = 10;

    //memset(self->FilterBuf,0,sizeof(self->FilterBuf[0]) * self->FilterBufLen);

    self->Read = ESensorRead;
    self->Filter = ESensorFilter;
    self->EnableFilter = ESensorEnableFilter;
    self->EnableGain = ESensorEnableGain;
    self->SetGain = ESensorSetGain;
    self->ReadFromCache = ESensorReadFromCache;
    self->BindUsrData = ESensorBindUsrData;

#if ESENSOR_FAST_READ_LEVEL == ESENSOR_FAST_READ_LEVEL_0
    self->ReadADC = ESensor_ReadADC;
#endif

    self->SetGain(self,1.0);
    self->EnableFilter(self,true);
    self->EnableGain(self,false);

    return 0;
}



