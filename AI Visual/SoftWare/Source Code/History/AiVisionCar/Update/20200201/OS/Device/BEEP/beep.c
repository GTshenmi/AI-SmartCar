/*
 * beep.c
 *
 *  Created on: 2020Äê12ÔÂ14ÈÕ
 *      Author: 936305695
 */
#include "beep.h"

void BEEP_ON(struct beep *self)
{
    GPIOx.Write(self->GPIODevice,0);
}

void BEEP_OFF(struct beep *self)
{
    GPIOx.Write(self->GPIODevice,1);
}

void BEEP_Toggle(struct beep *self)
{
    GPIOx.Reverse(self->GPIODevice);
}

void BEEP_Init(struct beep *self)
{
    GPIOx.Init(self->GPIODevice);
    self->ON = BEEP_ON;
    self->OFF = BEEP_OFF;
    self->Toggle = BEEP_Toggle;
}


