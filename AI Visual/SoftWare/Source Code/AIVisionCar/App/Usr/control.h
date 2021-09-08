/*
 * control.h
 *
 *  Created on: 2020年12月6日
 *      Author: 936305695
 */

#ifndef APP_USR_CONTROL_H_
#define APP_USR_CONTROL_H_

#include "sys.h"

void ServoControl(void);
void MotorControl(int LSpeed,int RSpeed);
void SpeedControl(void);


extern long ServoError;
extern float Servo_KpInc;
extern int ServoAngle;
extern float Servo_KpAll;
extern bool Car_Go;
extern int LCurrentSpeed;              //本次左电机速度
extern int RCurrentSpeed;              //本次右电机速度



#endif /* APP_USR_CONTROL_H_ */
