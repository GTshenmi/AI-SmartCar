/*
 * capture.h
 *
 *  Created on: 2020Äê12ÔÂ14ÈÕ
 *      Author: 936305695
 */

#ifndef OS_DEVICE_CAPTURE_CAPTURE_H_
#define OS_DEVICE_CAPTURE_CAPTURE_H_

#include "driver.h"

typedef struct capture
{
     uint8_t (*HardWareInit)(uint8_t fps);

     uint8_t (*Init)(struct capture *self,uint8_t fps);
     uint8_t (*Read)(struct capture *self);
     uint8_t (*DataProcess)(struct capture *self);
     uint8_t (*CameraAlgorithm)(struct capture *self);

     void (*ShowIMG)(struct capture *self);
     void (*ReportIMG)(struct capture *self);

     void (*Test)(struct capture *self);

     uint8_t IMG_Hight;
     uint8_t IMG_Weight;

     uint8_t *IMG_Data;

     dmax_t *DMADevice;
     bool Al_Is_Busy;
     bool Cap_Is_Busy;
     bool Enable;
}capture_t;

uint8_t CAP_Init(struct capture *self,uint8_t fps);

#endif /* OS_DEVICE_CAPTURE_CAPTURE_H_ */
