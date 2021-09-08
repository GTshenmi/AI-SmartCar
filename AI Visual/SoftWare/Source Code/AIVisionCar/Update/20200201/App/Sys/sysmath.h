/*
 * sysmath.h
 *
 *  Created on: 2021Äê1ÔÂ26ÈÕ
 *      Author: 936305695
 */

#ifndef APP_SYS_SYSMATH_H_
#define APP_SYS_SYSMATH_H_


#define fsign(x) ((x) > 0.0 ?  1.0 : -1.0 )

float NormalizeFloat(float value,float max,float min);
float CalculateDistanceDifDivSum(float L1,float L2);


#endif /* APP_SYS_SYSMATH_H_ */
