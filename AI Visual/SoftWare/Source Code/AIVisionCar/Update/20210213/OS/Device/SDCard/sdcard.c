/*
 * sdcard.c
 *
 *  Created on: 2021Äê1ÔÂ18ÈÕ
 *      Author: 936305695
 */
#include "sdcard.h"

uint8_t SDCard_ReadSector(struct sdcard *self,uint32_t sector,uint8_t *buf)
{
    return self->__ReadSector__(sector,buf);
}

uint8_t SDCard_WriteSector(struct sdcard *self,uint32_t sector,uint8_t *buf)
{
    return self->__WriteSector__(sector,buf);
}

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

uint32_t SDCard_GetSectorSize(struct sdcard *self)
{
    return self->CardSectorSize;
}

uint32_t SDCard_GetBlockSize(struct sdcard *self)
{
    return self->CardBlockSize;
}
uint32_t SDCard_GetSectorNum(struct sdcard *self)
{
    return self->CardSectorNum;
}

uint32_t SDCard_GetCapacity(struct sdcard *self)
{
    return self->CardCapacity;
}

uint8_t SDCard_Init(struct sdcard *self)
{
    self->WriteDisk = SDCard_WriteDisk;
    self->ReadDisk = SDCard_ReadDisk;
    self->WriteSector = SDCard_WriteSector;
    self->ReadSector = SDCard_ReadSector;

    self->CardSectorSize = 512;

    self->CardSectorNum = self->__GetSectorNum__();

    self->CardCapacity = self->CardSectorNum * self->CardSectorSize;

    self->CardBlockSize = 8;/*???*/

    self->GetSectorSize = SDCard_GetSectorSize;

    self->GetBlockSize = SDCard_GetBlockSize;

    self->GetSectorNum = SDCard_GetSectorNum;

    self->GetCapacity = SDCard_GetCapacity;

    uint8_t res = 0;

    if(self->__Init__ != NULL && self->__InitConfig__ != NULL)
        res = self->__Init__(self->__InitConfig__);

    return res;
}

