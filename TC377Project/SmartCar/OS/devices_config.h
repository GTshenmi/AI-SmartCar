/*
 * devices_config.h
 *
 *  Created on: 2021Äê1ÔÂ22ÈÕ
 *      Author: 936305695
 */

#ifndef OS_DEVICES_CONFIG_H_
#define OS_DEVICES_CONFIG_H_

#include "device.h"

extern beep_t BEEP;

extern bluetooth_t Bluetooth;

extern communicate_t CUART;
extern communicate_t DebugCom;

extern esensor_t LESensor[5];
extern esensor_t SESensor[7];

extern key_t KEY[7];

extern led_t GLED;
extern led_t BLED;

extern motor_ctrl_t Motor;

extern screen_t Screen;

extern memory_t SDCard;

extern servo_ctrl_t Servo;

extern switch_t Switch[8];
extern dip_switch_t DIPSwitch;

extern wireless_serial_t WirelessSerial;

#endif /* OS_DEVICES_CONFIG_H_ */
