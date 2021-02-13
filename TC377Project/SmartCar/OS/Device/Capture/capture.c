/*
 * capture.c
 *
 *  Created on: 2020Äê12ÔÂ14ÈÕ
 *      Author: 936305695
 */
#include "capture.h"
uint8_t CAP_Read(struct capture *self)
{
    if(!self->Cap_Is_Busy && !self->Al_Is_Busy && self->Enable)
    {
        self->Cap_Is_Busy = true;
        DMAx.Start(self->DMADevice);
    }
    return 0;
}
uint8_t CAP_DataProcess(struct capture *self)
{
    return 0;
}
uint8_t CAP_CameraAlgorithm(struct capture *self)
{
    if(!self->Cap_Is_Busy && !self->Al_Is_Busy && self->Enable)
    {
        self->Al_Is_Busy = true;

        self->DataProcess(self);

        self->Al_Is_Busy = false;
    }
    return 0;
}
void CAP_Test(struct capture *self)
{
    self->Init(self,30);
    self->Read(self);
    while(!self->Cap_Is_Busy)
    {
        self->Enable = false;
        self->ShowIMG(self);
        self->ReportIMG(self);
        self->Enable = true;
    }
}

uint8_t CAP_Init(struct capture *self,uint8_t fps)
{
    self->HardWareInit(fps);
    self->Read = CAP_Read;
    self->DataProcess = CAP_DataProcess;
    self->CameraAlgorithm = CAP_CameraAlgorithm;
    self->Cap_Is_Busy = false;
    self->Enable = true;
    self->Al_Is_Busy = true;
    self->Test = CAP_Test;
    return DMAx.Init(self->DMADevice);
}




