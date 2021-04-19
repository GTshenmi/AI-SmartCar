#ifndef APP_SYS_FUZZY_CTRL_H
#define APP_SYS_FIZZU_CTRL_H

#include "os.h"


typedef double (*quantization_func)(double input);

typedef struct
{
    uint32_t x;
    uint32_t y;
}fuzzy_dim_t;

typedef struct
{
    double input;
    double output;
}fuzzy_gain_t;


typedef struct
{
    double Input;

    uint32_t QuantifyDim;      /**/

    quantization_func Quantify;

    sint32_t Q;

    sint32_t Pn;

    double IF[2];

    double *IFF;

    double U;

    double *UFF;
    
    double *Rule;           /*模糊规则表*/

    fuzzy_dim_t RuleDim;

    fuzzy_gain_t Gain;

    double Result;
}fuzzy_ctrl_t;



#endif