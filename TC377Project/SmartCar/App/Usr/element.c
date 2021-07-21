/*
 * element.c
 *
 *  Created on: 2020骞�12鏈�6鏃�
 *      Author: 936305695
 *  @Brief:
 *          This file is for the special element.
 */
#include <element.h>
#include "include.h"

void LoseLine_Handler(data_t *data);
void Cycle_Handler(data_t *data);
void Cross_Handler(data_t *data);
void RightAngle_Handler(data_t *data);
void Ramp_Handler(data_t *data);
void StopSituationDetect(void);


float ElementDetermine(void *argv)
{
    data_t *data = (data_t *)argv;

    static uint32_t loseLineCnt = 0;

    if(Is_Ramp(data))
    {
        data->Element.Type = Ramp;
    }

    if(Is_RightAngle(data))
    {
        data->Element.Type = RightAngle;
    }

    if(Is_Cycle(data))
    {
        data->Element.Type = Cycle;
    }

    if(Is_Cross(data) && data->Element.Type != Cycle && data->Element.Type != RightAngle)
    {
        data->Element.Type = Cross;
    }


    if(Is_LoseLine(data) && data->Element.Type != RightAngle && data->Element.Type != Cycle)
    {
        data->TrackingState = LoseLine;
        loseLineCnt++;

        //DebugBeepOn;
    }
    else
    {
        data->TrackingState = Normal_Tracking;
        loseLineCnt = 0;

        //DebugBeepOff;
    }

    if(loseLineCnt >= 1500 && data->CarMode != DebugMode)
    {
        Motor.Break(Motor.Self);
        Servo.Break(Servo.Self);
    }

    return data->Element.Type * 1.0;

}

/*
 * @Brief:鐗规畩鍏冪礌澶勭悊鎺ュ彛鍑芥暟
 * */
void SpecialElementHandler(void *argv)
{
    data_t *data = (data_t *)argv;

    RightAngle_Handler(data);

    Cross_Handler(data);

    Cycle_Handler(data);

    LoseLine_Handler(data);

    Ramp_Handler(data);

    StopSituationDetect();
}

void StopSituationDetect(void) {
    //GPIO Resources[27]
    static _Bool firstExecute = 1;
    if (firstExecute) {
        GPIOx.Init(&GPIO_Resources[27].GPION);
        firstExecute = 0;
    }

    if ((!GPIOx.Read(&GPIO_Resources[27].GPION)) && (os.time.getTimes() >= 10)) {
        Motor.Break(Motor.Self);
    }
}

void Ramp_Handler(data_t *data)
{

}

void RustAngle_Handler(data_t *data)
{
    data->Bias = Servo.GetMaxAngle(Servo.Self);
}

void Cycle_Handler(data_t *data)
{
    static cycle_state_t cycleState = CC_Wait;
    static cycle_dir_t   cycleDir = CC_DirUndefined;

    static float bias = 0.0;

    static sint32_t cycleWaitCnt = 0;

    static sint32_t cycleWaitInCnt = 0;

    static sint32_t cycleInCnt = 0;

    static sint32_t cycleOutCnt;

    static sint32_t cycleFlagCnt = 0;

    static bool isMidESensorMaxValue = false;
    static bool isLeftOSensorFall = false;
    static bool isRightOSensorFall = false;
    static bool isMidHSensorFall = false;

    static bool isLeftHSensorFall = false;
    static bool isRightHSensorFall = false;

    static float isCyclePos = 0.0;

    static float waitInDistance = 0.0;

    static float inDistance = 0.0;

    switch(cycleState)
    {
        case CC_Undefined:

            cycleState = CC_Exception_Handler;

            break;

        case CC_Exception_Handler:

            isCyclePos = 0.0;

            cycleState = CC_Wait;

            break;

        case CC_Wait:

            cycleWaitCnt--;

            if(cycleWaitCnt <= 0)
                cycleWaitCnt = 0;

            if(data->Element.Type == Cycle && cycleWaitCnt <= 0)
            {
                cycleState = CC_Confirm;

                isCyclePos = data->x;
            }

            break;

        case CC_Confirm:

            if(1)
            {
                float sum_l,sum_r;

                sum_l = data->H_ESensorValue[0] + data->V_ESensorValue[0];

                sum_r = data->H_ESensorValue[3] + data->V_ESensorValue[1];

                if(sum_l > sum_r)
                {
                    cycleDir = CC_DirLeft;
                }
                else
                {
                    cycleDir = CC_DirRight;
                }

                if(cycleDir == CC_DirLeft)
                {
                    bias = 100.0;
                }
                else
                {
                    bias = -100.0;
                }

                waitInDistance = data->CycleWaitInDistance;
                inDistance = data->CycleInDistance;

                BLED.ON(BLED.Self);

                cycleState = CC_WaitIn;

            }
            else
            {
                    cycleState = CC_Undefined;
                    data->Element.Exception.CC = CC_MisJudge;
                    data->Element.Exception.Info[Cycle][Cycle] = CC_Confirm;
            }

            break;

        case CC_WaitIn:

            if(Is_ArriveCycleInPoint(data,isCyclePos,waitInDistance))
            {
                cycleState = CC_In;
            }

            break;


        case CC_In:

            data->Bias = bias;

            cycleInCnt--;

            if(Is_CycleIn(data,isCyclePos,inDistance))
            {
                isLeftOSensorFall = false;
                isRightOSensorFall = false;
                isMidHSensorFall = false;
                isRightHSensorFall = false;
                isLeftHSensorFall = false;
                isMidESensorMaxValue = false;

                cycleWaitInCnt = 0;

                cycleInCnt = 0;

                cycleFlagCnt = 0;

                cycleState = CC_Tracking;
            }

            if(cycleInCnt <= -2000)
            {
                cycleState = CC_Undefined;
            }

            break;

        case CC_Tracking:

            cycleOutCnt++;

            data->Is_AdjustAngle = false;

            data->Bias = ((data->h_difference + data->v_difference) / data->h_sum) * 100.0;

            data->Bias = data->Bias * 0.95 + fsign(bias) * 100.0 * 0.05;

            data->Bias = ConstrainFloat(data->Bias,-100.0,100.0);

            if(data->CarMode == SAutoBoot_Mode)
            {
                data->Is_AdjustAngle = true;

                data->Angle = data->AIAngle;
            }

            if(Is_CycleOut(data,cycleOutCnt) || cycleOutCnt >= 6000 || (data->x - isCyclePos) >= 2000.0)
            {
                cycleState = CC_Out;
            }

            break;

        case CC_Out:

            if(Is_CycleBackToStraight(data) || cycleOutCnt >= 6000 || (data->x - isCyclePos) >= 2000.0)
            {
                cycleState = CC_Wait;
                data->Element.Type = None;

                cycleWaitCnt = 500;

                data->Is_AdjustAngle = false;
                data->Is_AdjustSpeed = false;

                BLED.OFF(BLED.Self);
            }

            if(cycleOutCnt >= 6000)
            {
                cycleState = CC_Undefined;
                data->Element.Exception.CC = CC_Err;
                data->Element.Exception.Info[Cycle][Cycle] = CC_Out;
            }

            break;

        default:

            break;
    }

    data->CycleState = cycleState;
}

