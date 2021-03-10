/*
 * devices_config.h
 *
 *  Created on: 2021Äê1ÔÂ22ÈÕ
 *      Author: 936305695
 */

#ifndef OS_DEVICES_CONFIG_H_
#define OS_DEVICES_CONFIG_H_

#include "device.h"

extern capture_t Capture;
extern communicate_t CUART;
extern communicate_t DebugCom;

extern key_t KEY[6];

extern led_t RLED;

extern motor_ctrl_t LMotor;
extern motor_ctrl_t RMotor;

extern screen_t Screen;

extern sdcard_t SDCard;

extern servo_ctrl_t Servo1;
extern servo_ctrl_t Servo2;

#endif /* OS_DEVICES_CONFIG_H_ */
