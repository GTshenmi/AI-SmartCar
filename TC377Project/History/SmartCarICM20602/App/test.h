/*
 * test.h
 *
 *  Created on: 2021Äê3ÔÂ9ÈÕ
 *      Author: 936305695
 */

#ifndef APP_TEST_H_
#define APP_TEST_H_

#include "device.h"

void SPIx_Test(spix_t *spi);

void MotorTest(struct motor_ctrl *self);
void ServoTest(struct servo_ctrl *self);

void NN_Test(void);
void NNCU_Test(void);

#endif /* APP_TEST_H_ */
