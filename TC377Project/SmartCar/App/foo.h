/*
 * foo.h
 *
 *  Created on: 2021Äê1ÔÂ22ÈÕ
 *      Author: 936305695
 */

#ifndef APP_FOO_H_
#define APP_FOO_H_

#include "os.h"
#include "parameter.h"

void DataSaveSysInit(char * excel_filename,char *txt_filename);
void DataSaveSysDeInit(FIL *excel_fp,FIL *txt_fp);
void SaveAutoBootModeDataToExcel(void *data);
void ReadParameterFromSDCard(void *data);
void SaveParameterToSDCard(void *data);
uint SaveParameterSD(data_t *data,float *LADC_Value,float *SADC_Value,float *Angle);
uint SaveMotorSystemInfo(float *input,float *output,uint32_t len);
uint SaveSensorDataAndAngle(data_t *data,float *LADC_Value,float *SADC_Value,float *Angle,char *name);
#endif /* APP_FOO_H_ */
