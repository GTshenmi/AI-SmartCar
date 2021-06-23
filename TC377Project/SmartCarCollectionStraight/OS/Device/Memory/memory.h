/*
 * sdcard.h
 *
 *  Created on: 2021Äê1ÔÂ18ÈÕ
 *      Author: 936305695
 */

#ifndef OS_DEVICE_MEMORY_MEMORY_H_
#define OS_DEVICE_MEMORY_MEMORY_H_

#include "platform.h"

#define MEM_MAX_BUF_LEN 512

typedef struct memory
{
     private

         uint8_t (*__Init__)(void *config);
         void *__InitConfig__;

         uint8_t (*__ReadSector__)(uint32_t sector,uint32_t page,uint8_t *buf,uint32_t len);
         uint8_t (*__WriteSector__)(uint32_t sector,uint32_t page,uint8_t *buf,uint32_t len);
         uint32_t (*__GetSectorNum__)(void);

         uint32_t __SectorSize__;
         uint32_t __BlockSize__;
         uint32_t __Capacity__;
         uint32_t __SectorNum__;

     public

         uint8_t (*Init)(struct memory *self);

         uint8_t (*ReadSector)(struct memory *self,uint32_t sector,uint32_t page,uint8_t *buf,uint32_t len);
         uint8_t (*WriteSector)(struct memory *self,uint32_t sector,uint32_t page,uint8_t *buf,uint32_t len);

         uint8_t (*ReadBlock)(struct memory *self,uint32_t sector,uint8_t *buf,uint32_t len);
         uint8_t (*WriteBlock)(struct memory *self,uint32_t sector,uint8_t *buf,uint32_t len);

         /*To Support Fatfs.*/
         uint8_t (*ReadDisk)(struct memory *self,uint32_t sector,uint count,uint8_t *buf);
         uint8_t (*WriteDisk)(struct memory *self,uint32_t sector,uint count,uint8_t *buf);

         uint32_t (*Write)(struct memory *self,uint32_t sector,uint32_t page,const char *fmt,...);
         uint32_t (*WriteLine)(struct memory *self,uint32_t sector,uint32_t page,const char *fmt,...);
         uint32_t (*Read)(struct memory *self,uint32_t sector,uint32_t page,const char *fmt,...);
         uint32_t (*ReadLine)(struct memory *self,uint32_t sector,uint32_t page,const char *fmt,...);

         uint32_t (*GetSectorSize)(struct memory *self);
         uint32_t (*GetBlockSize)(struct memory *self);
         uint32_t (*GetSectorNum)(struct memory *self);

         uint32_t (*GetCapacity)(struct memory *self);

//         uint32_t (*GetUsageCapacity)(struct sdcard *self);
//         uint32_t (*GetFreeCapacity)(struct sdcard *self);


         struct memory *Self;

}memory_t;

uint8_t Mem_Init(struct memory *self);

#endif /* OS_DEVICE_MEMORY_MEMORY_H_ */
