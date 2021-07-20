/*
 * console.h
 *
 *  Created on: 2021年1月16日
 *      Author: 936305695
 */

#ifndef OS_DEBUG_CONSOLE_H_
#define OS_DEBUG_CONSOLE_H_

#include "device.h"

#define CONSOLE_MAX_FMT_LEN 128

typedef struct
{
    void     (*Init)(void);
    uint32_t (*Write)(const char *fmt,...);
    uint32_t (*WriteLine)(const char *fmt,...);
    
    uint32_t (*WriteArray)(const char *fmt,void *array,uint32_t len);

    uint32_t (*Read)(const char *fmt,...);
    uint32_t (*ReadLine)(const char *fmt,...);

    int32_t  (*ReadKey)(void);/*不要在OS文件内使用*/

    void     (*Update)(void *argv,uint16_t argc);
}console_t;

extern console_t Console;

#endif /* OS_DEBUG_CONSOLE_H_ */
