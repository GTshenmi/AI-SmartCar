/*
 * beep.c
 *
 *  Created on: 2020Äê12ÔÂ14ÈÕ
 *      Author: 936305695
 */
#include "beep.h"
#include "driver.h"

void BEEP_ON(struct beep *self)
{
    GPIOx.Write(self->GPIOn,self->ONLevel);
}

void BEEP_OFF(struct beep *self)
{
    GPIOx.Write(self->GPIOn,!self->ONLevel);
}

void BEEP_Toggle(struct beep *self)
{
    GPIOx.Reverse(self->GPIOn);
}

void BEEP_Init(struct beep *self)
{
    GPIOx.Init(self->GPIOn);

    self->ONLevel = 0;

    self->ON = BEEP_ON;
    self->OFF = BEEP_OFF;
    self->Toggle = BEEP_Toggle;
}


