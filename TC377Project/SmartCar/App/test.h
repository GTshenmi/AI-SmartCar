/*
 * test.h
 *
 *  Created on: 2021��3��9��
 *      Author: 936305695
 */

#ifndef APP_TEST_H_
#define APP_TEST_H_

#include "device.h"

void MotorTest(struct motor_ctrl *self);
void ServoTest(struct servo_ctrl *self);

void NNTest(void);

#endif /* APP_TEST_H_ */
