/*
 * capture.c
 *
 *  Created on: 2020年12月14日
 *      Author: 936305695
 */
#include "capture.h"

image_t Cap_Read(struct capture *self,uint16_t flags)
{
    if(self->State == Capture_Start)
    {
        self->State = Capture_Busy;

        switch(flags)
        {
            case 0:/*阻塞*/
                self->__Read__();
                self->State = Capture_Fin;
                break;
            case 1:/*查询(单次)*/
                break;
            case 2:/*查询(连续)*/
                break;
            default:
                break;
        }
    }
    else if(self->State == Capture_Fin)
    {

    }

    return self->ImageCache;
}

cap_state_t Cap_GetState(struct capture *self)
{
    return self->State;
}

void Cap_Test(struct capture *self)
{
    self->Init(self,30);
    //self->Read(self,0);
}

void Cap_Start(struct capture *self)
{
    if(self->State == Capture_Free)
    {
        self->__Start__();
        self->State = Capture_Start;
    }
    else if(self->State == Capture_Fin)
    {
        self->State = Capture_Start;
    }
}

void Cap_Stop(struct capture *self)
{
    if(self->State == Capture_Fin)
    {
        self->__Stop__();
        self->State = Capture_Free;
    }
}

uint8_t Cap_Init(struct capture *self,uint8_t fps)
{
    self->Read = Cap_Read;

    self->GetState = Cap_GetState;

    self->Test = Cap_Test;

    uint8_t res = 0;

    if(self->__Init__ != NULL && self->__InitConfig__ != NULL)
        res += self->__Init__(self->__InitConfig__);

    return res;
}




