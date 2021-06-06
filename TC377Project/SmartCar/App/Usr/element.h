/*
 * element.h
 *
 *  Created on: 2020-12-06
 *      Author: 936305695
 */


#ifndef APP_USR_ELEMENT_H_
#define APP_USR_ELEMENT_H_

#include "sys.h"
#include "parameter.h"

inline bool Is_RightAngle(data_t *data)
{
    return ((data->Ke[0] >= 25.0) && (fabs(data->Ke[6] <= 5.0)))|| \
           ((data->Ke[6] >= 25.0) && (fabs(data->Ke[0] <= 5.0)));

    //return ((fabs(data->v_difference) >= 35.0 &&\
    //         fabs(data->o_difference) <= 30.0));
}

inline bool Is_RightAngleOut(data_t *data,sint32_t rightAngleCount)
{
    float h_sum = data->H_ESensorValue[0] + data->H_ESensorValue[2] + data->H_ESensorValue[1];

    return ((rightAngleCount <= 0) &&\
            (data->V_ESensorValue[0] <=10.0 && data->V_ESensorValue[1] <= 10.0) &&\
            (h_sum >= 60.0) && \
            (fabs(data->h_bias) <= 40.0));
}

inline bool Is_RightAngleBackToStraight(data_t *data)
{
    return 1;
}

inline bool Is_Cycle(data_t *data)
{
    //return 0;
    float h_sum = data->H_ESensorValue[0] + data->H_ESensorValue[2] + data->H_ESensorValue[1];

    return (h_sum >= 200);
}

inline bool Is_CycleOut(data_t *data)
{
    return (data->H_ESensorValue[1] >= 60.0);
}

inline bool Is_CycleBackToStraight(data_t *data)
{
    return (data->H_ESensorValue[1] <= 50.0);
}


inline bool Is_Cross(data_t *data)
{
    return 0;
    //return ((fabs(data->o_difference >= 65.0)) && (fabs((data->v_sum >= 80.0))) && (fabs(data->o_sum >= 100.0)));
}

inline bool Is_CrossOut(data_t *data)
{
    return ((fabs(data->o_difference >= 65.0) &&\
             fabs(data->v_sum >= 80.0)));
}


#define LoseLineGateValue 15.0

inline bool Is_LoseLine(data_t *data)
{
//    return ((data->H_ESensorValue[0] <= LoseLineGateValue) &&\
//            (data->H_ESensorValue[1] <= LoseLineGateValue) &&\
//            (data->H_ESensorValue[2] <= LoseLineGateValue) &&\
//            (data->V_ESensorValue[0] <= LoseLineGateValue) &&\
//            (data->H_ESensorValue[1] <= LoseLineGateValue));

    return 0;
}

inline bool Is_LLBiasVaild(float *eSensorValue,float bias)
{
    #define LLBiasGateValue 20.0
    return ((bias >= 5.0) &&\
           ((eSensorValue[0] >= LLBiasGateValue) || eSensorValue[1] >= LLBiasGateValue || eSensorValue[3] >= LLBiasGateValue || eSensorValue[5] >= LLBiasGateValue || eSensorValue[6] >= LLBiasGateValue)\
           );
}
inline bool Is_SearchedLine(data_t *data)
{
    return ( data->V_ESensorValue[0] <= 20.0 &&\
             data->V_ESensorValue[1] <= 20.0 &&\
            (data->H_ESensorValue[0] >= 40.0 || data->H_ESensorValue[1] >= 40.0 || data->H_ESensorValue[2] >= 40.0));
}

void SpecialElementHandler(void *argv);
float ElementDetermine(void *argv);

#endif /* APP_USR_ELEMENT_H_ */
