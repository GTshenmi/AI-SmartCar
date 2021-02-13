/*
 * chipdatatype.h
 *
 *  Created on: 2020Äê12ÔÂ26ÈÕ
 *      Author: 936305695
 */

#ifndef DRIVER_SYSDRIVER_CHIPDATATYPE_H_
#define DRIVER_SYSDRIVER_CHIPDATATYPE_H_

#include "stdint.h"

#ifndef uint8_t
#define uint8_t unsigned char
#endif

#ifndef uint16_t
#define uint16_t unsigned short
#endif

#ifndef uint32_t
#define uint32_t unsigned int
#endif

#ifndef uint64_t
#define uint64_t unsigned long long
#endif

#ifndef sint8_t
#define sint8_t char
#endif

#ifndef sint16_t
#define sint16_t short
#endif

#ifndef sint32_t
#define sint32_t int
#endif

#ifndef sint64_t
#define sint64_t long long
#endif

#ifndef int8_t
#define int8_t sint8_t
#endif

#ifndef int16_t
#define int16_t sint16_t
#endif

#ifndef int32_t
#define int32_t sint32_t
#endif

#ifndef int64_t
#define int64_t sint64_t
#endif

#ifndef uint
#define uint unsigned int
#endif

#ifndef uintMax
#define uintMax UINT32_MAX
#endif

#endif /* DRIVER_SYSDRIVER_CHIPDATATYPE_H_ */
