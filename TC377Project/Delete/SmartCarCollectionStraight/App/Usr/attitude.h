/*
 * attitude.h
 *
 *  Created on: 2021Äê4ÔÂ18ÈÕ
 *      Author: 936305695
 */

#ifndef APP_USR_ATTITUDE_H_
#define APP_USR_ATTITUDE_H_

#include "os.h"

void AttitudeUpdate(axis_t *acc/*input*/,axis_t *gyro/*input*/,axis_t *mag/*not use*/,attitude_t *attitude/*output*/);

#endif /* APP_USR_ATTITUDE_H_ */
