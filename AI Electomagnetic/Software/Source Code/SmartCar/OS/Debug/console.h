/*
 * console.h
 *
 *  Created on: 2021骞�1鏈�16鏃�
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

    int32_t  (*ReadKey)(void);/*涓嶈鍦∣S鏂囦欢鍐呬娇鐢�*/
    
    uint32_t (*Log)(const char *fmt,...);
    
    uint32_t (*Warn)(const char *fmt,...);
    
    uint32_t (*Error)(const char *fmt,...);
    
    float (*Time)(const char *name);
    
    uint32_t (*Info)(const char *fmt,...);
    
    int      (*IO)(char *ch,int io);
    
    void     (*SetIO)(int (*io)(char *,int));

    void     (*Update)(void *argv,uint16_t argc);
}console_t;

extern console_t Console;

int  Screen_IO(char *ch,int io);
int  File_IO(char *ch,int io);
int  Console_IO(char *ch,int io);

#endif /* OS_DEBUG_CONSOLE_H_ */
