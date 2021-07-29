/*
 * calibration.h
 *
 *  Created on: 2021Äê7ÔÂ29ÈÕ
 *      Author: 936305695
 */

#ifndef APP_USR_CALIBRATION_H_
#define APP_USR_CALIBRATION_H_

#include "sys.h"

void CalibrationVESensor(uint16_t esensor,uint16_t index);
void CalibrationHOESensor(uint16_t esensor);

void SelfCalibration(void *argv,uint16_t object);


#endif /* APP_USR_CALIBRATION_H_ */
