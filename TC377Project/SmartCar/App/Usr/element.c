/*
 * element.c
 *
 *  Created on: 2020年12月6日
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
void StopSituationDetect(data_t *data);


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

//    if(loseLineCnt >= 1500 && data->CarMode != DebugMode)
//    {
//        Motor.Break(Motor.Self);
//        Servo.Break(Servo.Self);
//    }

    return data->Element.Type * 1.0;

}

/*
 * @Brief:特殊元素处理接口函数
 * */
void SpecialElementHandler(void *argv)
{
    data_t *data = (data_t *)argv;

    RightAngle_Handler(data);

    Cross_Handler(data);

    Cycle_Handler(data);

    //LoseLine_Handler(data);

    Ramp_Handler(data);

    StopSituationDetect(data);
}

void StopSituationDetect(data_t *data) {
    //GPIO Resources[27]
    static _Bool firstExecute = 1;
    if (firstExecute) {
        GPIOx.Init(&GPIO_Resources[27].GPION);
        firstExecute = 0;
    }

    if ((!GPIOx.Read(&GPIO_Resources[27].GPION)) && (os.time.getTimes() >= 10 && (data->x >= 100.0))) {
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

    static cycle_cnt_t cycleCnt = {0,0,0,0,0,0};

    static cycle_flag_t cycleFlag = {false,false,false,false,false,false};

    static float bias = 0.0;

    static float isCyclePos = 0.0;

    static float waitInDistance = 0.0;

    static float inDistance = 0.0;

    float sum_l = 0.0;
    float sum_r = 0.0;

    switch(cycleState)
    {
        case CC_Undefined:

            cycleState = CC_Exception_Handler;

            break;

        case CC_Exception_Handler:

            bias = 0.0;

            isCyclePos = 0.0;

            waitInDistance = 0.0;

            inDistance = 0.0;

            CycleClearCnt(&cycleCnt);

            CycleClearFlag(&cycleFlag);

            cycleState = CC_Wait;

            break;

        case CC_Wait:

            cycleCnt.Wait--;

            if(cycleCnt.Wait <= 0)
                cycleCnt.Wait = 0;

            if(data->Element.Type == Cycle && cycleCnt.Wait <= 0)
            {
                cycleState = CC_Config;

                isCyclePos = data->x;
            }

            break;

        case CC_Config:

            sum_l = data->HESensor[0].Value + data->VESensor[0].Value;

            sum_r = data->HESensor[3].Value + data->VESensor[1].Value;

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

            CycleClearCnt(&cycleCnt);

            CycleClearFlag(&cycleFlag);

            BLED.ON(BLED.Self);

            cycleState = CC_Confirm;

            break;

        case CC_Confirm:

            cycleCnt.Confirm--;

            if(Is_CycleConfirmed(data,&cycleCnt,&cycleFlag))
            {
                cycleState = CC_WaitIn;
            }

            if(cycleCnt.Confirm <= -2000.0)
                cycleState = CC_Undefined;

            break;

        case CC_WaitIn:

            cycleCnt.WaitIn--;

            if(Is_ArriveCycleInPoint(data,isCyclePos,waitInDistance))
            {
                cycleState = CC_In;
            }

            if(cycleCnt.WaitIn <= -2000.0)
            {
                cycleState = CC_Undefined;
            }

            break;


        case CC_In:

            cycleCnt.In--;

            data->Bias = bias;

            if(Is_CycleIn(data,isCyclePos,inDistance))
            {
                cycleState = CC_Tracking;
            }

            if(cycleCnt.In <= -2000)
            {
                cycleState = CC_Undefined;
            }

            break;

        case CC_Tracking:

            cycleCnt.Out++;

            data->Bias = ((data->h_difference + data->v_difference) / data->h_sum) * 100.0;

            data->Bias = data->Bias * 0.95 + fsign(bias) * 100.0 * 0.05;

            data->Bias = ConstrainFloat(data->Bias,-100.0,100.0);

            if(Is_CycleOut(data,cycleCnt.Out) || cycleCnt.Out >= 6000 || (data->x - isCyclePos) >= 2000.0)
            {
                cycleState = CC_Out;
            }

            break;

        case CC_Out:

            if(Is_CycleBackToStraight(data) || cycleCnt.Out >= 6000 || (data->x - isCyclePos) >= 2000.0)
            {
                cycleState = CC_Wait;
                data->Element.Type = None;

                cycleCnt.Wait = 500;

                BLED.OFF(BLED.Self);
            }

            if(cycleCnt.Out >= 6000)
            {
                cycleState = CC_Undefined;
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
                rightAngleState = RA_Config;

                goto RA_CONFIG;
            }

            break;

        case RA_Config:

            RA_CONFIG:

            rightAngleTrackingCount = 100;

            bias = fsign(data->v_difference) * 100.0;

            rightAngleState = RA_Confirm;

            goto RA_CONFIRM;

            break;

        case RA_Confirm:    //confirm and config some parameter.

            RA_CONFIRM:

            if(1)
            {
                GLED.ON(GLED.Self);

                rightAngleState = RA_Tracking;

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
