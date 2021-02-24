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
     public

         uint8_t (*Init)(struct sdcard *self);
         uint8_t (*HardWareInit)(void);

         uint8_t (*ReadSector)(struct sdcard *self,uint32_t sector,uint8_t *buf);
         uint8_t (*WriteSector)(struct sdcard *self,uint32_t sector,uint8_t *buf);

         uint8_t (*ReadDisk)(struct sdcard *self,uint32_t sector,uint count,uint8_t *buf);
         uint8_t (*WriteDisk)(struct sdcard *self,uint32_t sector,uint count,uint8_t *buf);

         const uint16_t CardSectorSize;
         const uint16_t CardBlockSize;
         const uint32_t CardCapacity;
         uint32_t CardSectorNum;

         struct sdcard *Self;
}sdcard_t;

uint8_t SDCard_Init(struct sdcard *self);

#endif /* OS_DEVICE_SDCARD_SDCARD_H_ */
