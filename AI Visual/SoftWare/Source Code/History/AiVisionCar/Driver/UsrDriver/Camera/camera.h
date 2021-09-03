/*
 * camera.h
 *
 *  Created on: 2020Äê12ÔÂ14ÈÕ
 *      Author: 936305695
 */

#ifndef DRIVER_USRDRIVER_CAMERA_CAMERA_H_
#define DRIVER_USRDRIVER_CAMERA_CAMERA_H_

#include "sys_driver.h"
#include "LQ_MT9V034M.h"
#include "LQ_CSI.h"

extern uint16_t fps;

uint8_t CameraInit(void *config);
uint8_t CameraRead(void);
uint8_t CameraStop(void);
uint8_t CameraStart(void);
uint8_t CameraGetState(void);

#endif /* DRIVER_USRDRIVER_CAMERA_CAMERA_H_ */
