/*
 * dataprocess.h
 *
 *  Created on: 2020Äê12ÔÂ6ÈÕ
 *      Author: 936305695
 */

#ifndef APP_USR_DATAPROCESS_H_
#define APP_USR_DATAPROCESS_H_

#include "sys.h"

void Motor_SensorUnitRun_ALLMode(struct unit *self,void *data);
void Servo_SensorUnitRun_ALLMode(struct unit *self,void *data);

void Motor_DecisionUnitRun_AIMode(struct unit *self,void *data);
void Servo_DecisionUnitRun_AIMode(struct unit *self,void *data);

void Motor_DecisionUnitRun_AutoBootMode(struct unit *self,void *data);
void Servo_DecisionUnitRun_AutoBootMode(struct unit *self,void *data);

void Motor_DecisionUnitRun_ManualBootMode(struct unit *self,void *data);
void Servo_DecisionUnitRun_ManualBootMode(struct unit *self,void *data);

void Motor_DecisionUnitRun_DebugMode(struct unit *self,void *data);
void Servo_DecisionUnitRun_DebugMode(struct unit *self,void *data);

#endif /* APP_USR_DATAPROCESS_H_ */
