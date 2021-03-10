/*
 * control.h
 *
 *  Created on: 2020Äê12ÔÂ6ÈÕ
 *      Author: 936305695
 */

#ifndef APP_USR_CONTROL_H_
#define APP_USR_CONTROL_H_

#include "sys.h"

sint16_t LeftMotorCtrlStrategy(struct motor_ctrl *self,sint16_t target_speed,sint16_t actual_speed,void *argv,uint16_t argc);
sint16_t RightMotorCtrlStrategy(struct motor_ctrl *self,sint16_t target_speed,sint16_t actual_speed,void *argv,uint16_t argc);
uint16_t Servo1CtrlStrategy(struct servo_ctrl *self,signed short target_angle,float actual_angle,void *argv,uint16_t argc);
uint16_t Servo2CtrlStrategy(struct servo_ctrl *self,signed short target_angle,float actual_angle,void *argv,uint16_t argc);

#endif /* APP_USR_CONTROL_H_ */
