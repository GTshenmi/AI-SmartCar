/*
 * foo.h
 *
 *  Created on: 2021Äê1ÔÂ22ÈÕ
 *      Author: 936305695
 */

#ifndef APP_FOO_H_
#define APP_FOO_H_

#include "os.h"

void DataSaveSysInit(char * excel_filename,char *txt_filename);
void DataSaveSysDeInit(FIL *excel_fp,FIL *txt_fp);
void SaveAutoBootModeDataToExcel(void *data);
void ReadParameterFromSDCard(void *data);
void SaveParameterToSDCard(void *data);

#endif /* APP_FOO_H_ */
