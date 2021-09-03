/*
 * control.h
 *
 *  Created on: 2020Äê12ÔÂ6ÈÕ
 *      Author: 936305695
 */

#ifndef APP_USR_CONTROL_H_
#define APP_USR_CONTROL_H_

#include "sys.h"

void SpeedControl(void *argv);
void AngleControl(void *argv);

sint16_t MotorCtrlStrategy(struct motor_ctrl *self,float target_speed,float actual_speed,void *argv,uint16_t argc);
uint16_t ServoCtrlStrategy(struct servo_ctrl *self,float target_angle,float actual_angle,void *argv,uint16_t argc);

#endif /* APP_USR_CONTROL_H_ */
