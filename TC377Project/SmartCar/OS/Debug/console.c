/*
 * console.c
 *
 *  Created on: 2021Äê1ÔÂ16ÈÕ
 *      Author: 936305695
 */
#include "console.h"
#include "driver.h"

void Console_Init(void)
{
    DebugCom.Init(DebugCom.Self);
}

void Console_Update(void *argv,uint16_t argc)
{

}

uint32_t Console_Write(const char *fmt,...)
{
    va_list arg;
    va_start(arg,fmt);

    vprintf(fmt,arg);

    va_end(arg);

    return 0;
}

uint32_t Console_WriteLine(const char *fmt,...)
{
    va_list arg;
    va_start(arg,fmt);

    vprintf(fmt,arg);

    printf("\n");

    va_end(arg);

    return 0;
}

uint32_t Console_Read(const char *fmt,...)
{
    va_list arg;
    va_start(arg,fmt);

    char readfmt[CONSOLE_MAX_FMT_LEN];

    char *p = (char *)fmt;

    uint32_t len = 0;

    for(; len < CONSOLE_MAX_FMT_LEN - 10 ; len++)
    {
        readfmt[len] = *p;
        p++;
        if(*p == '\0')
            break;
    }

    if(len >= CONSOLE_MAX_FMT_LEN - 10)
    {
        readfmt[CONSOLE_MAX_FMT_LEN - 10] = '\0';
    }
    else
        readfmt[++len] = '\0';

    strcat(readfmt,"%[^ ]");

    len = vscanf(fmt,arg);

    va_end(arg);

    return len;
}

uint32_t Console_ReadLine(const char *fmt,...)
{
    va_list arg;
    va_start(arg,fmt);

    char readfmt[CONSOLE_MAX_FMT_LEN];

    char *p = (char *)fmt;

    uint32_t len = 0;

    for(; len < CONSOLE_MAX_FMT_LEN - 10 ; len++)
    {
        readfmt[len] = *p;
        p++;
        if(*p == '\0')
            break;
    }

    if(len >= CONSOLE_MAX_FMT_LEN - 10)
    {
        readfmt[CONSOLE_MAX_FMT_LEN - 10] = '\0';
    }
    else
        readfmt[++len] = '\0';

    strcat(readfmt,"%[^\n]");

    len = vscanf(fmt,arg);

    va_end(arg);

    return len;
}

int32_t Console_ReadKey(void)
{
    int32_t index = -1;

    while(1)
    {
        for(int32_t i = 0 ; i < 4 ; i++)
        {
#if 0
            KEY[i].Scan(KEY[i].Self);
#endif
            if(KEY[i].GetEvent(KEY[i].Self) == KEY_DOWN)
            {
                index = i ;
                goto ConsoleReadKeyEnd;
            }
        }
    }

    ConsoleReadKeyEnd:

    return index;
}



int fputc(int ch, FILE *f)
{
//    uint8_t c = (uint8_t)ch;

    //DebugCom.Transmit(DebugCom.Self,&c,1,0);

    UARTx.WriteByte(DebugCom.Communicatorn,(uint8_t)ch,UARTx.Time_Infinite);

    return ch;
}

int fgetc(FILE *f)
{
    uint8_t ch = 0;

    DebugCom.Receive(DebugCom.Self,&ch,1,0);

    return ch;
}

console_t Console =
{
        .Init = Console_Init,
        .Write = Console_Write,
        .WriteLine = Console_WriteLine,
        .Read = Console_Read,
        .ReadLine = Console_ReadLine,
        .Update = Console_Update,
        .ReadKey = Console_ReadKey,
};
