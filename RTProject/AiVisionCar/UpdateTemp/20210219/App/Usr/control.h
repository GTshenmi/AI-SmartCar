/*
 * control.h
 *
 *  Created on: 2020��12��6��
 *      Author: 936305695
 */

#ifndef APP_USR_CONTROL_H_
#define APP_USR_CONTROL_H_

#include "sys.h"

void Motor_ExecutionUnitRun(struct unit *self,void *argv,uint16_t argc);
void Servo_ExecutionUnitRun(struct unit *self,void *argv,uint16_t argc);
sint16_t MotorCtrlStrategy(struct motor_ctrl *self,signed short target_speed,signed short actual_speed,void *argv,uint16_t argc);
unsigned short ServoCtrlStrategy(struct servo_ctrl *self,signed short target_angle,float actual_angle,void *argv,uint16_t argc);

#endif /* APP_USR_CONTROL_H_ */
