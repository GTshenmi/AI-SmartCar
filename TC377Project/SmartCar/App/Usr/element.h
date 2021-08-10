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

/*Ramp*/
inline bool Is_Ramp(data_t *data)
{
    //float distance = DSensor.GetDistance(DSensor.Self);

    //distance = distance * 1.0;

    return 0;
}
/*Ramp*/

/*Straight*/
inline bool Is_Straight(data_t *data)
{
    return (fabs(data->h_bias) <= 20.0 && data->v_sum <= 10.0 && data->h_sum >= 50.0);
}
/*Straight*/

/*Corner*/
inline bool Is_Corner(data_t *data)
{
    return (fabs(data->h_bias) >= 20.0 && data->h_sum >= 60.0 && data->v_sum >= 10.0);
}
/*Corner*/

/*RightAngle*/

inline void RightAngleClearConfig(rightangle_config *config)
{
    config->bias = 0.0;
}

inline void RightAngleClearCnt(rightangle_cnt *cnt)
{
    cnt->Tracking = 0;
}

inline bool Is_RightAngle(data_t *data)
{
    return ((fabs(data->v_difference) >= 30.0/*Judge  Right Angle*/) and (fabs(data->o_difference) <= 40.0)/*Distinguish Cross*/ and (fabs(data->v_difference/data->h_difference) >= 5.0) and (fabs(data->h_sum) <= 100.0)/*Distinguish Cycle*/);
}

inline bool Is_RightAngleOut(data_t *data,rightangle_cnt *cnt)
{    
    //Find Line.
    return ((((cnt->Tracking <= 0)/*Ensure tracking for a period of time*/) && ((data->HESensor[0].Value >= 30.0) || (data->HESensor[2].Value >= 30.0) || (data->HESensor[3].Value) || (data->HESensor[1].Value >= 30.0) || (data->h_sum >= 60.0))) /*find line*/ || (cnt->Tracking <= -1000)/*force out*/);
}

inline bool Is_RightAngleBackToStraight(data_t *data)
{
    return 1;
}
/*RightAngle*/

/*Cycle*/

inline void CycleClearConfig(cycle_config *config)
{
    config->bias = 0.0;

    config->isCyclePos = 0.0;

    config->waitInDistance = 0.0;

    config->inDistance = 0.0;

    config->inCyclePointYaw = 0.0;

    config->dYaw = 0.0;
}

inline void CycleClearCnt(cycle_cnt *cnt)
{
    cnt->Wait = 0;
    cnt->Confirm = 0;
    cnt->WaitIn = 0;
    cnt->Flags = 0;
    cnt->In = 0;
    cnt->Tracking = 0;
    cnt->Out = 0;
}