void RightAngle_Handler(data_t *data)
{
    static rightangle_state_t rightAngleState = RA_Wait;

    static sint32_t rightAngleTrackingCount = 0;

    static float bias = 0.0;

    switch(rightAngleState)
    {
        case RA_Wait:

            if(data->Element.Type == RightAngle)
            {
                rightAngleState = RA_Confirm;

                goto RA_CONFIRM;
            }

            break;

        case RA_Confirm:    //confirm and config some parameter.

            RA_CONFIRM:

            if(1)
            {
                rightAngleTrackingCount = 100;

                GLED.ON(GLED.Self);

                bias = fsign(data->v_difference) * 100.0;

                rightAngleState = RA_Tracking;

                data->Bias = bias;

                goto RA_TRACKING;
            }

            break;

        case RA_Tracking:

            RA_TRACKING:

            if((data->v_difference >= 15.0) && (data->v_sum >= 15.0))
                bias = fsign(data->v_difference) * 100.0;

            data->Bias = bias;

            rightAngleTrackingCount--;

            if(Is_RightAngleOut(data,rightAngleTrackingCount))
            {
                data->Is_AdjustAngle = false;

                data->Is_AdjustSpeed = false;

                rightAngleState = RA_Out;
            }

            break;

        case RA_Out:

            if(Is_RightAngleBackToStraight(data))
            {
                GLED.OFF(GLED.Self);
                rightAngleState = RA_Wait;
                data->Element.Type = None;
            }

            break;

        case RA_Undefined:

            break;

        default:

            break;
    }
}

void LoseLine_Handler(data_t *data)
{
    static loseline_state_t loseLineState = LL_Wait;

    static float bias = 0.0;

    if(data->TrackingState == Normal_Tracking)
    {
        loseLineState = LL_Wait;
    }

    switch(loseLineState)
    {
        case LL_Wait:

            if(data->TrackingState == LoseLine)
            {
                loseLineState = LL_Lose;
            }

            break;

        case LL_Lose:

            bias = data->B[8];

            loseLineState = LL_SearchLine;

            break;

        case LL_BackSearchLine:

            break;


        case LL_SearchLine:

            data->Bias = bias;

            if(Is_SearchedLine(data))
            {
                loseLineState = LL_Searched;
            }

            break;

        case LL_Searched:

            loseLineState = LL_Wait;
            data->TrackingState = Normal_Tracking;

            break;

        case LL_Undefined:default:

            break;
    }
}

void Cross_Handler(data_t *data)
{
    static cross_state_t crossState = CS_Wait;

    static uint32_t crossCnt = 0;

    uint32_t interval = 125;

    switch(crossState)
    {
        case CS_Wait:

            if(data->Element.Type == Cross)
            {
                crossCnt = interval;

                BEEP.ON(BEEP.Self);

                crossState = CS_Confirm;
            }

            break;

        case CS_Confirm:

            crossCnt --;


            if(crossCnt <= 0)
            {
                crossCnt = interval;

                crossState = CS_In;

                BEEP.Toggle(BEEP.Self);
            }


            break;

        case CS_In:
            crossCnt --;


            if(crossCnt <= 0)
            {
                crossCnt = interval;

                crossState = CS_Tracking;

                BEEP.Toggle(BEEP.Self);
            }

            break;

        case CS_Tracking:

            crossCnt --;


            if(crossCnt <= 0)
            {
                crossCnt = interval;

                crossState = CS_Out;

                BEEP.Toggle(BEEP.Self);
            }

            break;
        case CS_Out:

            crossCnt --;


            if(crossCnt <= 0)
            {
                crossCnt = interval;

                crossState = CS_Tracking;

                BEEP.Toggle(BEEP.Self);

                BEEP.OFF(BEEP.Self);

                crossState = CS_Wait;
                data->Element.Type = None;
            }


            break;

        default:

            break;
    }
}
