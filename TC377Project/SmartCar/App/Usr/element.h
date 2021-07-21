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

inline bool Is_Boundary(data_t *data)
{
#define BoundaryGate 10.0

   uint8_t is_boundary = 0;

   is_boundary += ((data->H_ESensorValue[0] >= BoundaryGate) &&\
            (data->H_ESensorValue[1] <= BoundaryGate) &&\
            (data->H_ESensorValue[2] <= BoundaryGate) &&\
            (data->V_ESensorValue[0] <= BoundaryGate) &&\
            (data->V_ESensorValue[1] <= BoundaryGate) &&\
            (data->O_ESensorValue[0] <= BoundaryGate) &&\
            (data->O_ESensorValue[1] <= BoundaryGate) &&\
            (data->H_ESensorValue[3] <= BoundaryGate));

   is_boundary += ((data->H_ESensorValue[0] <= BoundaryGate) &&\
           (data->H_ESensorValue[1] <= BoundaryGate) &&\
           (data->H_ESensorValue[2] <= BoundaryGate) &&\
           (data->V_ESensorValue[0] <= BoundaryGate) &&\
           (data->V_ESensorValue[1] <= BoundaryGate) &&\
           (data->O_ESensorValue[0] <= BoundaryGate) &&\
           (data->O_ESensorValue[1] <= BoundaryGate) &&\
           (data->H_ESensorValue[3] >= BoundaryGate));

   return is_boundary;

}
/*ÅÐ¶ÏÆÂµÀ*/
inline bool Is_Ramp(data_t *data)
{
    //float distance = DSensor.GetDistance(DSensor.Self);

    //distance = distance * 1.0;

    return 0;
}

inline bool Is_Straight(data_t *data)
{
    return (fabs(data->h_bias) <= 20.0 && data->v_sum <= 10.0 && data->h_sum >= 50.0);
}

inline bool Is_Corner(data_t *data)
{
    return (fabs(data->h_bias) >= 20.0 && data->h_sum >= 60.0 && data->v_sum >= 10.0);
}

inline bool Is_RightAngle(data_t *data)
{
    if(data->CarMode == LAutoBoot_Mode || data->CarMode == AI_Mode)
    {
        return ((fabs(data->v_difference) >= 30.0) and (fabs(data->o_difference) <= 40.0) and (fabs(data->v_difference/data->h_difference) >= 5.0) and (fabs(data->h_sum) <= 100.0));
    }
    else
    {
        return ((fabs(data->v_difference) >= 30.0) and (fabs(data->o_difference) <= 40.0) and (fabs(data->v_difference/data->h_difference) >= 5.0) and (fabs(data->h_sum) <= 100.0));
    }
}

inline bool Is_RightAngleOut(data_t *data,sint32_t rightAngleTrackingCount)
{

    if(data->CarMode == LAutoBoot_Mode || data->CarMode == AI_Mode)
    {
        return ((((rightAngleTrackingCount <= 0)) && ((data->H_ESensorValue[0] >= 30.0) || (data->H_ESensorValue[2] >= 30.0) || (data->H_ESensorValue[1] >= 30.0) || (data->h_sum >= 60.0)))  || (rightAngleTrackingCount <= -1000));
    }
    else
    {
        return ((((rightAngleTrackingCount <= 0)) && ((data->H_ESensorValue[0] >= 35.0) || (data->H_ESensorValue[3] >= 35.0) || (data->H_ESensorValue[1] >= 35.0) || (data->H_ESensorValue[2] >= 35.0)))  || (rightAngleTrackingCount <= -1000));
    }

}

inline bool Is_RightAngleBackToStraight(data_t *data)
{
    return 1;
}

