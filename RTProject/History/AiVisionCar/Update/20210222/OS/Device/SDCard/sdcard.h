/*
 * sdcard.h
 *
 *  Created on: 2021Äê1ÔÂ18ÈÕ
 *      Author: 936305695
 */

#ifndef OS_DEVICE_SDCARD_SDCARD_H_
#define OS_DEVICE_SDCARD_SDCARD_H_

#include "driver.h"

typedef struct sdcard
{
     private

         uint8_t (*__Init__)(void *config);
         void *__InitConfig__;

         uint8_t (*__ReadSector__)(uint32_t sector,uint8_t *buf);
         uint8_t (*__WriteSector__)(uint32_t sector,uint8_t *buf);
         uint32_t (*__GetSectorNum__)(void);

         uint32_t CardSectorSize;
         uint32_t CardBlockSize;
         uint32_t CardCapacity;
         uint32_t CardSectorNum;

     public

         uint8_t (*Init)(struct sdcard *self);

         uint8_t (*ReadSector)(struct sdcard *self,uint32_t sector,uint8_t *buf);
         uint8_t (*WriteSector)(struct sdcard *self,uint32_t sector,uint8_t *buf);
         uint8_t (*ReadDisk)(struct sdcard *self,uint32_t sector,uint count,uint8_t *buf);
         uint8_t (*WriteDisk)(struct sdcard *self,uint32_t sector,uint count,uint8_t *buf);

         uint32_t (*GetSectorSize)(struct sdcard *self);
         uint32_t (*GetBlockSize)(struct sdcard *self);
         uint32_t (*GetSectorNum)(struct sdcard *self);

         uint32_t (*GetCapacity)(struct sdcard *self);

//         uint32_t (*GetUsageCapacity)(struct sdcard *self);
//         uint32_t (*GetFreeCapacity)(struct sdcard *self);


         struct sdcard *Self;

}sdcard_t;

uint8_t SDCard_Init(struct sdcard *self);

#endif /* OS_DEVICE_SDCARD_SDCARD_H_ */
