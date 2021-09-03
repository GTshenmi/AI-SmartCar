/*
 * control.h
 *
 *  Created on: 2020Äê12ÔÂ6ÈÕ
 *      Author: 936305695
 */

#ifndef APP_USR_CONTROL_H_
#define APP_USR_CONTROL_H_

#include "sys.h"

void Motor_ExecutionUnitRun(struct unit *self,void *data);
void Servo_ExecutionUnitRun(struct unit *self,void *data);
unsigned short MotorCtrlStrategy(struct motor_ctrl *self,signed short target_speed,signed short actual_speed,void *usr);
unsigned short ServoCtrlStrategy(struct servo_ctrl *self,signed short target_angle,float actual_angle,void *usr);

#endif /* APP_USR_CONTROL_H_ */
