#ifndef APP_USR_FUZZYCONTROL_H
#define APP_USR_FUZZYCONTROL_H

#include "sys.h"

float FuzzyControl(Fuzzy_TypeDef* fuzzy, float target, float actual);
float FuzzySpeedControl(Fuzzy_TypeDef *fuzzy,float target,float actual);
float FuzzyPIDInit(Fuzzy_TypeDef *pFuzzy,Fuzzy_TypeDef *iFuzzy);
float FuzzyPIDCtrl(Fuzzy_TypeDef *pFuzzy,Fuzzy_TypeDef *iFuzzy,float target,float actual);

extern sint32_t LABMSFuzzyRule[7][7];

extern sint32_t SABMSFuzzyRule[7][7];

extern sint32_t SFuzzyRule[7][7];

#endif
