/*
 * debug.h
 *
 *  Created on: 2021Äê2ÔÂ23ÈÕ
 *      Author: 936305695
 */

#ifndef OS_DEBUG_DEBUG_H_
#define OS_DEBUG_DEBUG_H_


#include "print.h"
#include "console.h"
#include "ano_dt.h"

#ifdef Debug

#define DebugCopy(dst,src) (dst) = (src)
#define DebugDefine(type,param) type param
#define DebugPrint(format,__VA_ARGS__) Console.WriteLine(format,__VA_ARGS__)

#define DebugBeepOn BEEP.ON(BEEP.Self)
#define DebugBeepOff BEEP.OFF(BEEP.Self)

#else

#define DebugCopy(x,y)
#define DebugDefine(type,param)
#define DebugPrint(format,VA_ARGS)

#endif



#endif /* OS_DEBUG_DEBUG_H_ */