inline bool Is_Cycle(data_t *data)
{
    if(data->CarMode == LAutoBoot_Mode || data->CarMode == AI_Mode)
    {
        float sum_l = data->H_ESensorValue[0] + data->O_ESensorValue[0];
        float sum_r = data->H_ESensorValue[2] + data->O_ESensorValue[1];

        float sum = sum_l + sum_r + data->H_ESensorValue[1];

        //return (((fabs(sum_l - sum_r) >= 60.0) or (sum >= 340)) and ((sum_l >= 150) || (sum_r >= 150)) and (data->v_sum <= 50.0) and (fabs(data->v_difference) >= 5.0) and (data->o_sum >= 100.0) and (data->V_ESensorValue[0] <= 25.0) and(data->V_ESensorValue[1] <= 25.0));

        return (((fabs(sum_l - sum_r) >= 60.0) or (sum >= 340)) and ((sum_l >= 150) || (sum_r >= 150))); //and (fabs(data->v_difference) >= 5.0) and (fabs(data->h_difference) >= 20.0) and (data->o_sum >= 100.0) and (data->V_ESensorValue[0] <= 25.0) and (data->V_ESensorValue[1] <= 25.0));
        //return ((h_sum >= 160.0) and ((sum_1 >= 180.0) or (sum_2 >= 180.0)) and ((data->V_ESensorValue[0] >= 15.0) || (data->V_ESensorValue[1] >= 15.0) || (data->v_sum >= 20.0)) and (fabs(data->v_difference) >= 15.0));
    }
    else
    {
//        float sum_l = data->H_ESensorValue[0] + data->H_ESensorValue[1] + data->H_ESensorValue[2];
//        float sum_r = data->H_ESensorValue[1] + data->H_ESensorValue[2] + data->H_ESensorValue[3];

        float h_sum = data->H_ESensorValue[0] + data->H_ESensorValue[1] + data->H_ESensorValue[2] + data->H_ESensorValue[3];

        return ((h_sum >= 200.0) and ((data->V_ESensorValue[0] >= 15.0) || (data->V_ESensorValue[1] >= 15.0)) and (fabs(data->v_difference) >= 20.0));
        //return (((h_sum1 >= 160.0) or (h_sum2 >= 160.0)) and (fabs(data->o_difference) >= 40.0) and (fabs(data->v_difference) >= 10.0));//(data->o_difference >= 40.0) and ((data->V_ESensorValue[0] >= 5.0) || (data->V_ESensorValue[1] >= 5.0) || (data->v_sum >= 10.0)) and (fabs(data->v_difference) >= 5.0));
    }
}

inline float CycleInFunc(data_t *data,float x)
{
    if(data->CarMode == LAutoBoot_Mode)
        return 120* exp(-0.008 * square(x - 95));
    else if(data->CarMode == SAutoBoot_Mode)
        return 120* exp(-0.001 * square(x - 55));
    else
        return 0.0;

    //return 120* exp(-0.002 * square(x - 75));

    //return 120* exp(-0.001 * square(x - 65));

    //return (100 * exp(-0.001 * square(x - 75)));

    //return 120 * exp(-0.0008 * square(x - 75));

    //return (120* exp(-0.01 * square(x - 60.0)));//((5.0 * x * 360.0)/((1.0 + 25.0 * pow(x,4.0)) * 2 * PI));
}

inline bool Is_ArriveCycleInPoint(data_t *data,float isCyclePos,float waitInDistance)
{
    return ((data->x - isCyclePos) >= waitInDistance);
}

inline bool Is_CycleIn(data_t *data,float isCyclePos,float inDistance)
{
    return (data->h_sum <= 100.0 && data->h_bias <= 35.0 && ((data->x - isCyclePos) >= inDistance));
}

inline bool Is_CycleOut(data_t *data,uint32_t cycleOutCnt)
{
    float h_sum = 0.0;

    if(data->CarMode == LAutoBoot_Mode || data->CarMode == AI_Mode)
        h_sum = data->H_ESensorValue[0] + data->H_ESensorValue[1] + data->H_ESensorValue[2];
    else
        h_sum = data->H_ESensorValue[0] + data->H_ESensorValue[2] + data->H_ESensorValue[3];

    return (h_sum >= 180.0 && cycleOutCnt >= 1000);
}

inline bool Is_CycleBackToStraight(data_t *data)
{
    float h_sum = 0.0;

    if(data->CarMode == LAutoBoot_Mode || data->CarMode == AI_Mode)
    {
        h_sum = data->H_ESensorValue[0] + data->H_ESensorValue[1] + data->H_ESensorValue[2];
        return (h_sum <= 150.0 || (data->H_ESensorValue[0] <=50.0 && data->H_ESensorValue[2] <= 50.0));
    }
    else
    {
        h_sum = data->H_ESensorValue[0] + data->H_ESensorValue[2] + data->H_ESensorValue[3];
        return (h_sum <= 150.0 && (data->H_ESensorValue[0] <=50.0 && data->H_ESensorValue[3] <= 50.0));
    }
}


inline bool Is_Cross(data_t *data)
{
    return ((fabs(data->o_difference) >= 40.0) && (fabs((data->v_sum >= 60.0))) && (fabs(data->o_sum >= 90.0) && fabs(data->h_sum >= 70) && (fabs(data->h_bias) <= 40.0) && (fabs(data->v_difference <= 20.0))));
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
