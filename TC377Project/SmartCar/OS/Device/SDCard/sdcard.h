/*
 * sdcard.h
 *
 *  Created on: 2021Äê1ÔÂ18ÈÕ
 *      Author: 936305695
 */

#ifndef OS_DEVICE_SDCARD_SDCARD_H_
#define OS_DEVICE_SDCARD_SDCARD_H_

#include "platform.h"

typedef struct memory
{
     private

         uint8_t (*__Init__)(void *config);
         void *__InitConfig__;

         uint8_t (*__ReadSector__)(uint32_t sector,uint32_t page,uint8_t *buf,uint32_t len);
         uint8_t (*__WriteSector__)(uint32_t sector,uint32_t page,uint8_t *buf,uint32_t len);
         uint32_t (*__GetSectorNum__)(void);

         uint32_t CardSectorSize;
         uint32_t CardBlockSize;
         uint32_t CardCapacity;
         uint32_t CardSectorNum;

     public

         uint8_t (*Init)(struct memory *self);

         uint8_t (*ReadSector)(struct memory *self,uint32_t sector,uint32_t page,uint8_t *buf,uint32_t len);
         uint8_t (*WriteSector)(struct memory *self,uint32_t sector,uint32_t page,uint8_t *buf,uint32_t len);
         uint8_t (*ReadDisk)(struct memory *self,uint32_t sector,uint count,uint8_t *buf);
         uint8_t (*WriteDisk)(struct memory *self,uint32_t sector,uint count,uint8_t *buf);

         uint32_t (*Write)(struct memory *self,sint64_t time_out,const char *fmt,...);
         uint32_t (*WriteLine)(struct memory *self,sint64_t time_out,const char *fmt,...);
         uint32_t (*Read)(struct memory *self,sint64_t time_out,const char *fmt,...);
         uint32_t (*ReadLine)(struct memory *self,sint64_t time_out,const char *fmt,...);

         uint32_t (*GetSectorSize)(struct memory *self);
         uint32_t (*GetBlockSize)(struct memory *self);
         uint32_t (*GetSectorNum)(struct memory *self);

         uint32_t (*GetCapacity)(struct memory *self);

//         uint32_t (*GetUsageCapacity)(struct sdcard *self);
//         uint32_t (*GetFreeCapacity)(struct sdcard *self);


         struct memory *Self;

}memory_t;

uint8_t Mem_Init(struct memory *self);

#endif /* OS_DEVICE_SDCARD_SDCARD_H_ */
