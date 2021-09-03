/*
 * filter.h
 *
 *  Created on: 2021Äê1ÔÂ17ÈÕ
 *      Author: 936305695
 */

#ifndef APP_SYS_FILTER_H_
#define APP_SYS_FILTER_H_

float LowPass_FilterCalA(float dt,float Fcut);
float LowPass_Filter(float old_data,float new_data,float a);
float Average_Filter(float *data,int len);
float Silding_Filter(float *data,float new_data,float *weight,int len);
float Middle_Filter(float *data,int len);
float RemoveExtremumAverage_Filter(float *data,int len);
float IIR_Filter(float *a,float *b,float *x,float *y,float new_x,int len);
float FIR_Filter(float *b,float *x,float new_x,int len);
float Fusion_Filter(float *data,int len);
float Limiting_Filter(float data,float max,float min);


#endif /* APP_SYS_FILTER_H_ */
