/*
 * dipswitch.c
 *
 *  Created on: 2021Äê1ÔÂ8ÈÕ
 *      Author: 936305695
 */
#include "dipswitch.h"

uint32_t DIPSwitch_Read(struct dip_switch *self)
{
    uint32_t Bits = 0;
    if(self->SwitchNum >= 32)
        return 0xFFFFFFFF;
    else
    {
        for(uint8_t i = 0;i<self->SwitchNum;i++)
        {
            if(self->Switch[i].Read(self->Switch[i].Self) == Switch_ON)
            {
                Bits |= 0x1 << i;
            }
        }
    }
    return self->Bits = Bits;

}
void DISSwitch_SetSwitchNum(struct dip_switch *self,uint8_t num)
{
    self->SwitchNum = num;
}

void DIPSwitch_Init(struct dip_switch *self)
{
    for(uint8_t i = 0;i<self->SwitchNum;i++)
        self->Switch[i].Init(self->Switch[i].Self);

    self->Bits = 0;
    self->SwitchNum = 4;

    self->Read = DIPSwitch_Read;
    self->SetSwitchNum = DISSwitch_SetSwitchNum;
}

