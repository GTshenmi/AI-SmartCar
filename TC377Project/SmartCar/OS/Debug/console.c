/*
 * console.c
 *
 *  Created on: 2021Äê1ÔÂ16ÈÕ
 *      Author: 936305695
 */
#include "console.h"
#include "device.h"
#include "os.h"

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

uint32_t Console_WriteArray(const char *fmt,void *data,uint32_t len)
{
    if(!strcmp(fmt,"float"))
    {
        float *array = (float *)data;

        Console.Write("[");

        for(int i = 0; i < len - 1 ; i++)
        {
            Console.Write("%f,",array[i]);
        }

        Console.WriteLine("%f]",array[len - 1]);
    }
    else if(!strcmp(fmt,"int"))
    {
        int *array = (int *)data;

        Console.Write("[");
        
        for(int i = 0; i < len - 1 ; i++)
        {
            Console.Write("%d,",array[i]);
        }

        Console.WriteLine("%d]",array[len - 1]);
    }

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
    Console.IO(&ch,ConsoleOut);
    return ch;
}

int fgetc(FILE *f)
{
    uint8_t ch = 0;

    Console.IO(&ch,ConsoleIn);

    return ch;
}

uint32_t Console_Log(const char *fmt,...)
{
    va_list arg;
    va_start(arg,fmt);
    
    printf("[log]:");

    vprintf(fmt,arg);

    printf("\n");

    va_end(arg);

    return 0;
}

uint32_t Console_Warn(const char *fmt,...)
{
    va_list arg;
    va_start(arg,fmt);
    
    printf("[warn]:");

    vprintf(fmt,arg);

    printf("\n");

    va_end(arg);

    return 0;
}

uint32_t Console_Error(const char *fmt,...)
{
    va_list arg;
    va_start(arg,fmt);
    
    printf("[Error]:");

    vprintf(fmt,arg);

    printf("\n");

    va_end(arg);

    return 0;
}

uint32_t Console_Info(const char *fmt,...)
{
    va_list arg;
    va_start(arg,fmt);
    
    printf("[Info]:");

    vprintf(fmt,arg);

    printf("\n");

    va_end(arg);

    return 0;
}

uint32_t Console_Time(const char *name)
{
    float time = 0.0;
    
    printf("%s:%f",name,time);

    printf("\n");

    return 0;
}

int  Console_IO(const char *ch,int io);
{
   switch(io)
   {
       case ConsoleIn:
           DebugCom.Receive(DebugCom.Self,ch,1,0);
           break;
           
       case ConsoleOut:
           DebugCom.Transmit(DebugCom.Self,(uint8_t *)ch,1,0);
           break;
   }
    
   return *ch;
}

int  File_IO(const char *ch,int io);
{
   switch(io)
   {
       case ConsoleIn:
           os.file.fastRead(".log/log.txt",ch,1);
           break;
           
       case ConsoleOut:
           os.file.fastWrite(".log/log.txt",ch);
           break;
   }
    
   return *ch;
}

int  Screen_IO(const char *ch,int io);
{
   static uint16_t x = 0,y = 0;
    
   switch(io)
   {
       case ConsoleIn:
           
           break;
           
       case ConsoleOut:
           
           Screen.ShowChar(Screen.Self,x,y,*ch);
           
           x += Screen.Font.Weight;
           
           if( x > Screen.Weight)
           {
               x = 0;
               y += Screen.Font.Hight;
           }
           
           break;
   }
    
   return *ch;
} 


void Console_SetIO(int (*io)(const char *,int))
{
    Console.IO = io;
}

console_t Console =
{
        .Init = Console_Init,
        .Write = Console_Write,
        .WriteLine = Console_WriteLine,
        .WriteArray = Console_WriteArray,
        
        .Read = Console_Read,
        .ReadLine = Console_ReadLine,
        .Update = Console_Update,
        .ReadKey = Console_ReadKey,
        .Info = Console_Info,
        .Warn = Console_Warn,
        .Error = Console_Error,
        .Log = Console_Log,
        .Time = Console_Time,
        .IO = Console_IO,
        .SetIO = Console_SetIO,
    
    
};
