/*
 * sysmath.h
 *
 *  Created on: 2021��1��26��
 *      Author: 936305695
 */

#ifndef APP_SYS_SYSMATH_H_
#define APP_SYS_SYSMATH_H_

#include "platform.h"

#define Zero 1e-6

#define INFINITE 0xFFFFFFFF

#define Is_Zero(x) ((fabs(x)) < Zero)

#define fsign(x) ((x) > 0.0 ?  1.0 : -1.0 )

#define max(a,b) ((a) > (b) ? (a) : (b))

#define min(a,b) ((a) < (b) ? (a) : (b))

float NormalizeFloat(float value,float min,float max);
float CalculateDistanceDifDivSum(float L1,float L2);
uint16_t FindMaxIndex(float *array,uint16_t len);
float ConstrainFloat(float value,float min,float max);
float random(float lower,float upper);
float frand(void);
double Sigmoid(double input);
double Tanh(double input);
double Rule(double input);
double Rule8(double input);


#endif /* APP_SYS_SYSMATH_H_ */