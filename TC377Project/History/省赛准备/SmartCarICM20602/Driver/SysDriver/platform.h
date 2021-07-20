/*
 * chipdatatype.h
 *
 *  Created on: 2020Äê12ÔÂ26ÈÕ
 *      Author: 936305695
 */

#ifndef DRIVER_SYSDRIVER_PLATFORM_H_
#define DRIVER_SYSDRIVER_PLATFORM_H_

#include "stdint.h"
#include "stdbool.h"

#include "math.h"
#include "stdio.h"
#include "string.h"
#include <stdarg.h>
#include "stdlib.h"

/*Debug Mode: #define Debug.*/
//#define Debug

#define TC264 0
#define TC377 1
#define RT1064 2
#define Other -1

#define Chip TC377

#define and &&
#define or ||
#define public /*public*/
#define private /*private*/

#define pass

typedef char * string;

#ifndef uint8_t
#define uint8_t unsigned char
#endif

#ifndef uint16_t
#define uint16_t unsigned short
#endif

#ifndef uint32_t
#define uint32_t unsigned long
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
#define sint32_t long
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

#if defined(Chip) && (Chip == TC264 || Chip == TC377)
#define Cpu_AcquireMutex(lock) IfxCpu_acquireMutex(lock)
#define Cpu_ReleaseMutex(lock) IfxCpu_releaseMutex(lock)
#define Cpu_LockMutex(lock) IfxCpu_setSpinLock((lock),1)
#define Cpu_MutexLock IfxCpu_mutexLock

#define Cpu_AcquireAndLockMutex(lock)     \
    while(!Cpu_AcquireMutex(lock));\
    //Cpu_LockMutex(lock);
#endif



#endif /* DRIVER_SYSDRIVER_PLATFORM_H_ */
