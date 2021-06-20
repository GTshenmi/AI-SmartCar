/*
 * sdcard.c
 *
 *  Created on: 2021Äê1ÔÂ18ÈÕ
 *      Author: 936305695
 */
#include <memory.h>
#include "driver.h"

uint8_t Mem_ReadSector(struct memory *self,uint32_t sector,uint32_t page,uint8_t *buf,uint32_t len)
{
    return self->__ReadSector__(sector,page,buf,len);
}

uint8_t Mem_WriteSector(struct memory *self,uint32_t sector,uint32_t page,uint8_t *buf,uint32_t len)
{
    return self->__WriteSector__(sector,page,buf,len);
}

uint8_t Mem_ReadBlock(struct memory *self,uint32_t sector,uint8_t *buf,uint32_t len)
{
    uint8_t res = 0;
    while(len--)
    {
        res += self->ReadSector(self,sector,0,buf,0);
    }
    return res;
}

uint8_t Mem_WriteBlock(struct memory *self,uint32_t sector,uint8_t *buf,uint32_t len)
{
    uint8_t res = 0;
    while(len--)
    {
        res += self->WriteSector(self,sector,0,buf,0);
    }
    return res;
}


uint8_t Mem_ReadDisk(struct memory *self,uint32_t sector,uint count,uint8_t *buf)
{
    return self->ReadBlock(self,sector,buf,count);
}
uint8_t Mem_WriteDisk(struct memory *self,uint32_t sector,uint count,uint8_t *buf)
{
    return self->WriteBlock(self,sector,buf,count);
}

uint32_t Mem_GetSectorSize(struct memory *self)
{
    return self->__SectorSize__;
}

uint32_t Mem_GetBlockSize(struct memory *self)
{
    return self->__BlockSize__;
}
uint32_t Mem_GetSectorNum(struct memory *self)
{
    return self->__SectorNum__;
}

uint32_t Mem_GetCapacity(struct memory *self)
{
    return self->__Capacity__;
}

uint32_t Mem_Write(struct memory *self,uint32_t sector,uint32_t page,const char *fmt,...)
{
    char writebuf[MEM_MAX_BUF_LEN];

    va_list arg;
    va_start(arg,fmt);

    uint32_t len = vsprintf(writebuf,fmt,arg);

    self->WriteSector(self,sector,page,(uint8_t *)writebuf,len);

    va_end(arg);

    return len;
}

uint32_t Mem_WriteLine(struct memory *self,uint32_t sector,uint32_t page,const char *fmt,...)
{
    char writebuf[MEM_MAX_BUF_LEN];

    va_list arg;
    va_start(arg,fmt);

    uint32_t len = vsprintf(writebuf,fmt,arg);

    if(len < MEM_MAX_BUF_LEN - 1)
        writebuf[len++] = '\n';
    else
        writebuf[MEM_MAX_BUF_LEN - 1] = '\n';

    self->WriteSector(self,sector,page,(uint8_t *)writebuf,len);

    va_end(arg);

    return len;
}

uint32_t Mem_Read(struct memory *self,uint32_t sector,uint32_t page,const char *fmt,...)
{
    uint32_t len = 0;

    char readbuf[MEM_MAX_BUF_LEN];

    va_list arg;
    va_start(arg,fmt);

    for(; len < MEM_MAX_BUF_LEN - 1;len++)
    {
        self->ReadSector(self,sector,page,(uint8_t *)readbuf,MEM_MAX_BUF_LEN);
    }

    vsscanf(readbuf,fmt,arg);

    va_end(arg);

    return len;
}

uint32_t Mem_ReadLine(struct memory *self,uint32_t sector,uint32_t page,const char *fmt,...)
{
    uint32_t len = 0;

    char readbuf[MEM_MAX_BUF_LEN];

    va_list arg;
    va_start(arg,fmt);

    for(; len < MEM_MAX_BUF_LEN - 1;len++)
    {
        self->ReadSector(self,sector,page,(uint8_t *)readbuf,MEM_MAX_BUF_LEN);
    }

    vsscanf(readbuf,fmt,arg);

    va_end(arg);

    return len;
}

uint8_t Mem_Init(struct memory *self)
{
    self->WriteDisk = Mem_WriteDisk;
    self->ReadDisk = Mem_ReadDisk;
    self->WriteSector = Mem_WriteSector;
    self->ReadSector = Mem_ReadSector;

    self->ReadBlock = Mem_ReadBlock;
    self->WriteBlock = Mem_WriteBlock;

    self->Read  = Mem_Read;
    self->Write = Mem_Write;
    self->ReadLine = Mem_ReadLine;
    self->WriteLine = Mem_WriteLine;

    self->__SectorNum__ = self->__GetSectorNum__();

    self->__Capacity__ = self->__SectorNum__ * self->__SectorSize__;

    self->GetSectorSize = Mem_GetSectorSize;

    self->GetBlockSize = Mem_GetBlockSize;

    self->GetSectorNum = Mem_GetSectorNum;

    self->GetCapacity = Mem_GetCapacity;

    uint8_t res = 0;

    if(self->__Init__ != NULL && self->__InitConfig__ != NULL)
        res = self->__Init__(self->__InitConfig__);

    return res;
}

