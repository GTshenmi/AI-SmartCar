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

enum
{
    ConsoleIn,
    ConsoleOut,
};

typedef struct
{
    void     (*Init)(void);
    uint32_t (*Write)(const char *fmt,...);
    uint32_t (*WriteLine)(const char *fmt,...);
    
    uint32_t (*WriteArray)(const char *fmt,void *array,uint32_t len);

    uint32_t (*Read)(const char *fmt,...);
    uint32_t (*ReadLine)(const char *fmt,...);

    int32_t  (*ReadKey)(void);/*不要在OS文件内使用*/
    
    uint32_t (*Log)(const char *fmt,...);
    
    uint32_t (*Warn)(const char *fmt,...);
    
    uint32_t (*Error)(const char *fmt,...);
    
    float (*Time)(const char *name);
    
    uint32_t (*Info)(const char *fmt,...);
    
    int      (*IO)(const char *ch,int io);
    
    void     (*SetIO)(int (*io)(const char *,int));  

    void     (*Update)(void *argv,uint16_t argc);
}console_t;

extern console_t Console;

int  Screen_IO(const char *ch,int io);
int  File_IO(const char *ch,int io);
int  Console_IO(const char *ch,int io);

//void ErrorMsg(const char *fmt,...);

#endif /* OS_DEBUG_CONSOLE_H_ */
