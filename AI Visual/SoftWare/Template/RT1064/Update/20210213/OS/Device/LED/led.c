/*
 * led.c
 *
 *  Created on: 2020Äê12ÔÂ8ÈÕ
 *      Author: 936305695
 */
#include "led.h"

void LED_ON(struct led *self)
{
    GPIOx.Write(self->GPIODevice,self->ONLevel);
}
void LED_OFF(struct led *self)
{
    GPIOx.Write(self->GPIODevice,!self->ONLevel);
}
void LED_Toggle(struct led *self)
{
    GPIOx.Reverse(self->GPIODevice);
}

void LED_Init(struct led *self)
{
    GPIOx.Init(self->GPIODevice);
    self->ONLevel = 0;
    self->OFF = LED_OFF;
    self->ON = LED_ON;
    self->Toggle = LED_Toggle;
}



