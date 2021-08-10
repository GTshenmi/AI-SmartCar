/*
 * attitude.h
 *
 *  Created on: 2021Äê4ÔÂ18ÈÕ
 *      Author: 936305695
 */

#ifndef APP_USR_ATTITUDE_H_
#define APP_USR_ATTITUDE_H_

#include "os.h"

//extern short turns=0;
//extern float newdata=0.0f,olddata=0.0f;
//extern float pitchoffset,rolloffset,yawoffset;
void IMU_Update_Init(void);
void IMU_Update0(float gx, float gy, float gz, float ax, float ay, float az,float mx,float my,float mz,float *roll,float *pitch,float *yaw);

#endif /* APP_USR_ATTITUDE_H_ */
