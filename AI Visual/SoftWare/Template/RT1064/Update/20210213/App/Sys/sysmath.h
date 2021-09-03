/*
 * sysmath.h
 *
 *  Created on: 2021Äê1ÔÂ26ÈÕ
 *      Author: 936305695
 */

#ifndef APP_SYS_SYSMATH_H_
#define APP_SYS_SYSMATH_H_

#include "chipdatatype.h"

#define fsign(x) ((x) > 0.0 ?  1.0 : -1.0 )

float NormalizeFloat(float value,float min,float max);
float CalculateDistanceDifDivSum(float L1,float L2);
uint16_t FindMaxIndex(float *array,uint16_t len);
float ConstrainFloat(float value,float min,float max);
float random(float lower,float upper);
float frand(void);

#endif /* APP_SYS_SYSMATH_H_ */
