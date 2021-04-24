/*
 * attitude.h
 *
 *  Created on: 2021��4��18��
 *      Author: 936305695
 */

#ifndef APP_USR_ATTITUDE_H_
#define APP_USR_ATTITUDE_H_

typedef struct
{
   float x;
   float y;
   float z;
}axis_t;

typedef struct
{
   float roll;  //�����
   float yaw;   //ƫ����
   float pitch; //������
}attitude_t;

void AttitudeUpdate(axis_t *acc/*input*/,axis_t *gyro/*input*/,axis_t *mag/*not use*/,attitude_t *attitude/*output*/);

#endif /* APP_USR_ATTITUDE_H_ */
