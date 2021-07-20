/*
 * switch.c
 *
 *  Created on: 2021Äê1ÔÂ8ÈÕ
 *      Author: 936305695
 */
#include "switch.h"

switch_state_t Switch_Read(struct switchx *self)
{
    return (GPIOx.Read(self->GPIODevice))?Switch_ON:Switch_OFF;
}
void Switch_Init(struct switchx *self)
{
    GPIOx.Init(self->GPIODevice);
    self->Level = 0;
    self->Read = Switch_Read;
    self->State = Switch_OFF;
}


