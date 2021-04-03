/*
 * sdcard.c
 *
 *  Created on: 2021Äê1ÔÂ18ÈÕ
 *      Author: 936305695
 */
#include "sdcard.h"
#include "driver.h"

uint8_t Mem_ReadSector(struct memory *self,uint32_t sector,uint32_t page,uint8_t *buf,uint32_t len)
{
    return self->__ReadSector__(sector,page,buf,len);
}

uint8_t Mem_WriteSector(struct memory *self,uint32_t sector,uint32_t page,uint8_t *buf,uint32_t len)
{
    return self->__WriteSector__(sector,page,buf,len);
}

uint8_t Mem_ReadDisk(struct memory *self,uint32_t sector,uint count,uint8_t *buf)
{
    uint8_t res = 0;
    while(count--)
    {
        res += self->ReadSector(self,sector,0,buf,0);
    }
    return res;
}
uint8_t Mem_WriteDisk(struct memory *self,uint32_t sector,uint count,uint8_t *buf)
{
    uint8_t res = 0;
    while(count--)
    {
        res += self->WriteSector(self,sector,0,buf,0);
    }
    return res;
}

uint32_t Mem_GetSectorSize(struct memory *self)
{
    return self->CardSectorSize;
}

uint32_t Mem_GetBlockSize(struct memory *self)
{
    return self->CardBlockSize;
}
uint32_t Mem_GetSectorNum(struct memory *self)
{
    return self->CardSectorNum;
}

uint32_t Mem_GetCapacity(struct memory *self)
{
    return self->CardCapacity;
}

uint8_t Mem_Init(struct memory *self)
{
    self->WriteDisk = Mem_WriteDisk;
    self->ReadDisk = Mem_ReadDisk;
    self->WriteSector = Mem_WriteSector;
    self->ReadSector = Mem_ReadSector;

    self->CardSectorSize = 512;

    self->CardSectorNum = self->__GetSectorNum__();

    self->CardCapacity = self->CardSectorNum * self->CardSectorSize;

    self->CardBlockSize = 8;/*???*/

    self->GetSectorSize = Mem_GetSectorSize;

    self->GetBlockSize = Mem_GetBlockSize;

    self->GetSectorNum = Mem_GetSectorNum;

    self->GetCapacity = Mem_GetCapacity;

    uint8_t res = 0;

    if(self->__Init__ != NULL && self->__InitConfig__ != NULL)
        res = self->__Init__(self->__InitConfig__);

    return res;
}

