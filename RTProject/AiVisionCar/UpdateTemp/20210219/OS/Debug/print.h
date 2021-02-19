/*
 * print.h
 *
 *  Created on: 2021Äê1ÔÂ2ÈÕ
 *      Author: 936305695
 */

#ifndef OS_DEBUG_PRINT_H_
#define OS_DEBUG_PRINT_H_

#include "device.h"

typedef struct
{
    bool b;
    char c;
    char *s;
    int i;
    long l;

    float f;
    double d;

    unsigned char uc;
    unsigned int ui;
    unsigned long ul;
}print_var_t;

extern print_var_t var;

#define typecmp(x,y) (!strcmp((getTypeName(x)),y))

#define printb(var)\
{                                           \
    printf("%s = %s\n",(#var),(var?"true":"false"));\
}

#define printc(var)\
{                                           \
    printf("%s = %c\n",(#var),(var));\
}

#define prints(var)\
{                                           \
    printf("%s = %s\n",(#var),(var));\
}

#define printi(var)                   \
{                                           \
    printf("%s = %d\n",(#var),(var));\
}

#define printu(var)\
{                                           \
    printf("%s = %u\n",(#var),(var));\
}

#define printd(var)\
{                                           \
    printf("%s = %f\n",(#var),(var));\
}

#define _print(var,type)\
{\
    switch(type)\
    {\
       case 'd':\
           printi(var);\
           break;\
       case 's':\
           prints(var);\
           break;\
       case 'u':\
           printu(var);\
           break;\
       case 'b':\
           printb(var);\
           break;\
       case 'c':\
           printc(var);\
           break;\
       case 'f':\
           printd(var);\
           break;\
       default:\
           printd(var * 1.0);\
           break;\
    }\
}

#define print _print

#define uprint

#endif /* OS_DEBUG_PRINT_H_ */
