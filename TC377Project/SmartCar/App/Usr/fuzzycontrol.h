#ifndef APP_USR_FUZZYCONTROL_H
#define APP_USR_FUZZYCONTROL_H

#include "sys.h"
// #define NL -3.0
// #define NM -2.0
// #define NS -1.0
// #define ZE 0.0
// #define PS 1.0
// #define PM 2.0
// #define PL 3.0

typedef struct
{
    float e[2];
    float ec;
    float U;
}fuzzy_ctrl_t;

float FuzzyControl(fuzzy_ctrl_t* fuzzy, float target, float actual);
float FuzzySpeedControl(Fuzzy_TypeDef *fuzzy,float target,float actual);
float FuzzyPIDInit(Fuzzy_TypeDef *pFuzzy,Fuzzy_TypeDef *iFuzzy);
float FuzzyPID(Fuzzy_TypeDef *pFuzzy,Fuzzy_TypeDef *iFuzzy,float target,float actual);

extern sint32_t LABMSFuzzyRule[7][7];

extern sint32_t SABMSFuzzyRule[7][7];

extern sint32_t SFuzzyRule[7][7];

#endif
