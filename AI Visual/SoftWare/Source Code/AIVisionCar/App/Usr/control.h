/*
 * control.h
 *
 *  Created on: 2020��12��6��
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
extern int LCurrentSpeed;              //���������ٶ�
extern int RCurrentSpeed;              //�����ҵ���ٶ�



#endif /* APP_USR_CONTROL_H_ */
