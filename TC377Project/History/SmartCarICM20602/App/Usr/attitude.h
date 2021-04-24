/*
 * attitude.h
 *
 *  Created on: 2021Äê4ÔÂ18ÈÕ
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
   float roll;  //ºá¹ö½Ç
   float yaw;   //Æ«º½½Ç
   float pitch; //¸©Ñö½Ç
}attitude_t;

void AttitudeUpdate(axis_t *acc/*input*/,axis_t *gyro/*input*/,axis_t *mag/*not use*/,attitude_t *attitude/*output*/);

#endif /* APP_USR_ATTITUDE_H_ */
