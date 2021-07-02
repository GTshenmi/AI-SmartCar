#ifndef APP_SYS_FUZZY_CTRL_H
#define APP_SYS_FIZZU_CTRL_H

#include "os.h"

typedef struct
{
    uint x;
    uint y;

}fuzzy_dim_t;

typedef struct 
{
    float *EFF;
    uint  EFFLen;

    float *DFF;
    uint  DFFLen;

    float *UFF;
    uint  UFFLen;

    float *UBuf;

    sint32_t *Rule;
}fuzzy_init_t;


typedef struct
{
    float e[2];
    float ec;
    float U;

    float *EFF;
    uint  EFFLen;

    float *DFF;
    uint  DFFLen;

    float *UFF;
    uint  UFFLen;

    float *UBuf;
    uint UBufLen;

    sint32_t *Rule;
    fuzzy_dim_t RuleDim;

}Fuzzy_TypeDef;

float FuzzyInit(Fuzzy_TypeDef *fuzzy,fuzzy_init_t *fuzzyInitStr);
float FuzzyCtrl(Fuzzy_TypeDef *fuzzy, float target, float actual);

#endif
