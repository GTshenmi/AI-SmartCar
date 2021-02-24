/*
 * capture.c
 *
 *  Created on: 2020Äê12ÔÂ14ÈÕ
 *      Author: 936305695
 */
#include "capture.h"

uint8_t Cap_Read(struct capture *self)
{       
   return 0;
}
void Cap_Test(struct capture *self)
{
    self->Init(self,30);
    self->Read(self);
}

uint8_t Cap_Init(struct capture *self,uint8_t fps)
{
    self->Read = Cap_Read;

    self->Test = Cap_Test;

    uint8_t res = 0;

    //res =  DMAx.Init(self->DMADevice);

    if(self->__Init__ != NULL && self->__InitConfig__ != NULL)
        res += self->__Init__(self->__InitConfig__);

    return res;
}




