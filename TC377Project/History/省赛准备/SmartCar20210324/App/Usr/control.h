/*
 * control.h
 *
 *  Created on: 2020Äê12ÔÂ6ÈÕ
 *      Author: 936305695
 */

#ifndef APP_USR_CONTROL_H_
#define APP_USR_CONTROL_H_

#include "sys.h"

void Motor_ExecutionUnitRun(struct unit *self,void *argv,uint16_t argc);
void Servo_ExecutionUnitRun(struct unit *self,void *argv,uint16_t argc);
sint16_t MotorCtrlStrategy(struct motor_ctrl *self,sint16_t target_speed,sint16_t actual_speed,void *argv,uint16_t argc);
uint16_t ServoCtrlStrategy(struct servo_ctrl *self,float target_angle,float actual_angle,void *argv,uint16_t argc);

#endif /* APP_USR_CONTROL_H_ */
