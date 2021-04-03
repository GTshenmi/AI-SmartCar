/*
 * eeprom.h
 *
 *  Created on: 2021Äê4ÔÂ2ÈÕ
 *      Author: 936305695
 */

#ifndef DRIVER_SYSDRIVER_EEPROM_H_
#define DRIVER_SYSDRIVER_EEPROM_H_

#include "platform.h"
#include "common.h"

uint8_t EEPROM_WriteSector(unsigned char sector, unsigned short page, uint8_t * buff, unsigned short len);
uint8_t EEPROM_ReadSector(unsigned char sector, unsigned short page, uint8_t * buff, uint32_t len);


#endif /* DRIVER_SYSDRIVER_EEPROM_H_ */