inline void CycleClearFlag(cycle_flag_t *flag)
{
    flag->isLeftHSensorFall = false;
    flag->isLeftOSensorFall = false;
    flag->isMidHSensorFall = false;
    flag->isOESensorMaxValue = false;
    flag->isRightHSensorFall = false;
    flag->isRightOSensorFall = false;
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

inline bool Is_Cycle(data_t *data)
{
    if(data->CarMode == LAutoBoot_Mode)
    {
        float sum_l = data->HESensor[0].Value + data->OESensor[0].Value;
        float sum_r = data->HESensor[3].Value + data->OESensor[1].Value;

        float sum = sum_l + sum_r + data->HESensor[1].Value;

        return (((fabs(sum_l - sum_r) >= 60.0) or (sum >= 340)) and ((sum_l >= 150) || (sum_r >= 150)));
    }
    else
    {
        return ((data->h_sum4 >= 200.0) and ((data->VESensor[0].Value >= 15.0) || (data->VESensor[1].Value >= 15.0)) and (fabs(data->v_difference) >= 20.0));
    }
}

inline bool Is_CycleConfirmed(data_t *data,cycle_cnt *cnt,cycle_flag_t *flag)
{
    if(data->OESensor[0].K >= 5.0 || data->OESensor[1].K >= 5.0)    //Find the inflection point of the oblique inductance change rate.
    {
        flag->isOESensorMaxValue = true;
    }

    if(flag->isOESensorMaxValue)  //if arrive inflection point of oblique inductance change rate，find the inflection point of other inductance change rate.
    {
        if(data->OESensor[0].K < 0.0 && !flag->isLeftOSensorFall)//count inflection point of other inductance change rate.
        {
            flag->isLeftOSensorFall = true;

            cnt->Flags++;
        }

        if(data->OESensor[1].K  < 0.0 && !flag->isRightOSensorFall)
        {
            flag->isRightOSensorFall = true;
            cnt->Flags++;
        }

        if((data->HESensor[2].K <= 0.0 || data->HESensor[3].K <= 0.0) && !flag->isMidHSensorFall)
        {
            flag->isMidHSensorFall = true;

            cnt->Flags++;
        }

        if(data->HESensor[0].K <= 0.0 && !flag->isLeftHSensorFall)
        {
            flag->isLeftHSensorFall = true;

            cnt->Flags++;
        }

        if(data->HESensor[3].K <= 0.0 && !flag->isLeftHSensorFall)
        {
            flag->isRightHSensorFall = true;

            cnt->Flags++;
        }

        if(cnt->Flags >= 2) //if inflection point > 2 , confirm it is cycle.
        {
            return true;
        }
    }

    return false;
}

inline bool Is_ArriveCycleInPoint(data_t *data,cycle_config *config)
{
    return ((data->x - config->isCyclePos) >= config->waitInDistance);//The encoder integrates a distance to reach the  cycle-in point
}

inline bool Is_CycleIn(data_t *data,cycle_config *config)
{
    return (data->h_sum <= 100.0 && data->h_bias <= 35.0/*in cycle*/ && ((data->x - config->isCyclePos) >= config->inDistance/*The encoder integrates a distance.*/));

    //return ((data->h_sum <= 100.0 && data->h_bias <= 35.0) && (((data->x - config->isCyclePos) >= config->inDistance)/*The encoder integrates a distance.*/ || (config->dYaw >= 30.0))/*The yaw angle offset exceeds 30 degrees*/);
}

inline bool Is_CycleOut(data_t *data,cycle_cnt *cnt,cycle_config *config)
{
    return (data->h_sum3 >= 180.0/*arrive in cycle point again.*/ && cnt->Tracking >= 1000/*ensure tracking for a period of time.*/);
}

inline bool Is_CycleBackToStraight(data_t *data)
{
    return (data->h_sum3 <= 150.0 && (data->HESensor[0].Value <=50.0 && data->HESensor[3].Value <= 50.0));
}
/*Cycle*/

/*Cross*/

inline void CrossClearCnt(cross_cnt *cnt)
{
    cnt->cnt = 0;
}

inline void CrossClearConfig(cross_config *config)
{
    config->interval = 0;
}

inline bool Is_Cross(data_t *data)
{
    return ((fabs(data->o_difference) >= 40.0) && ((fabs(data->o_sum) >= 90.0) && fabs(data->h_sum) >= 70 && (fabs(data->h_bias) <= 40.0)/*judge cross*/ && fabs((data->v_sum) >= 60.0)/*Distinguish cycle*/ && (fabs(data->v_difference) <= 20.0)/*Distinguish right angle*/));
}

inline bool Is_CrossOut(data_t *data)
{
    return ((fabs(data->o_difference) >= 65.0 &&\
             fabs(data->v_sum) >= 80.0));
}
/*Cross*/

inline bool Is_Boundary(data_t *data)
{
#define BoundaryGate 10.0

   uint8_t is_boundary = 0;

   is_boundary += ((data->HESensor[0].Value >= BoundaryGate) &&\
                   (data->HESensor[1].Value <= BoundaryGate) &&\
                   (data->HESensor[2].Value <= BoundaryGate) &&\
                   (data->VESensor[0].Value <= BoundaryGate) &&\
                   (data->VESensor[1].Value <= BoundaryGate) &&\
                   (data->OESensor[0].Value <= BoundaryGate) &&\
                   (data->OESensor[1].Value <= BoundaryGate) &&\
                   (data->HESensor[3].Value <= BoundaryGate));

   is_boundary += ((data->HESensor[0].Value <= BoundaryGate) &&\
                   (data->HESensor[1].Value <= BoundaryGate) &&\
                   (data->HESensor[2].Value <= BoundaryGate) &&\
                   (data->VESensor[0].Value <= BoundaryGate) &&\
                   (data->VESensor[1].Value <= BoundaryGate) &&\
                   (data->OESensor[0].Value <= BoundaryGate) &&\
                   (data->OESensor[1].Value <= BoundaryGate) &&\
                   (data->HESensor[3].Value >= BoundaryGate));

   return is_boundary;

}

#define LoseLineGateValue 10.0

inline void LoseLineClearConfig(loseline_config *config)
{
    config->bias = 0.0;
}

inline bool Is_LoseLine(data_t *data)
{
    return ((data->HESensor[0].Value <= LoseLineGateValue) &&\
            (data->HESensor[1].Value <= LoseLineGateValue) &&\
            (data->HESensor[2].Value <= LoseLineGateValue) &&\
            (data->VESensor[0].Value <= LoseLineGateValue) &&\
            (data->VESensor[1].Value <= LoseLineGateValue) &&\
            (data->OESensor[0].Value <= LoseLineGateValue) &&\
            (data->OESensor[1].Value <= LoseLineGateValue) &&\
            (data->HESensor[3].Value <= LoseLineGateValue));

    //return 0;
}

inline bool Is_SearchedLine(data_t *data)
{
    return ((data->HESensor[0].Value >= LoseLineGateValue) ||\
            (data->HESensor[1].Value >= LoseLineGateValue) ||\
            (data->HESensor[2].Value >= LoseLineGateValue) ||\
            (data->VESensor[0].Value >= LoseLineGateValue) ||\
            (data->VESensor[1].Value >= LoseLineGateValue) ||\
            (data->HESensor[3].Value >= LoseLineGateValue));
}

void SpecialElementHandler(void *argv);
float ElementDetermine(void *argv);

#endif /* APP_USR_ELEMENT_H_ */
