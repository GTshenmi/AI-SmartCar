/*
 * eeprom.h
 *
 *  Created on: 2021Äê4ÔÂ2ÈÕ
 *      Author: 936305695
 */

#ifndef DRIVER_SYSDRIVER_EEPROM_H_
#define DRIVER_SYSDRIVER_EEPROM_H_

#include "sys_driver.h"

uint8_t EEPROM_WriteSector(uint32_t sector, uint32_t page, uint8_t * buff, uint32_t len);
uint8_t EEPROM_ReadSector(uint32_t sector, uint32_t page, uint8_t * buff, uint32_t len);
uint32_t EEPROM_GetSectorNum(void);

#endif /* DRIVER_SYSDRIVER_EEPROM_H_ */
