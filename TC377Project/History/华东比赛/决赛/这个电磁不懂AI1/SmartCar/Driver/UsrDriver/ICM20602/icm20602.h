/*
 * icm20602.h
 *
 *  Created on: 2021年4月22日
 *      Author: 936305695
 */

#ifndef DRIVER_USRDRIVER_ICM20602_ICM20602_H_
#define DRIVER_USRDRIVER_ICM20602_ICM20602_H_

#include "sys_driver.h"

/*初始化*/
uint8_t ICM20602_Init(void *config);

/*读取三轴加速度*/
uint8_t ICM20602_ReadAcc(float *x,float *y,float *z);

/*读取三轴角度*/
uint8_t ICM20602_ReadGyro(float *x,float *y,float *z);

#endif /* DRIVER_USRDRIVER_ICM20602_ICM20602_H_ */
