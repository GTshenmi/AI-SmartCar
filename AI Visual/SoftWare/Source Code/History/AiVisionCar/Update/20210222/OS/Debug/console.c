/*
 * console.c
 *
 *  Created on: 2021��1��16��
 *      Author: 936305695
 */
#include "console.h"

#define ConsoleAscLin UART_Resources[0].UARTN

void Console_Init(void)
{
    UARTx.Init(&ConsoleAscLin);
}

void Console_Updata(void *argv,uint16_t argc)
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
    UARTx.WriteByte(&ConsoleAscLin,(uint8_t)ch,UARTx.Time_Infinite);

    return ch;
}

int fgetc(FILE *f)
{
    int ch = 0;

    ch = UARTx.ReadByte(&ConsoleAscLin,UARTx.Time_Infinite);

    return ch;
}

console_t Console =
{
        .Init = Console_Init,
        .Write = Console_Write,
        .WriteLine = Console_WriteLine,
        .Read = Console_Read,
        .ReadLine = Console_ReadLine,
        .Update = Console_Updata,
        .ReadKey = Console_ReadKey,
};