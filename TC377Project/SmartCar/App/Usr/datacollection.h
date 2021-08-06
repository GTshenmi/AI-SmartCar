/*
 * datacollection.h
 *
 *  Created on: 2021Äê7ÔÂ21ÈÕ
 *      Author: 936305695
 */

#ifndef APP_USR_DATACOLLECTION_H_
#define APP_USR_DATACOLLECTION_H_


#include "sys.h"

void HowToNameThisFunc(void *argv);
uint SaveParameterSD(void *argv,float *LADC_Value,float *SADC_Value,float *Angle);
uint SaveMotorSystemInfo(float *input,float *output,uint32_t len);
uint SaveSensorDataAndAngle(void *argv,char *path);
uint SaveSensorDataAndAngleAI(void *argv,char *path);

#endif /* APP_USR_DATACOLLECTION_H_ */
