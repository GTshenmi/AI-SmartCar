/*
 * sdcard.c
 *
 *  Created on: 2021Äê1ÔÂ18ÈÕ
 *      Author: 936305695
 */
#include "sdcard.h"

uint8_t SDCard_ReadDisk(struct sdcard *self,uint32_t sector,uint count,uint8_t *buf)
{
    uint8_t res = 0;
    while(count--)
    {
        res += self->ReadSector(self,sector,buf);
    }
    return res;
}
uint8_t SDCard_WriteDisk(struct sdcard *self,uint32_t sector,uint count,uint8_t *buf)
{
    uint8_t res = 0;
    while(count--)
    {
        res += self->WriteSector(self,sector,buf);
    }
    return res;
}


uint8_t SDCard_Init(struct sdcard *self)
{
    self->WriteDisk = SDCard_WriteDisk;
    self->ReadDisk = SDCard_ReadDisk;
    self->CardSectorNum = (self->CardCapacity/self->CardSectorSize);
    return  self->HardWareInit();;
}

