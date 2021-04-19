#ifndef APP_USR_FUZZYCONTROL_H
#define APP_USR_FUZZYCONTROL_H

#include "sys.h"

#define NL -3.0
#define NM -2.0
#define NS -1.0
#define ZE 0.0
#define PS 1.0
#define PM 2.0
#define PL 3.0

extern float fuzzyRuleKp[7][7];
extern float fuzzyRuleKi[7][7];
extern float fuzzyRuleKd[7][7];

pid_ctrl_t fuzzy(float e,float ec);


#endif