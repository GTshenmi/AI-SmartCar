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

#define printbool(var)\
{                                           \
    printf("%s = %s\n",(#var),(var?"true":"false"));\
}

#define printchar(var)\
{                                           \
    printf("%s = %c\n",(#var),(var));\
}

#define printstring(var)\
{                                           \
    printf("%s = %s\n",(#var),(var));\
}

#define printint(var)                   \
{                                           \
    printf("%s = %d\n",(#var),(var));\
}

#define printunsigned(var)\
{                                           \
    printf("%s = %u\n",(#var),(var));\
}

#define printdouble(var)\
{                                           \
    printf("%s = %f\n",(#var),(var));\
}

#define _print(var,type)\
{\
    char vartype = type;\
    switch(type)\
    {\
       case 'd':\
           printint(var);\
           break;\
       case 's':\
           printstring(var);\
           break;\
       case 'u':\
           printunsigned(var);\
           break;\
       case 'b':\
           printbool(var);\
           break;\
       case 'c':\
           printchar(var);\
           break;\
       case 'f':\
           printdouble(var);\
           break;\
       default:\
           printdouble(var * 1.0);\
           break;\
    }\
}

#define print _print

#define uprint

#endif /* OS_DEBUG_PRINT_H_ */
