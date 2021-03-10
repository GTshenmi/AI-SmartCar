/*
 * switch.c
 *
 *  Created on: 2021Äê1ÔÂ8ÈÕ
 *      Author: 936305695
 */
#include "switch.h"


uint8_t Switch_ReadIOLevel(struct switchx *self)
{
    return GPIOx.Read(self->GPIOn);
}

switch_state_t Switch_Read(struct switchx *self)
{
    return (self->ReadIOLevel(self) == self->DownLevel)?Switch_ON:Switch_OFF;
}

void Switch_SetDownLevel(struct switchx *self,uint8_t downlevel)
{
    self->DownLevel = downlevel;
}

void Switch_Init(struct switchx *self)
{
    GPIOx.Init(self->GPIOn);
    self->Level = 0;
    self->DownLevel = 0;
    self->ReadIOLevel = Switch_ReadIOLevel;
    self->Read = Switch_Read;
    self->SetDownLevel = Switch_SetDownLevel;
    self->State = Switch_NULL;
}


