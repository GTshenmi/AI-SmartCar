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

inline bool Is_Straight(data_t *data)
{
    return (data->h_bias <= 20.0 && data->v_sum <= 10.0);
}

inline bool Is_RightAngle(data_t *data)
{
//    return ((data->Ke[0] >= 25.0) && (fabs(data->Ke[6] <= 5.0)))|| \
//           ((data->Ke[6] >= 25.0) && (fabs(data->Ke[0] <= 5.0)));
    if(data->CarMode == LAutoBoot_Mode)
    {
        return ((fabs(data->v_difference) >= 30.0) and (fabs(data->o_difference) <= 40.0) and (fabs(data->v_difference/data->h_difference) >= 1.5))||\
                ((fabs(data->v_sum) >= 20.0) and (data->v_bias >= 80.0) and (data->h_bias <= 10.0) && data->h_sum <= 70.0);
    }
    else
    {
        return  ((fabs(data->v_difference) >= 25.0) and (fabs(data->o_difference) <= 30.0))||\
                ((fabs(data->v_sum) >= 20.0) and (data->v_bias >= 80.0) and (data->h_bias <= 20.0));
    }
}

inline bool Is_RightAngleOut(data_t *data,sint32_t rightAngleCount)
{
    //float h_sum = data->H_ESensorValue[0] + data->H_ESensorValue[2] + data->H_ESensorValue[1];

    //(data->V_ESensorValue[0] <=10.0 && data->V_ESensorValue[1] <= 10.0) &&

    if(data->CarMode == LAutoBoot_Mode)
    {
        return ((rightAngleCount <= 0)) && ((data->H_ESensorValue[0] >= 35.0) || (data->H_ESensorValue[2] >= 35.0));
                   //(h_sum >= 45.0) && );
                   //(fabs(data->h_bias) <= 60.0));
    }
    else
    {
        return ((rightAngleCount <= 0)) && ((data->H_ESensorValue[0] >= 35.0) || (data->H_ESensorValue[3] >= 35.0));
    }

}

inline bool Is_RightAngleBackToStraight(data_t *data)
{
    //float sum_h = data->H_ESensorValue[0] + data->H_ESensorValue[2] + data->H_ESensorValue[1];

    //return (sum_h >= 100.0);

    return 1;
}

inline bool Is_Cycle(data_t *data)
{
    //return 0;


    float h_sum = 0.0;

    if(data->CarMode == LAutoBoot_Mode)
        h_sum = data->H_ESensorValue[0] + data->H_ESensorValue[1] + data->H_ESensorValue[2];
    else
        h_sum = data->H_ESensorValue[0] + data->H_ESensorValue[3] + data->H_ESensorValue[4];

    return (h_sum >= 200.0);
}

inline bool Is_CycleOut(data_t *data,uint32_t cycleOutCnt)
{

    float h_sum = 0.0;

    if(data->CarMode == LAutoBoot_Mode)
        h_sum = data->H_ESensorValue[0] + data->H_ESensorValue[1] + data->H_ESensorValue[2];
    else
        h_sum = data->H_ESensorValue[0] + data->H_ESensorValue[3] + data->H_ESensorValue[4];

    return (h_sum >= 200.0 && cycleOutCnt >= 1000);
}

inline bool Is_CycleBackToStraight(data_t *data)
{
    float h_sum = 0.0;

    if(data->CarMode == LAutoBoot_Mode)
    {
        h_sum = data->H_ESensorValue[0] + data->H_ESensorValue[1] + data->H_ESensorValue[2];
        return (h_sum <= 150.0 && (data->H_ESensorValue[0] <=50.0 && data->H_ESensorValue[2] <= 50.0));
    }
    else
    {
        h_sum = data->H_ESensorValue[0] + data->H_ESensorValue[3] + data->H_ESensorValue[4];
        return (h_sum <= 150.0 && (data->H_ESensorValue[0] <=50.0 && data->H_ESensorValue[3] <= 50.0));
    }



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

#define LoseLineGateValue 10.0

inline bool Is_LoseLine(data_t *data)
{
    return ((data->H_ESensorValue[0] <= LoseLineGateValue) &&\
            (data->H_ESensorValue[1] <= LoseLineGateValue) &&\
            (data->H_ESensorValue[2] <= LoseLineGateValue) &&\
            (data->V_ESensorValue[0] <= LoseLineGateValue) &&\
            (data->V_ESensorValue[1] <= LoseLineGateValue) &&\
            (data->O_ESensorValue[0] <= LoseLineGateValue) &&\
            (data->O_ESensorValue[1] <= LoseLineGateValue) &&\
            (data->H_ESensorValue[3] <= LoseLineGateValue));

    //return 0;
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
    return ((data->H_ESensorValue[0] >= LoseLineGateValue) ||\
            (data->H_ESensorValue[1] >= LoseLineGateValue) ||\
            (data->H_ESensorValue[2] >= LoseLineGateValue) ||\
            (data->V_ESensorValue[0] >= LoseLineGateValue) ||\
            (data->V_ESensorValue[1] >= LoseLineGateValue) ||\
            (data->H_ESensorValue[3] >= LoseLineGateValue));
}

void SpecialElementHandler(void *argv);
float ElementDetermine(void *argv);

#endif /* APP_USR_ELEMENT_H_ */
