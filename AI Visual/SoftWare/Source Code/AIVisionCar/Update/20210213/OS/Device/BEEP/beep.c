/*
 * beep.c
 *
 *  Created on: 2020��12��14��
 *      Author: 936305695
 */
#include "beep.h"

void BEEP_ON(struct beep *self)
{
    GPIOx.Write(self->GPIODevice,self->ONLevel);
}

void BEEP_OFF(struct beep *self)
{
    GPIOx.Write(self->GPIODevice,!self->ONLevel);
}

void BEEP_Toggle(struct beep *self)
{
    GPIOx.Reverse(self->GPIODevice);
}

void BEEP_Init(struct beep *self)
{
    GPIOx.Init(self->GPIODevice);

    self->ONLevel = 0;

    self->ON = BEEP_ON;
    self->OFF = BEEP_OFF;
    self->Toggle = BEEP_Toggle;
}


