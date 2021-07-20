/*
 * icm20602.h
 *
 *  Created on: 2021��4��22��
 *      Author: 936305695
 */

#ifndef DRIVER_USRDRIVER_ICM20602_ICM20602_H_
#define DRIVER_USRDRIVER_ICM20602_ICM20602_H_

#include "sys_driver.h"

/*��ʼ��*/
uint8_t ICM20602_Init(void *config);

/*��ȡ������ٶ�*/
uint8_t ICM20602_ReadAcc(float *x,float *y,float *z);

/*��ȡ����Ƕ�*/
uint8_t ICM20602_ReadGyro(float *x,float *y,float *z);

#endif /* DRIVER_USRDRIVER_ICM20602_ICM20602_H_ */
