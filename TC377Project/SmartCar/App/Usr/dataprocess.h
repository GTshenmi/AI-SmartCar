/*
 * dataprocess.h
 *
 *  Created on: 2020��12��6��
 *      Author: 936305695
 */

#ifndef APP_USR_DATAPROCESS_H_
#define APP_USR_DATAPROCESS_H_

#include "sys.h"

void GetESensorData(void *argv);
void ESensorDataProcess(void *argv);

float CalculateBias(void *argv);

#endif /* APP_USR_DATAPROCESS_H_ */
