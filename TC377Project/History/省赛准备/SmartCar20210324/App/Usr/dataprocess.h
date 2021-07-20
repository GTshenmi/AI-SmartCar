/*
 * dataprocess.h
 *
 *  Created on: 2020Äê12ÔÂ6ÈÕ
 *      Author: 936305695
 */

#ifndef APP_USR_DATAPROCESS_H_
#define APP_USR_DATAPROCESS_H_

#include "sys.h"

void Motor_SensorUnitRun(struct unit *self,void *argv,uint16_t argc);
void Servo_SensorUnitRun(struct unit *self,void *argv,uint16_t argc);

void Motor_DecisionUnitRun_AIMode(struct unit *self,void *argv,uint16_t argc);
void Servo_DecisionUnitRun_AIMode(struct unit *self,void *argv,uint16_t argc);

void Motor_DecisionUnitRun_AutoBootMode(struct unit *self,void *argv,uint16_t argc);
void Servo_DecisionUnitRun_AutoBootMode(struct unit *self,void *argv,uint16_t argc);

void Motor_DecisionUnitRun_ManualBootMode(struct unit *self,void *argv,uint16_t argc);
void Servo_DecisionUnitRun_ManualBootMode(struct unit *self,void *argv,uint16_t argc);

void Motor_DecisionUnitRun_DebugMode(struct unit *self,void *argv,uint16_t argc);
void Servo_DecisionUnitRun_DebugMode(struct unit *self,void *argv,uint16_t argc);

float CalculateDistance(void *argv);

#endif /* APP_USR_DATAPROCESS_H_ */
